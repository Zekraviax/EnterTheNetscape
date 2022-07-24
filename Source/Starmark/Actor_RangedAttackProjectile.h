#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actor_RangedAttackProjectile.generated.h"


// Forward declarations
class ACharacter_Pathfinder;


UCLASS()
class STARMARK_API AActor_RangedAttackProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActor_RangedAttackProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

// Variables
// --------------------------------------------------

// ------------------------- 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ACharacter_Pathfinder* EntityOwner;

// ------------------------- 
	UPROPERTY()
	FVector TargetLocation;

	UPROPERTY()
	FVector MovementEachTick;

// Functions
// --------------------------------------------------

// ------------------------- 
	// Use this function to move the actor in a straight line, without using UE's built-in move functions like Launch
	// We're doing this because we don't want to use UE's built-in move functions
	// Should be called once per tick until the destination is reached
	UFUNCTION(BlueprintCallable)
	void AddToLocationCoordinates(float AddToX, float AddToY, float AddToZ);

	UFUNCTION(BlueprintCallable)
	void BeginOverlapEntity(ACharacter_Pathfinder* OverlappingEntity);
};
