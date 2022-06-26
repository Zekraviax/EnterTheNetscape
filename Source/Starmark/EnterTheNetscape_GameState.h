#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"

#include "Engine/DataTable.h"
#include "EnterTheNetscape_Variables.h"

#include "EnterTheNetscape_GameState.generated.h"


// Forward Declarations
class ACharacter_Pathfinder;
class AEnterTheNetscape_GameMode;
class APlayerController_Battle;
class UWidgetComponent_LobbyPlayerVals;
class UWidget_HUD_Battle;


UCLASS()
class STARMARK_API AEnterTheNetscape_GameState : public AGameState
{
	GENERATED_BODY()

public:
// Variables
// --------------------------------------------------

// ------------------------- References
	UPROPERTY()
	AEnterTheNetscape_GameMode* GameModeReference;

// ------------------------- Battle
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	TArray<ACharacter_Pathfinder*> AvatarTurnOrder;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	TArray<ACharacter_Pathfinder*> DynamicAvatarTurnOrder;

	// The Index of the Avatar whose turn it is, in the AvatarTurnOrder Array.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrentAvatarTurnIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	FString CurrentTurnOrderText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	bool AvatarDiedThisTurn = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* StatusEffectsDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	FString GameStateContextString;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FAvatar_StatusEffect StunStatus;
	
// Functions
// --------------------------------------------------

// ------------------------- Battle
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SetTurnOrder(const TArray<APlayerController_Battle*>& PlayerControllers);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void AvatarBeginTurn();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void AvatarEndTurn();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void EndOfBattle();
};
