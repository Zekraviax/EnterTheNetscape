#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlayerState_Base.generated.h"


// Forward Declarations
class ACharacter_Pathfinder;
class UPlayer_SaveData;


UCLASS()
class STARMARK_API APlayerState_Base : public APlayerState
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
// Variables
// --------------------------------------------------

// ------------------------- Player
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPlayer_SaveData* PlayerProfileReference;

// ------------------------- Battle
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ACharacter_Pathfinder* PlayerState_CurrentControlledAvatar;

// Functions
// --------------------------------------------------

};
