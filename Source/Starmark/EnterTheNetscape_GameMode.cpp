#include "EnterTheNetscape_GameMode.h"

#include "Actor_AttackEffectsLibrary.h"
#include "Actor_GridTile.h"
#include "Character_Pathfinder.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerController_Battle.h"
#include "PlayerPawn_Flying.h"
#include "EnterTheNetscape_GameInstance.h"
#include "EnterTheNetscape_GameState.h"
#include "EnterTheNetscape_PlayerState.h"
#include "EnterTheNetscape_Variables.h"
#include "Widget_HUD_Battle.h"


// ------------------------- Battle
void AEnterTheNetscape_GameMode::OnPlayerPostLogin(APlayerController_Battle* NewPlayerController)
{
	// Spawn and posses player pawn
	TArray<AActor*> FoundPlayerStartActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundPlayerStartActors);
	FVector Location;
	FRotator Rotation;
	FActorSpawnParameters SpawnInfo;

	if (FoundPlayerStartActors.IsValidIndex(0)) {
		if (FoundPlayerStartActors[0]->IsValidLowLevel()) {
			Location = FoundPlayerStartActors[0]->GetActorLocation();
			Rotation = FoundPlayerStartActors[0]->GetActorRotation();
		}
	}

	NewPlayerController->Possess(GetWorld()->SpawnActor<APlayerPawn_Flying>(PlayerPawnBlueprintClass, Location, Rotation, SpawnInfo));

	MultiplayerUniqueIDCounter++;
	NewPlayerController->MultiplayerUniqueID = MultiplayerUniqueIDCounter;
	UE_LOG(LogTemp, Warning, TEXT("OnPlayerPostLogin / MultiplayerUniqueIDCounter is: %d"), MultiplayerUniqueIDCounter);

	// Load player data
	Cast<AEnterTheNetscape_PlayerState>(NewPlayerController->PlayerState)->Server_UpdatePlayerData();
	Cast<AEnterTheNetscape_PlayerState>(NewPlayerController->PlayerState)->Client_UpdateReplicatedPlayerName();

	PlayerControllerReferences.Add(NewPlayerController);
	UE_LOG(LogTemp, Warning, TEXT("OnPlayerPostLogin / PlayerControllerReferences found: %d"), PlayerControllerReferences.Num());

	// Clear Combat Log
	if (CombatLogTextArray.Num() > 0)
		CombatLogTextArray.Empty();

	// When all players have joined, begin running the functions needed to start the battle
	UE_LOG(LogTemp, Warning, TEXT("OnPlayerPostLogin / Call Server_SinglePlayerBeginMultiplayerBattle()"));
	Server_SinglePlayerBeginMultiplayerBattle(NewPlayerController);
}


void AEnterTheNetscape_GameMode::Server_BeginMultiplayerBattle_Implementation()
{

}


void AEnterTheNetscape_GameMode::Server_SinglePlayerBeginMultiplayerBattle_Implementation(APlayerController_Battle* PlayerControllerReference)
{
	TArray<FNetscapeExplorer_Struct> CurrentPlayerTeam = Cast<UEnterTheNetscape_GameInstance>(PlayerControllerReferences[0]->GetGameInstance())->CurrentProfileReference->CurrentExplorerTeam;
	int SpawnedAvatarCount = 0;

	for (int j = CurrentPlayerTeam.Num() - 1; j >= 0; j--) {
		if (CurrentPlayerTeam[j].NetscapeExplorerName != "Default") {
			if (SpawnedAvatarCount < 4) {
				// Make sure each character can use all of their attacks
				for (int x = 0; x < CurrentPlayerTeam[j].Attacks.Num(); x++) {
					CurrentPlayerTeam[j].CurrentAttacks.Add(*AttacksDataTable->FindRow<FAvatar_AttackStruct>((CurrentPlayerTeam[j].Attacks[x].RowName), GameModeContextString));
				}

				// Spawn the actor
				Server_SpawnAvatar(PlayerControllerReferences[0], j + 1, CurrentPlayerTeam[j]);
				SpawnedAvatarCount++;
			}
		} else {
			Cast<UEnterTheNetscape_GameInstance>(PlayerControllerReferences[0]->GetGameInstance())->CurrentProfileReference->CurrentExplorerTeam.RemoveAt(j);
			UE_LOG(LogTemp, Warning, TEXT("Server_SinglePlayerBeginMultiplayerBattle / Remove invalid member %d from PlayerState_PlayerParty"), j);
		}
	}

	Server_MultiplayerBattleCheckAllPlayersReady();
}


