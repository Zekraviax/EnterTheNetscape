#include "PlayerController_Battle.h"


#include "Actor_AttackEffectsLibrary.h"
#include "Actor_GridTile.h"
#include "AIController.h"
#include "Character_HatTrick.h"
#include "Engine/World.h"
#include "EnterTheNetscape_GameInstance.h"
#include "EnterTheNetscape_GameMode.h"
#include "EnterTheNetscape_GameState.h"
#include "EnterTheNetscape_PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Player_SaveData.h"
#include "Widget_HUD_Battle.h"
#include "WidgetComponent_AvatarBattleData.h"


APlayerController_Battle::APlayerController_Battle()
{
	bShowMouseCursor = true;

	IsCurrentlyActingPlayer = false;
	PlayerClickMode = E_PlayerCharacter_ClickModes::E_Nothing;

	// Multiplayer
	bReplicates = true;
}


void APlayerController_Battle::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps); 

	DOREPLIFETIME(APlayerController_Battle, CurrentSelectedAvatar);
	DOREPLIFETIME(APlayerController_Battle, BattleWidgetReference);
	DOREPLIFETIME(APlayerController_Battle, IsCurrentlyActingPlayer);
	DOREPLIFETIME(APlayerController_Battle, PlayerClickMode);
	DOREPLIFETIME(APlayerController_Battle, PlayerParty);
	DOREPLIFETIME(APlayerController_Battle, PlayerProfileReference);
	DOREPLIFETIME(APlayerController_Battle, IsReadyToStartMultiplayerBattle);
	DOREPLIFETIME(APlayerController_Battle, MultiplayerUniqueID);
	DOREPLIFETIME(APlayerController_Battle, PlayerName);
}


void APlayerController_Battle::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();
}


void APlayerController_Battle::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	SetBattleWidgetVariables();
}


// ------------------------- Widgets
void APlayerController_Battle::CreateBattleWidget()
{
	if (BattleWidgetChildClass && IsLocalPlayerController() && !BattleWidgetReference) {
		BattleWidgetReference = CreateWidget<UWidget_HUD_Battle>(this, BattleWidgetChildClass);

		if (IsValid(BattleWidgetReference))
			BattleWidgetReference->AddToViewport();
	}
}


void APlayerController_Battle::SetBattleWidgetVariables()
{
	if (IsValid(BattleWidgetReference) && IsValid(CurrentSelectedAvatar)) {
		if (BattleWidgetReference->PlayerControllerReference != this)
			BattleWidgetReference->PlayerControllerReference = this;

		// To-Do: Fix these
		BattleWidgetReference->AvatarBattleDataWidget->UpdateAvatarData(CurrentSelectedAvatar->AvatarData);
		//BattleWidgetReference->UpdateAvatarAttacksComponents();
		BattleWidgetReference->AvatarBattleDataWidget->GetAvatarStatusEffects(CurrentSelectedAvatar->CurrentStatusEffectsArray);
	}
}


void APlayerController_Battle::Client_GetTurnOrderText_Implementation(const FString& NewTurnOrderText)
{
	Local_GetTurnOrderText(NewTurnOrderText);
}


void APlayerController_Battle::Local_GetTurnOrderText(const FString & NewTurnOrderText) const
{
	BattleWidgetReference->UpdateTurnOrderText(NewTurnOrderText);
}


void APlayerController_Battle::Local_GetEntitiesInTurnOrder(TArray<ACharacter_Pathfinder*> TurnOrderArray, int IndexOfCurrentlyActingEntity)
{
	BattleWidgetReference->SetUiIconsInTurnOrder(TurnOrderArray, IndexOfCurrentlyActingEntity);
}


// ------------------------- Avatar
void APlayerController_Battle::OnRepNotify_CurrentSelectedAvatar_Implementation()
{
	AEnterTheNetscape_PlayerState* PlayerStateReference = Cast<AEnterTheNetscape_PlayerState>(PlayerState);

	// (Default) Player party initialization
	if (PlayerStateReference) {
		PlayerStateReference->PlayerState_BeginBattle();

		// Avatar initialization
		if (CurrentSelectedAvatar) {
			CurrentSelectedAvatar->BeginPlayWorkaroundFunction_Implementation(BattleWidgetReference);

			// Widget initialization
			if (!IsValid(BattleWidgetReference))
				CreateBattleWidget();

			if (IsValid(BattleWidgetReference))
				SetBattleWidgetVariables();

			if (!IsReadyToStartMultiplayerBattle)
				Server_SetReadyToStartMultiplayerBattle();
		} else {
			GetWorld()->GetTimerManager().SetTimer(PlayerStateTimerHandle, this, &APlayerController_Battle::OnRepNotify_CurrentSelectedAvatar, 0.2f, false);
		}
	} else {
		GetWorld()->GetTimerManager().SetTimer(PlayerStateTimerHandle, this, &APlayerController_Battle::OnRepNotify_CurrentSelectedAvatar, 0.2f, false);
	}
}


