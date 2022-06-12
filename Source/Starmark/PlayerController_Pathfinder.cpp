#include "PlayerController_Pathfinder.h"


#include "NavigationSystem.h"
#include "AI/NavigationSystemBase.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Character_Pathfinder.h"
#include "Engine/World.h"


APlayerController_Pathfinder::APlayerController_Pathfinder()
{
	bShowMouseCursor = true;
}

void APlayerController_Pathfinder::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void APlayerController_Pathfinder::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();
}