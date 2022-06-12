#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"

#include "SaveData_PlayerProfilesList.generated.h"


UCLASS()
class STARMARK_API USaveData_PlayerProfilesList : public USaveGame
{
	GENERATED_BODY()
	
public:
// Variables
// --------------------------------------------------

// ------------------------- Save Data
	UPROPERTY()
	TArray<FString> PlayerProfileNames;

	UPROPERTY()
	TArray<FString> LastLoadedProfileName;
};
