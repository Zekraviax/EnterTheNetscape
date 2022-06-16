#include "EnterTheNetscape_PlayerState.h"


#include "Character_Pathfinder.h"
#include "PlayerController_Battle.h"
#include "Player_SaveData.h"
#include "EnterTheNetscape_GameMode.h"
#include "EnterTheNetscape_GameInstance.h"
#include "EnterTheNetscape_GameState.h"
#include "Widget_HUD_Battle.h"


AEnterTheNetscape_PlayerState::AEnterTheNetscape_PlayerState()
{
	bReplicates = true;
}


void AEnterTheNetscape_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEnterTheNetscape_PlayerState, PlayerReadyStatus);
	DOREPLIFETIME(AEnterTheNetscape_PlayerState, ReplicatedPlayerName);
	DOREPLIFETIME(AEnterTheNetscape_PlayerState, PlayerState_PlayerParty);
	DOREPLIFETIME(AEnterTheNetscape_PlayerState, PlayerProfileReference);
}


// ------------------------- Player
void AEnterTheNetscape_PlayerState::UpdatePlayerData()
{
	UE_LOG(LogTemp, Warning, TEXT("UpdatePlayerData / IsValid(GetWorld()) returns: %s"), IsValid(GetWorld()) ? TEXT("true") : TEXT("false"));
	UE_LOG(LogTemp, Warning, TEXT("UpdatePlayerData / IsValid(GetGameInstance) returns: %s"), IsValid(UGameplayStatics::GetGameInstance(GetWorld())) ? TEXT("true") : TEXT("false"));
	
	if (GetWorld()) {
		if (UGameplayStatics::GetGameInstance(GetWorld())) {
			GameInstanceReference = Cast<UEnterTheNetscape_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

			ReplicatedPlayerName = GameInstanceReference->PlayerName;
			PlayerProfileReference = GameInstanceReference->CurrentProfileReference;

			UE_LOG(LogTemp, Warning, TEXT("UpdatePlayerData / IsValid(PlayerProfileReference) returns: %s"), IsValid(PlayerProfileReference) ? TEXT("true") : TEXT("false"));

			SetPlayerName(GameInstanceReference->PlayerName);
			SendUpdateToMultiplayerLobby();
		}
	}
}


