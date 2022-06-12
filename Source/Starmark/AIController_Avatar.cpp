#include "AIController_Avatar.h"

#include "Character_Pathfinder.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"


// ------------------------- AI
void AAIController_Avatar::GetMoveCommandFromPlayer_Implementation(FVector MoveLocation, int TilesPerTurnCost, ACharacter_Pathfinder* AvatarReference)
{
	AvatarReference->AvatarData.CurrentTileMoves -= TilesPerTurnCost;
}