void APlayerController_Battle::Server_SetReadyToStartMultiplayerBattle_Implementation()
{
	IsReadyToStartMultiplayerBattle = true;
}


// ------------------------- Battle
void APlayerController_Battle::Server_GetDataFromProfile_Implementation()
{
	// ReSharper disable once CppLocalVariableMayBeConst
	UEnterTheNetscape_GameInstance* GameInstanceReference = Cast<UEnterTheNetscape_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	PlayerProfileReference = GameInstanceReference->CurrentProfileReference;
	PlayerName = GameInstanceReference->PlayerName;
	PlayerParty = GameInstanceReference->CurrentProfileReference->CurrentExplorerTeam;

	UE_LOG(LogTemp, Warning, TEXT("Server_GetDataFromProfile / IsValid(PlayerProfileReference) returns: %s"), IsValid(PlayerProfileReference) ? TEXT("true") : TEXT("false"));
	UE_LOG(LogTemp, Warning, TEXT("Server_GetDataFromProfile / PlayerName is: %s"), *PlayerName);
}


void APlayerController_Battle::OnPrimaryClick(AActor* ClickedActor, TArray<AActor*> ValidTargetsArray)
{
	TArray<AActor*> AttackEffectsLibraries;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor_AttackEffectsLibrary::StaticClass(), AttackEffectsLibraries);

	CurrentSelectedAvatar->ValidAttackTargetsArray = ValidTargetsArray;

	if (ClickedActor->IsValidLowLevel()) {
		// The Hat Trick functions should take priority over other moves
		if (ClickedActor->GetClass()->GetFullName().Contains("HatTrick")) {
			// Hide avatar
			Cast<ACharacter_HatTrick>(ClickedActor)->IsOwnerHiddenInThisHat = true;
			CurrentSelectedAvatar->SetActorLocation(ClickedActor->GetActorLocation());
			CurrentSelectedAvatar->SetActorHiddenInGame(true);

			// End avatar turn
			Cast<AEnterTheNetscape_GameState>(GetWorld()->GetGameState())->AvatarEndTurn();
		} else if (CurrentSelectedAvatar->CurrentSelectedAttack.AttackEffectsOnTarget.Contains(EBattle_AttackEffects::SpawnHats)) {
			if (AttackEffectsLibraries.Num() <=  0) {
				AttackEffectsLibrary_Reference = GetWorld()->SpawnActor<AActor_AttackEffectsLibrary>(AttackEffectsLibrary_Class);
				UE_LOG(LogTemp, Warning, TEXT("OnPrimaryClick / Spawn AttackEffectsLibrary_Reference"));
			}

			if (AttackEffectsLibrary_Reference->HatTilesArray.Num() < 3) {
				if (Cast<AActor_GridTile>(ClickedActor)) {
					AttackEffectsLibrary_Reference->HatTilesArray.Add(Cast<AActor_GridTile>(ClickedActor));
					UE_LOG(LogTemp, Warning, TEXT("OnPrimaryClick / Add selected tile to HatTilesArray"));
				}
			}
		} else if (CurrentSelectedAvatar->CurrentSelectedAttack.AttackEffectsOnTarget.Contains(EBattle_AttackEffects::Chirp_Peck)) {

		} else if (CurrentSelectedAvatar->CurrentSelectedAttack.Name != "Default" && 
					CurrentSelectedAvatar->CurrentSelectedAttack.Name != "None" && 
					CurrentSelectedAvatar->CurrentSelectedAttack.Name != "---" && 
					ValidTargetsArray.Num() > 0) {
			// Subtract attack's MP cost
			CurrentSelectedAvatar->AvatarData.CurrentManaPoints -= CurrentSelectedAvatar->CurrentSelectedAttack.ManaCost;

			if (ClickedActor &&
				CurrentSelectedAvatar->CurrentSelectedAttack.AttackTargetsInRange == EBattle_AttackTargetsInRange::AttackClickedAvatar) {
				// Find an entity amongst the valid targets
				for (AActor* Actor : ValidTargetsArray) {
					if (Cast<ACharacter_Pathfinder>(Actor)) {
						ClickedActor = Cast<ACharacter_Pathfinder>(Actor);
						break;
					}
				}

				// If we're attacking, and we clicked on a valid target in-range, launch an attack
				UE_LOG(LogTemp, Warning, TEXT("OnPrimaryClick / Launch attack against ClickedActor"));
				UE_LOG(LogTemp, Warning, TEXT("OnPrimaryClick / ClickedActor is: %s"), *ClickedActor->GetFullName());
				
				if (Cast<ACharacter_Pathfinder>(ClickedActor)) {
					if (CurrentSelectedAvatar->ValidAttackTargetsArray.Contains(ClickedActor)) {
						CurrentSelectedAvatar->LaunchAttack_Implementation(Cast<ACharacter_Pathfinder>(ClickedActor));
						Client_SendEndOfTurnCommandToServer();
					}
				}
			} else if (CurrentSelectedAvatar->CurrentSelectedAttack.AttackTargetsInRange == EBattle_AttackTargetsInRange::AttackAllTargets) {
				UE_LOG(LogTemp, Warning, TEXT("OnPrimaryClick / Launch attack against all valid targets in range: %d"), CurrentSelectedAvatar->ValidAttackTargetsArray.Num());

				for (int i = 0; i < CurrentSelectedAvatar->ValidAttackTargetsArray.Num(); i++) {
					if (Cast<ACharacter_Pathfinder>(CurrentSelectedAvatar->ValidAttackTargetsArray[i])) {
						UE_LOG(LogTemp, Warning, TEXT("OnPrimaryClick / AttackTarget is: %s"), *CurrentSelectedAvatar->ValidAttackTargetsArray[i]->GetFullName());
						CurrentSelectedAvatar->LaunchAttack_Implementation(Cast<ACharacter_Pathfinder>(CurrentSelectedAvatar->ValidAttackTargetsArray[i]));
					}
				}

				Client_SendEndOfTurnCommandToServer();
			}
		}
	}
}


