#pragma once

#include "CoreMinimal.h"
#include "Character_NonAvatarEntity.h"
#include "Character_HatTrick.generated.h"


// Forward Declarations
class AActor_GridTile;
class ACharacter_Pathfinder;


UCLASS()
class STARMARK_API ACharacter_HatTrick : public ACharacter_NonAvatarEntity
{
	GENERATED_BODY()

public:
	ACharacter_HatTrick();

// Variables
// --------------------------------------------------

// -------------------------
	UPROPERTY()
	TArray<AActor_GridTile*> HatTilesArray;

	UPROPERTY()
	ACharacter_Pathfinder* OwningAvatar;

	UPROPERTY()
	bool IsOwnerHiddenInThisHat = false;


// Functions
// --------------------------------------------------

// -------------------------
};