void AEnterTheNetscape_GameMode::Server_MultiplayerBattleCheckAllPlayersReady_Implementation()
{
	TArray<bool> ReadyStatuses;
	TArray<AActor*> GridTilesArray;
	AEnterTheNetscape_GameState* GameStateReference = Cast<AEnterTheNetscape_GameState>(GetWorld()->GetGameState());

	for (int i = 0; i < PlayerControllerReferences.Num(); i++) {
		ReadyStatuses.Add(PlayerControllerReferences[i]->IsReadyToStartMultiplayerBattle);
	}

	// Assemble turn order text
	if (ReadyStatuses.Contains(false) || ReadyStatuses.Num() < ExpectedPlayers) {
		GetWorld()->GetTimerManager().SetTimer(PlayerReadyCheckTimerHandle, this, &AEnterTheNetscape_GameMode::Server_MultiplayerBattleCheckAllPlayersReady, 0.5f, false);
	} else {
		// Only set the turn order once all entities are spawned
		for (int i = 0; i < PlayerControllerReferences.Num(); i++) {
			GameStateReference->SetTurnOrder(PlayerControllerReferences);
		}

		Server_AssembleTurnOrderText();

		for (int i = 0; i < PlayerControllerReferences.Num(); i++) {
			for (int j = 0; j < GameStateReference->AvatarTurnOrder.Num(); j++) {
				if (PlayerControllerReferences[i]->MultiplayerUniqueID == GameStateReference->AvatarTurnOrder[j]->MultiplayerControllerUniqueID) {
					PlayerControllerReferences[i]->CurrentSelectedAvatar = GameStateReference->AvatarTurnOrder[j];
					break;
				}
			}

			PlayerControllerReferences[i]->SetBattleWidgetVariables();
		}

		Server_UpdateAllAvatarDecals();
		
		// Set first Avatar's controller as the currently acting player
		GameStateReference->AvatarTurnOrder[0]->PlayerControllerReference->IsCurrentlyActingPlayer = true;
	}
}


void AEnterTheNetscape_GameMode::Server_AssembleTurnOrderText_Implementation()
{
	FString NewTurnOrderText;

	if (GameStateReference == nullptr) {
		GameStateReference = Cast<AEnterTheNetscape_GameState>(GetWorld()->GetGameState());
	}

	for (int i = 0; i < GameStateReference->AvatarTurnOrder.Num(); i++) {
		if (GameStateReference->AvatarTurnOrder[i]->PlayerControllerReference->IsValidLowLevel()) {
			AEnterTheNetscape_PlayerState* PlayerStateReference = Cast<AEnterTheNetscape_PlayerState>(GameStateReference->AvatarTurnOrder[i]->PlayerControllerReference->PlayerState);

			// To-Do: Differentiate between player controlled entities and enemy entities in the turn order list.
			NewTurnOrderText.Append(GameStateReference->AvatarTurnOrder[i]->AvatarData.Nickname + "\n");
		} else {
			NewTurnOrderText.Append(GameStateReference->AvatarTurnOrder[i]->AvatarData.Nickname + " / ?\n");
		}
	}

	GameStateReference->CurrentTurnOrderText = NewTurnOrderText;

	for (int i = 0; i < PlayerControllerReferences.Num(); i++) {
		PlayerControllerReferences[i]->Client_GetTurnOrderText(GameStateReference->CurrentTurnOrderText);
		PlayerControllerReferences[i]->Local_GetEntitiesInTurnOrder(GameStateReference->DynamicAvatarTurnOrder, GameStateReference->CurrentAvatarTurnIndex);
	}
}