void APlayerController_Battle::DelayedEndTurn()
{
	GetWorld()->GetTimerManager().SetTimer(PlayerStateTimerHandle, this, &APlayerController_Battle::OnRepNotify_CurrentSelectedAvatar, 1.f, false);
}


void APlayerController_Battle::SendMoveCommandToServer_Implementation(FVector MoveLocation)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Send Move Command To Server")));
}


void APlayerController_Battle::Client_SendEndOfTurnCommandToServer_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Client_SendEndOfTurnCommandToServer / Call SendEndOfTurnCommandToServer()"));

	CurrentSelectedAvatar->CurrentSelectedAttack.Name = "Default";
	CurrentSelectedAvatar->CurrentSelectedAttack.AttackPattern = EBattle_AttackPatterns::Circle;

	SendEndOfTurnCommandToServer();
}


void APlayerController_Battle::SendEndOfTurnCommandToServer_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("SendEndOfTurnCommandToServer / Call AvatarEndTurn_Implementation()"));
	Cast<AEnterTheNetscape_GameState>(GetWorld()->GetGameState())->AvatarEndTurn_Implementation();
}


void APlayerController_Battle::Player_OnAvatarTurnChanged_Implementation()
{
	TArray<AActor*> GridTilesArray;
	SetBattleWidgetVariables();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor_GridTile::StaticClass(), GridTilesArray);
	for (int j = 0; j < GridTilesArray.Num(); j++) {
		Cast<AActor_GridTile>(GridTilesArray[j])->SetTileHighlightProperties(false, true, E_GridTile_ColourChangeContext::Normal);
	}
}


// ------------------------- Multiplayer Battle
void APlayerController_Battle::GetAvatarUpdateFromServer_Implementation(ACharacter_Pathfinder* AvatarReference, int AvatarUniqueID, bool IsCurrentlyActing, bool IsCurrentlSelectedAvatar)
{
	if (IsValid(AvatarReference)) {
		LocalAvatarUpdate(AvatarReference, AvatarUniqueID, IsCurrentlyActing, IsCurrentlSelectedAvatar);
	}
}


void APlayerController_Battle::LocalAvatarUpdate(ACharacter_Pathfinder* AvatarReference, int AvatarUniqueID, bool IsCurrentlyActing, bool IsCurrentlSelectedAvatar)
{
	ACharacter_Pathfinder* FoundActor = AvatarReference;

	if (AvatarUniqueID == MultiplayerUniqueID)
		FoundActor->ActorSelected_DynamicMaterial_Colour = FLinearColor::Green;
	else
		FoundActor->ActorSelected_DynamicMaterial_Colour = FLinearColor::Red;

	FoundActor->ActorSelectedPlane->SetHiddenInGame(!IsCurrentlyActing);
	FoundActor->ActorSelectedDynamicMaterialColourUpdate();

	SetBattleWidgetVariables();
}


void APlayerController_Battle::Client_SendLaunchAttackToServer_Implementation(ACharacter_Pathfinder* Attacker, AActor* Target, const FString& AttackName)
{
	UE_LOG(LogTemp, Warning, TEXT("Client_SendLaunchAttackToServer / Attack chosen: %s"), *AttackName);
	Cast<AEnterTheNetscape_GameMode>(GetWorld()->GetAuthGameMode())->Server_LaunchAttack(Attacker, Target, AttackName);
}