#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Engine/DataTable.h"
#include "EnterTheNetscape_Variables.h"

#include "Actor_StatusEffectsLibrary.generated.h"


// Forward Declarations
class ACharacter_Pathfinder;


UCLASS()
class STARMARK_API AActor_StatusEffectsLibrary : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActor_StatusEffectsLibrary();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

// Variables
// --------------------------------------------------

// ------------------------- Data Tables
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* StatusEffectsDataTable;

// ------------------------- Special Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int RememberedVariableOne = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ACharacter_Pathfinder* RememberedAvatarOne = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ACharacter_Pathfinder* RememberedAvatarTwo = nullptr;


// Functions
// --------------------------------------------------
	// Replicated functions can't have return values
	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void OnStatusEffectApplied(ACharacter_Pathfinder* AffectedAvatar, FAvatar_StatusEffect StatusEffectReference);

	// On Start-of-turn
	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void OnStatusEffectStartOfTurn(ACharacter_Pathfinder* AffectedAvatar, FAvatar_StatusEffect StatusEffectReference);

	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void OnStatusEffectRemoved(ACharacter_Pathfinder* AffectedAvatar, FAvatar_StatusEffect StatusEffectReference);
};