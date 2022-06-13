#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"

#include "EnterTheNetscape_Variables.h"

#include "Player_SaveData.generated.h"


UCLASS()
class STARMARK_API UPlayer_SaveData : public USaveGame
{
	GENERATED_BODY()

public:
// Variables
// --------------------------------------------------

// ------------------------- Base
	UPROPERTY()
	FString Name;

	UPROPERTY()
	FString ProfileName;

// ------------------------- Avatars
	UPROPERTY()
	TArray<FNetscapeExplorer_Struct> Explorers;

	UPROPERTY()
	TArray<FNetscapeExplorer_Struct> CurrentExplorerTeam;
};
