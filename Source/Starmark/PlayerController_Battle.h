#pragma once

#include "CoreMinimal.h"
#include "Actor_AttackEffectsLibrary.h"
#include "GameFramework/PlayerController.h"

#include "Character_Pathfinder.h"
#include "Engine.h"

#include "PlayerController_Battle.generated.h"


// Forward Declarations
class AAIController_Avatar;
class UPlayer_SaveData;
class UWidget_HUD_Battle;


// Unique Enums
UENUM(BlueprintType)
enum class E_PlayerCharacter_ClickModes : uint8
{
	E_Nothing,
	E_SelectCharacterToControl,
	E_SelectCharacterToAttack,
	E_MoveCharacter,
};


UENUM(BlueprintType)
enum class E_PlayerCharacter_HighlightModes : uint8
{
	E_None,
	E_MovePath,
	E_AttackPattern
};


UCLASS()
class STARMARK_API APlayerController_Battle : public APlayerController
{
	GENERATED_BODY()

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

public:
	APlayerController_Battle();

// Variables
// --------------------------------------------------

// ------------------------- Controller
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Controller")
	E_PlayerCharacter_ClickModes PlayerClickMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controller")
	E_PlayerCharacter_HighlightModes TileHighlightMode = E_PlayerCharacter_HighlightModes::E_MovePath;

// ------------------------- Widgets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UWidget_HUD_Battle> BattleWidgetChildClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Widgets")
	UWidget_HUD_Battle* BattleWidgetReference;

// ------------------------- Avatar
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Avatar")
	ACharacter_Pathfinder* CurrentSelectedAvatar;

	UPROPERTY()
	FTimerHandle PlayerStateTimerHandle;

// ------------------------- Player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Player")
	bool IsReadyToStartMultiplayerBattle = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Player")
	TArray<FNetscapeExplorer_Struct> PlayerParty;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Player")
	bool IsCurrentlyActingPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Player")
	UPlayer_SaveData* PlayerProfileReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Player")
	FString PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Player")
	int MultiplayerUniqueID;

// ------------------------- Grid
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	FVector CursorLocationSnappedToGrid;

// ------------------------- Other
	UPROPERTY()
	FTimerHandle DelayedEndTurnTimerHandle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor_AttackEffectsLibrary> AttackEffectsLibrary_Class;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor_AttackEffectsLibrary* AttackEffectsLibrary_Reference;

// Functions
// --------------------------------------------------

// ------------------------- Widgets
	UFUNCTION(BlueprintCallable)
	void CreateBattleWidget();

	UFUNCTION(BlueprintCallable)
	void SetBattleWidgetVariables();

	UFUNCTION(Client, Unreliable)
	void Client_GetTurnOrderText(const FString& NewTurnOrderText);

	UFUNCTION()
	void Local_GetTurnOrderText(const FString& NewTurnOrderText) const;

// ------------------------- Avatar
	UFUNCTION(BlueprintCallable, Client, Reliable)
	void OnRepNotify_CurrentSelectedAvatar();

// ------------------------- Battle
	UFUNCTION(Server, Reliable)
	void Server_GetDataFromProfile();

	UFUNCTION(Server, Reliable)
	void Server_SetReadyToStartMultiplayerBattle();

	UFUNCTION(BlueprintCallable)
	void OnPrimaryClick(AActor* ClickedActor);

	UFUNCTION(BlueprintNativeEvent)
	void SendMoveCommandToServer(FVector MoveLocation);

	UFUNCTION()
	void DelayedEndTurn();

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_SendEndOfTurnCommandToServer();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SendEndOfTurnCommandToServer();

	// Called at the end of a turn, before the currently acting Avatar changes
	UFUNCTION(Server, Unreliable)
	void Player_OnAvatarTurnChanged();

// ------------------------- Multiplayer Battle
	UFUNCTION(Client, Reliable)
	void GetAvatarUpdateFromServer(ACharacter_Pathfinder* AvatarReference, int AvatarUniqueID, bool IsCurrentlyActing, bool IsCurrentlSelectedAvatar);

	UFUNCTION()
	void LocalAvatarUpdate(ACharacter_Pathfinder* AvatarReference, int AvatarUniqueID, bool IsCurrentlyActing, bool IsCurrentlSelectedAvatar);

	UFUNCTION(Server, Reliable)
	void Client_SendLaunchAttackToServer(ACharacter_Pathfinder* Attacker, AActor* Target, const FString& AttackName);
};