void AEnterTheNetscape_GameMode::Server_SpawnAvatar_Implementation(APlayerController_Battle* PlayerController, int IndexInPlayerParty, FNetscapeExplorer_Struct AvatarData)
{
	FString ContextString;
	const FActorSpawnParameters SpawnInfo;
	TArray<AActor*> FoundGridTileActors, ValidMultiplayerSpawnTiles;
	TArray<FName> AvatarRowNames;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor_GridTile::StaticClass(), FoundGridTileActors);


	// Find all tiles that can spawn avatars in multiplayer battles
	for (int i = 0; i < FoundGridTileActors.Num(); i++) {
		const AActor_GridTile* GridTileReference = Cast<AActor_GridTile>(FoundGridTileActors[i]);

		if (GridTileReference->Properties.Contains(E_GridTile_Properties::E_PlayerAvatarSpawn) && GridTileReference->AvatarSlotNumber == IndexInPlayerParty) {
			ValidMultiplayerSpawnTiles.Add(FoundGridTileActors[i]);
		}
	}

	FVector Location = ValidMultiplayerSpawnTiles[0]->GetActorLocation();
	Location.Z = 1000;

	ACharacter_Pathfinder* NewCharacter = GetWorld()->SpawnActor<ACharacter_Pathfinder>(AvatarBlueprintClass, Location, FRotator::ZeroRotator, SpawnInfo);
	NewCharacter->AvatarData = AvatarData;

	// Avatar Stats
	NewCharacter->AvatarData.CurrentHealthPoints = NewCharacter->AvatarData.BattleStats.MaximumHealthPoints;
	NewCharacter->AvatarData.CurrentManaPoints = NewCharacter->AvatarData.BattleStats.MaximumManaPoints;
	NewCharacter->AvatarData.CurrentTileMoves = NewCharacter->AvatarData.MaximumTileMoves;

	// MultiplayerUniqueID
	NewCharacter->PlayerControllerReference = PlayerController;
	NewCharacter->MultiplayerControllerUniqueID = PlayerController->MultiplayerUniqueID;

	// Get attacks
	for (int i = 0; i < AvatarData.CurrentAttacks.Num(); i++) {
		NewCharacter->CurrentKnownAttacks.Add(AvatarData.CurrentAttacks[i]);
	}

	// Send data to Clients
	NewCharacter->Client_GetAvatarData(NewCharacter->AvatarData);

	PlayerController->CurrentSelectedAvatar = NewCharacter;
	PlayerController->OnRepNotify_CurrentSelectedAvatar();
	
	// Set spawn tile to be occupied
	if (Cast<AActor_GridTile>(ValidMultiplayerSpawnTiles[0])->Properties.Contains(E_GridTile_Properties::E_None))
		Cast<AActor_GridTile>(ValidMultiplayerSpawnTiles[0])->Properties.Remove(E_GridTile_Properties::E_None);

	Cast<AActor_GridTile>(ValidMultiplayerSpawnTiles[0])->Properties.Add(E_GridTile_Properties::E_Occupied);
	Cast<AActor_GridTile>(ValidMultiplayerSpawnTiles[0])->OccupyingActor = NewCharacter;
}


void AEnterTheNetscape_GameMode::Server_UpdateAllAvatarDecals_Implementation()
{
	TArray<AActor*> Avatars;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter_Pathfinder::StaticClass(), Avatars);
	AEnterTheNetscape_GameState* GameStateReference = Cast<AEnterTheNetscape_GameState>(GetWorld()->GetGameState());
	ACharacter_Pathfinder* CurrentlyActingAvatar;

	if (GameStateReference->AvatarTurnOrder.IsValidIndex(GameStateReference->CurrentAvatarTurnIndex))
		CurrentlyActingAvatar = GameStateReference->AvatarTurnOrder[GameStateReference->CurrentAvatarTurnIndex];
	else 
		CurrentlyActingAvatar = GameStateReference->AvatarTurnOrder[0];

	for (int i = 0; i < Avatars.Num(); i++) {
		ACharacter_Pathfinder* FoundActor = Cast<ACharacter_Pathfinder>(Avatars[i]);

		if (IsValid(FoundActor->PlayerControllerReference)) {
			bool IsCurrentlyActing = false;

			if (CurrentlyActingAvatar == Avatars[i])
				IsCurrentlyActing = true;

			FoundActor->MultiplayerControllerUniqueID = FoundActor->PlayerControllerReference->MultiplayerUniqueID;

			for (int j = 0; j < PlayerControllerReferences.Num(); j++) {
				PlayerControllerReferences[j]->GetAvatarUpdateFromServer(FoundActor, FoundActor->MultiplayerControllerUniqueID, IsCurrentlyActing, true);
			}
		}
	}
}


