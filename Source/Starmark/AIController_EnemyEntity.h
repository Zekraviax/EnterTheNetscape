#pragma once

#include "CoreMinimal.h"
#include "AIController_Avatar.h"
#include "AIController_EnemyEntity.generated.h"


// Forward Declarations
class ACharacter_Pathfinder;


UCLASS(Blueprintable)
class STARMARK_API AAIController_EnemyEntity : public AAIController_Avatar
{
	GENERATED_BODY()

public:
// Variables
// --------------------------------------------------

// ------------------------- Self
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ACharacter_Pathfinder* SelfEntityReference;

// ------------------------- AI
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ACharacter_Pathfinder* CurrentTarget;

// Functions
// --------------------------------------------------

// ------------------------- AI
	// Step 1: Choose a target (randomly)
	UFUNCTION(BlueprintCallable)
	void StepOne_ChooseTarget();

	// Step 2: Move towards target (ominously)
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "blueprints")
	void StepTwo_MoveToTarget();
};