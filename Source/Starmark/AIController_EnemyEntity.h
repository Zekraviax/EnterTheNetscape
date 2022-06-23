#pragma once

#include "CoreMinimal.h"
#include "AIController_Avatar.h"

#include "EnterTheNetscape_Variables.h"

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAvatar_AttackStruct CurrentAttack;

// Functions
// --------------------------------------------------

// ------------------------- AI
	// Step 1: Choose a target (randomly)
	UFUNCTION(BlueprintCallable)
	void StepOne_ChooseTarget();

	// Step 2: Move towards target (ominously)
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Blueprints")
	void StepTwo_MoveToTarget();

	// Step 3: Choose attack and check whether the chosen target is in attack range (intelligently)
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Blueprints")
	void StepThree_CheckIfTargetIsInRange();

	// Step 4: Launch attack (aggressively)
	UFUNCTION(BlueprintCallable)
	void StepFour_LaunchAttack();
};