void AEnterTheNetscape_GameMode::Server_LaunchAttack_Implementation(ACharacter_Pathfinder* Attacker, AActor* Target, const FString& AttackName)
{
	FAvatar_AttackStruct AttackData;
	FActorSpawnParameters SpawnInfo;
	FString ContextString, MoveTypeAsString, TargetTypeAsString;
	TArray<FName> ComplexAttackRowNames = AvatarComplexAttacksDataTable->GetRowNames();
	ACharacter_Pathfinder* TargetAsCharacter = Cast<ACharacter_Pathfinder>(Target);
	//int AttackerStat, DefenderStat;

	UE_LOG(LogTemp, Warning, TEXT("Server_LaunchAttack / Attack chosen: %s"), *AttackName);
	for (int i = 0; i < ComplexAttackRowNames.Num(); i++) {
		if (AvatarComplexAttacksDataTable->FindRow<FAvatar_AttackStruct>(ComplexAttackRowNames[i], ContextString)->Name == AttackName) {
			AttackData = *AvatarComplexAttacksDataTable->FindRow<FAvatar_AttackStruct>(ComplexAttackRowNames[i], ContextString);
			break;
		}
	}

	if (IsValid(TargetAsCharacter) && AttackData.AttackCategory == EBattle_AttackCategories::Offensive) {
		int CurrentDamage = 1;
		
		// Check for the No Friendly Fire attack ability
		if (AttackData.AttackEffectsOnTarget.Contains(EBattle_AttackEffects::NoFriendlyFire)) {
			if (Attacker->MultiplayerControllerUniqueID == TargetAsCharacter->MultiplayerControllerUniqueID) {
				return;
			}
		}

		// Calculate damage for moves with variable base damage
		if (AttackData.AttackEffectsOnTarget.Contains(EBattle_AttackEffects::LowerTargetHealthEqualsHigherDamageDealt)) {
			float VariableBasePower = (float)TargetAsCharacter->AvatarData.CurrentHealthPoints / (float)TargetAsCharacter->AvatarData.BattleStats.MaximumHealthPoints;
			VariableBasePower = 0.9 - VariableBasePower;
			VariableBasePower = VariableBasePower * 100.f;
			VariableBasePower += 10;

			if (VariableBasePower < 1)
				VariableBasePower = 1;

			AttackData.BasePower = FMath::CeilToInt(VariableBasePower);
		}

		// To-Do: Get either the Physical or Special stats, based on the move
		// Standard Attack Damage Formula
		CurrentDamage = FMath::CeilToInt(Attacker->AvatarData.BattleStats.Strength + AttackData.BasePower);
		UE_LOG(LogTemp, Warning, TEXT("Server_LaunchAttack / Attacker's Attack + Attack's Base Power is: %d"), CurrentDamage);
		CurrentDamage = FMath::CeilToInt(CurrentDamage - TargetAsCharacter->AvatarData.BattleStats.Endurance);
		UE_LOG(LogTemp, Warning, TEXT("Server_LaunchAttack / Current Damage - Defender's Defence is: %d"), CurrentDamage);

		// Get the averages of the attacker' and defender's combat stats and use them in the damage calculation
		// Clamp the value between 0.5 and 1.5 so we can use it as a multiplier

		// Ensure that at least 1 damage is dealt
		if (CurrentDamage < 1)
			CurrentDamage = 1;

		UE_LOG(LogTemp, Warning, TEXT("Server_LaunchAttack / Calculated damage is: %d"), CurrentDamage);

		// Subtract Health
		//AEnterTheNetscape_PlayerState* PlayerStateReference = Cast<AEnterTheNetscape_PlayerState>(Attacker->PlayerControllerReference->PlayerState);
		//PlayerStateReference->Server_SubtractHealth_Implementation(TargetAsCharacter, CurrentDamage);
		//GetWorld()->State
		Cast<AEnterTheNetscape_PlayerState>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerState)->Server_SubtractHealth_Implementation(TargetAsCharacter, CurrentDamage);

		// Restore half of the heal if the attacker has Vampirism
		/*
		FAvatar_StatusEffect* VampirismStatus = StatusEffectsDataTable->FindRow<FAvatar_StatusEffect>("Vampirism", ContextString);
		for (int i = 0; i < TargetAsCharacter->CurrentStatusEffectsArray.Num(); i++) {
			if (TargetAsCharacter->CurrentStatusEffectsArray[i] == *VampirismStatus) {
				//CurrentDamage += CurrentDamage * 0.5;
				PlayerStateReference->Server_AddHealth(Attacker, FMath::CeilToInt(CurrentDamage * 0.5));
				break;
			}
		}
		*/
	}


	// Apply move effects after the damage has been dealt
	if (!AttackEffectsLibrary_Reference && AttackEffectsLibrary_Class) {
		for (int i = 0; i < AttackData.AttackEffectsOnTarget.Num(); i++) {
			AttackEffectsLibrary_Reference = GetWorld()->SpawnActor<AActor_AttackEffectsLibrary>(AttackEffectsLibrary_Class, FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);
			AttackEffectsLibrary_Reference->SwitchOnAttackEffect(AttackData.AttackEffectsOnTarget[i], Attacker, Target);

			if (AttackData.Name == "Drown") {
				Cast<AEnterTheNetscape_GameState>(GetWorld()->GetGameState())->SetTurnOrder(PlayerControllerReferences);

				// Re-set the turn order text
				Server_AssembleTurnOrderText();

				// Call the EndTurn function again (?)
				Cast<AEnterTheNetscape_GameState>(GetWorld()->GetGameState())->AvatarEndTurn();
			}
		}
	}
}


void AEnterTheNetscape_GameMode::EndOfBattle_Implementation()
{
	// Implemented in Blueprints
}