#include "Actor_AbilitiesLibrary.h"

#include "Character_Pathfinder.h"
#include "PlayerController_Battle.h"
#include "EnterTheNetscape_PlayerState.h"


// Sets default values
AActor_AbilitiesLibrary::AActor_AbilitiesLibrary()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AActor_AbilitiesLibrary::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActor_AbilitiesLibrary::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AActor_AbilitiesLibrary::SwitchOnAbilityEffect_Implementation(E_Ability_Functions AbilityFunction, ACharacter_Pathfinder* Avatar, AActor* Target)
{
	switch(AbilityFunction)
	{
		case(E_Ability_Functions::Regenerate):
		{
			const int Healing = FMath::CeilToInt(Avatar->AvatarData.BattleStats.MaximumHealthPoints * 0.15);
			AEnterTheNetscape_PlayerState* PlayerStateReference = Cast<AEnterTheNetscape_PlayerState>(Avatar->PlayerControllerReference->PlayerState);

			PlayerStateReference->Server_AddHealth(Avatar, Healing);
			break;
		}
		default:
		{
			break;
		}
	}
}