void AEnterTheNetscape_PlayerState::Server_UpdatePlayerData_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Server_UpdatePlayerData / IsValid(GetWorld()) returns: %s"), IsValid(GetWorld()) ? TEXT("true") : TEXT("false"));
	UE_LOG(LogTemp, Warning, TEXT("Server_UpdatePlayerData / IsValid(GetGameInstance) returns: %s"), IsValid(UGameplayStatics::GetGameInstance(GetWorld())) ? TEXT("true") : TEXT("false"));

	//if (!GameInstanceReference)
	if (GetWorld()) {
		if (UGameplayStatics::GetGameInstance(GetWorld())) {
			if (!GameInstanceReference)
				GameInstanceReference = Cast<UEnterTheNetscape_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

			ReplicatedPlayerName = GameInstanceReference->PlayerName;
			if (!GameInstanceReference->CurrentProfileReference->IsValidLowLevel()) {
				// If the player doesn't have a profile loaded, attempt to load a default profile.
				GameInstanceReference->CurrentProfileReference = Cast<UPlayer_SaveData>(UGameplayStatics::LoadGameFromSlot("DefaultProfile", 0));

				// Check if a default profile exists. If not, create one.
				if (!GameInstanceReference->CurrentProfileReference->IsValidLowLevel()) {
					UPlayer_SaveData* DefaultProfile = Cast<UPlayer_SaveData>(UGameplayStatics::CreateSaveGameObject(UPlayer_SaveData::StaticClass()));

					// Add all available explorers to the default profile
					if (ExplorersDataTableRowNames.Num() == 0) {
						ExplorersDataTableRowNames = ExplorersDataTable->GetRowNames();
					}

					for (const FName ExplorerRowName : ExplorersDataTableRowNames) {
						FNetscapeExplorer_Struct* ExplorerDataTableRow = ExplorersDataTable->FindRow<FNetscapeExplorer_Struct>(ExplorerRowName, PlayerStateContextString);
						FNetscapeExplorer_Struct Explorer = *ExplorerDataTableRow;

						DefaultProfile->Explorers.Add(*ExplorerDataTableRow);

						if (DefaultProfile->CurrentExplorerTeam.Num() < 4) {
							Explorer.IndexInPlayerLibrary = DefaultProfile->CurrentExplorerTeam.Num();

							// Apply formulae to stats
							// Battle Stats
							// Total Battle Stat = Base Battle Stat x (Social Stat/Number) + Level
	
							Explorer.BattleStats.Strength = (ExplorersDataTable->FindRow<FNetscapeExplorer_Struct>(ExplorerRowName, PlayerStateContextString)->BattleStats.Strength * (Explorer.SocialStats.Courage / 2)) + 1;
							Explorer.BattleStats.Endurance = (ExplorersDataTable->FindRow<FNetscapeExplorer_Struct>(ExplorerRowName, PlayerStateContextString)->BattleStats.Endurance * (Explorer.SocialStats.Diligence / 2)) + 1;
							Explorer.BattleStats.Agility = (ExplorersDataTable->FindRow<FNetscapeExplorer_Struct>(ExplorerRowName, PlayerStateContextString)->BattleStats.Agility * (Explorer.SocialStats.Empathy / 2)) + 1;
							Explorer.BattleStats.Magic = (ExplorersDataTable->FindRow<FNetscapeExplorer_Struct>(ExplorerRowName, PlayerStateContextString)->BattleStats.Magic * (Explorer.SocialStats.Insight / 2)) + 1;
							Explorer.BattleStats.Luck = (ExplorersDataTable->FindRow<FNetscapeExplorer_Struct>(ExplorerRowName, PlayerStateContextString)->BattleStats.Luck * (Explorer.SocialStats.Wit / 2)) + 1;

							// Health Points
							Explorer.BattleStats.MaximumHealthPoints += Explorer.BattleStats.Endurance;

							// Mana Points
							Explorer.BattleStats.MaximumManaPoints += Explorer.BattleStats.Magic;

							// Tile Moves
							Explorer.MaximumTileMoves = 2 + FMath::RoundToInt(Explorer.BattleStats.Agility / 5);

							DefaultProfile->CurrentExplorerTeam.Add(Explorer);
						}
					}

					// Set other variables
					DefaultProfile->Name = "DefaultProfile";
					DefaultProfile->ProfileName = "DefaultProfile";

					// Save the default slot
					UGameplayStatics::SaveGameToSlot(DefaultProfile, "DefaultProfile", 0);
					GameInstanceReference->CurrentProfileReference = DefaultProfile;
				}
			}

			PlayerProfileReference = GameInstanceReference->CurrentProfileReference;
			PlayerState_PlayerParty = GameInstanceReference->CurrentProfileReference->CurrentExplorerTeam;

			UE_LOG(LogTemp, Warning, TEXT("Server_UpdatePlayerData / IsValid(PlayerProfileReference) returns: %s"), IsValid(PlayerProfileReference) ? TEXT("true") : TEXT("false"));
			UE_LOG(LogTemp, Warning, TEXT("Server_UpdatePlayerData / ReplicatedPlayerName is: %s"), *ReplicatedPlayerName);
			

			/*
			const APawn* Pawn = GetPawn();
			while (Pawn == nullptr) {
				Pawn = GetPawn();
			}
			AController* Controller = Pawn->GetController();
			Cast<APlayerController_Battle>(Controller)->PlayerParty = GameInstanceReference->CurrentProfileReference->CurrentExplorerTeam;
			*/
			// Update player controller with team
			UE_LOG(LogTemp, Warning, TEXT("Server_UpdatePlayerData / ReplicatedPlayerName is: %s"), *GetNetOwningPlayer()->GetPlayerController(GetWorld())->GetName());
			Cast<APlayerController_Battle>(GetNetOwningPlayer()->GetPlayerController(GetWorld()))->PlayerProfileReference = GameInstanceReference->CurrentProfileReference;

			SetPlayerName(GameInstanceReference->PlayerName);
			SendUpdateToMultiplayerLobby();
		}
	}
}


void AEnterTheNetscape_PlayerState::SaveToCurrentProfile()
{
	if (!GameInstanceReference)
		GameInstanceReference = Cast<UEnterTheNetscape_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (!PlayerProfileReference)
		PlayerProfileReference = GameInstanceReference->CurrentProfileReference;

	if (PlayerProfileReference) {
		PlayerProfileReference->Name = GameInstanceReference->PlayerName;

		UGameplayStatics::SaveGameToSlot(PlayerProfileReference, GameInstanceReference->CurrentProfileName, 0);
	}
}


// ------------------------- Lobby
void AEnterTheNetscape_PlayerState::SendUpdateToMultiplayerLobby_Implementation()
{
	// Implemented in Blueprints
}


// ------------------------- Battle
void AEnterTheNetscape_PlayerState::Client_UpdateReplicatedPlayerName_Implementation()
{

	Server_UpdateReplicatedPlayerName(ReplicatedPlayerName);
	Server_UpdatePlayerStateVariables(PlayerState_PlayerParty);
}


void AEnterTheNetscape_PlayerState::Server_UpdateReplicatedPlayerName_Implementation(const FString& UpdatedReplicatedPlayerName)
{
	ReplicatedPlayerName = UpdatedReplicatedPlayerName;
	SetPlayerName(UpdatedReplicatedPlayerName);
}


void AEnterTheNetscape_PlayerState::PlayerState_BeginBattle_Implementation()
{
	// Retrieve player profile
	Server_UpdatePlayerData();
}


