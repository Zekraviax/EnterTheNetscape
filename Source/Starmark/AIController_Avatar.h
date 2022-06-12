#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "AIController_Avatar.generated.h"


// Forward Declarations
class ACharacter_Pathfinder;


UCLASS()
class STARMARK_API AAIController_Avatar : public AAIController
{
	GENERATED_BODY()

public:
// Functions
// --------------------------------------------------

// ------------------------- AI
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void GetMoveCommandFromPlayer(FVector MoveLocation, int TilesPerTurnCost, ACharacter_Pathfinder* AvatarReference);
};
