#include "AIController_EnemyEntity.h"


#include "Character_Pathfinder.h"


void AAIController_EnemyEntity::StepOne_ChooseTarget()
{
	TArray<AActor*> Entities;
	TArray<ACharacter_Pathfinder*> ValidTargets;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter_Pathfinder::StaticClass(), Entities);
	for (AActor* Actor: Entities) {
		ACharacter_Pathfinder* Entity = Cast<ACharacter_Pathfinder>(Actor);
		if (Entity->AvatarData.Factions.Contains(EEntity_Factions::Player1)) {
			ValidTargets.Add(Entity);
		}
	}

	CurrentTarget = ValidTargets[FMath::RandRange(0, ValidTargets.Num() - 1)];

	this->StepTwo_MoveToTarget();
}


void AAIController_EnemyEntity::StepFour_LaunchAttack()
{

}