void AEnterTheNetscape_PlayerState::Server_SubtractHealth_Implementation(ACharacter_Pathfinder* Defender, int DamageDealt)
{
	UE_LOG(LogTemp, Warning, TEXT("Server_SubtractHealth / IsValid(Defender) returns: %s"), IsValid(Defender) ? TEXT("true") : TEXT("false"));

	if (IsValid(Defender)) {
		Defender->AvatarData.CurrentHealthPoints -= DamageDealt;
		Defender->UpdateAvatarDataInPlayerParty();

		if (Defender->AvatarData.CurrentHealthPoints <= 0) {
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Avatar Defeated")));

			Battle_AvatarDefeated(Defender);
		}
	}
}


void AEnterTheNetscape_PlayerState::Server_AddHealth_Implementation(ACharacter_Pathfinder* Avatar, int Healing)
{
	UE_LOG(LogTemp, Warning, TEXT("Server_AddHealth / IsValid(Defender) returns: %s"), IsValid(Avatar) ? TEXT("true") : TEXT("false"));

	if (IsValid(Avatar)) {
		Avatar->AvatarData.CurrentHealthPoints += Healing;
		Avatar->UpdateAvatarDataInPlayerParty();

		if (Avatar->AvatarData.CurrentHealthPoints > Avatar->AvatarData.BattleStats.MaximumHealthPoints)
			Avatar->AvatarData.CurrentHealthPoints = Avatar->AvatarData.BattleStats.MaximumHealthPoints;
	}
}


void AEnterTheNetscape_PlayerState::Battle_AvatarDefeated_Implementation(ACharacter_Pathfinder* Avatar)
{
	AEnterTheNetscape_GameState* GameStateReference = Cast<AEnterTheNetscape_GameState>(GetWorld()->GetGameState());
	APlayerController_Battle* PlayerControllerReference = Avatar->PlayerControllerReference;

	if (IsValid(Avatar->PlayerControllerReference)) {
		if (Avatar->PlayerControllerReference->PlayerParty.IsValidIndex(Avatar->IndexInPlayerParty)) {
			Avatar->PlayerControllerReference->PlayerParty.RemoveAt(Avatar->IndexInPlayerParty);

			// Remove the Avatar from the turn order
			UE_LOG(LogTemp, Warning, TEXT("Battle_AvatarDefeated / Remove Avatar %s from Turn Order"), *Avatar->AvatarData.NetscapeExplorerName);
			GameStateReference->AvatarTurnOrder.Remove(Avatar);
			GameStateReference->DynamicAvatarTurnOrder.Remove(Avatar);

			Cast<AEnterTheNetscape_GameMode>(GetWorld()->GetAuthGameMode())->Server_AssembleTurnOrderText();
		}

		if (Avatar->PlayerControllerReference->PlayerParty.Num() <= 0) {
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Player has run out of Avatars")));
			GameStateReference->EndOfBattle_Implementation();
		} else {
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Player Avatars Remaining: %d"), Avatar->PlayerControllerReference->PlayerParty.Num()));

			// Check for any avatars in reserve
			bool FoundAvatarInReserve = false;
			
			for (int i = 0; i < Avatar->PlayerControllerReference->PlayerParty.Num(); i++) {
				FNetscapeExplorer_Struct FoundAvatar = Avatar->PlayerControllerReference->PlayerParty[i];
				if (FoundAvatar.CurrentHealthPoints > 0 && FoundAvatar.IndexInPlayerLibrary >= 4) {
					// To-Do: Allow the player to summon an Avatar from reserve as a special action.
					FoundAvatarInReserve = true;

					PlayerControllerReference->CurrentSelectedAvatar->CurrentSelectedAttack.AttackEffectsOnTarget.Empty();
					PlayerControllerReference->CurrentSelectedAvatar->CurrentSelectedAttack.AttackEffectsOnTarget.Add(EBattle_AttackEffects::SummonAvatar);
					PlayerControllerReference->CurrentSelectedAvatar->CurrentSelectedAttack.AttackTargetsInRange = EBattle_AttackTargetsInRange::SelectAllGridTiles;
					PlayerControllerReference->CurrentSelectedAvatar->CurrentSelectedAttack.AttackPattern = EBattle_AttackPatterns::SingleTile;
					
					break;
				}
			}
		}
	}

	Avatar->Destroy();
}


void AEnterTheNetscape_PlayerState::Server_UpdatePlayerStateVariables_Implementation(const TArray<FNetscapeExplorer_Struct>& UpdatedPlayerParty)
{
	PlayerState_PlayerParty = UpdatedPlayerParty;

	for (int i = 0; i < UpdatedPlayerParty.Num(); i++) {
		if (UpdatedPlayerParty[i].NetscapeExplorerName != "Default") {
			UE_LOG(LogTemp, Warning, TEXT("Server_UpdatePlayerStateVariables / Found Avatar %s in Player %s's PlayerState"), *UpdatedPlayerParty[i].NetscapeExplorerName, *GetPlayerName());
		}
	}
}