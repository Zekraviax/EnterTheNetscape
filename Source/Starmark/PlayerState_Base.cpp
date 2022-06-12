#include "PlayerState_Base.h"


#include "Character_Pathfinder.h"
#include "PlayerController_Battle.h"
#include "Player_SaveData.h"


void APlayerState_Base::BeginPlay()
{
	Super::BeginPlay();

	SetPlayerName("Default");
}