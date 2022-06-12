#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "EnterTheNetscape_Variables.h"
#include "Player_SaveData.h"

#include "EnterTheNetscape_GameInstance.generated.h"


// Forward Declarations
class AActor_GridTile;


UCLASS()
class STARMARK_API UEnterTheNetscape_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
// Variables
// --------------------------------------------------

// ------------------------- Player
	UPROPERTY()
	FPlayer_Data PlayerData;

	UPROPERTY()
	FString PlayerName;

	UPROPERTY()
	FString CurrentProfileName;

	UPROPERTY()
	UPlayer_SaveData* CurrentProfileReference;


// Functions
// --------------------------------------------------

// ------------------------- Player
	UFUNCTION(BlueprintCallable)
	void LoadProfile(FString ProfileName);

// ------------------------- Level
	UFUNCTION()
	AActor_GridTile* FindTileByCoordinates(int x, int y) const;
};
