#pragma once

#include "CoreMinimal.h"

#include "Character_Pathfinder.h"
#include "Components/StaticMeshComponent.h"

#include "Character_NonAvatarEntity.generated.h"


// Forward Declarations
class ACharacter_Pathfinder;


// Unique Enums
UENUM(BlueprintType)
enum class E_NonAvatarEntity_EntityType : uint8
{
	RockWall,
	Hurricane,
	Fire,
};


UCLASS()
class STARMARK_API ACharacter_NonAvatarEntity : public ACharacter_Pathfinder
{
	GENERATED_BODY()

public:
	ACharacter_NonAvatarEntity();

// Variables
// --------------------------------------------------

// ------------------------- Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* CubeComponent;

// ------------------------- Entity
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	E_NonAvatarEntity_EntityType EntityType;

// ------------------------- Non-Avatar Entity
	UPROPERTY()
	bool CanBeClicked = false;

	UPROPERTY()
	bool CanBeHurtByAttacks = false;

// Functions
// --------------------------------------------------

// ------------------------- Hurricane
	UFUNCTION()
	void HurricaneOnSpawn();

	UFUNCTION(BlueprintCallable)
	void HurricaneOnAvatarCollision(ACharacter_Pathfinder* Avatar);
};