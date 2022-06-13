#include "Actor_StatusEffectsLibrary.h"


#include "Character_Pathfinder.h"


// Sets default values
AActor_StatusEffectsLibrary::AActor_StatusEffectsLibrary()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AActor_StatusEffectsLibrary::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActor_StatusEffectsLibrary::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Functions
// --------------------------------------------------
void AActor_StatusEffectsLibrary::OnStatusEffectApplied_Implementation(ACharacter_Pathfinder* AffectedAvatar, FAvatar_StatusEffect StatusEffectReference)
{
	// switch expression of type 'const FString' is illegal
	if (StatusEffectReference.Name == FString("Stone Skin")) {
		RememberedVariableOne = FMath::CeilToInt(AffectedAvatar->AvatarData.BattleStats.Endurance * 1.5);
		AffectedAvatar->AvatarData.BattleStats.Endurance += RememberedVariableOne;

		AffectedAvatar->CurrentStatusEffectsArray.Add(StatusEffectReference);
	} else {
		// Do nothing
	}

	StatusEffectReference.SpecialFunctionsActor = this;
	AffectedAvatar->CurrentStatusEffectsArray.Add(StatusEffectReference);
}


void AActor_StatusEffectsLibrary::OnStatusEffectStartOfTurn_Implementation(ACharacter_Pathfinder* AffectedAvatar, FAvatar_StatusEffect StatusEffectReference)
{
	if (StatusEffectReference.Name == FString("Paralyzed")) {
		AffectedAvatar->AvatarData.CurrentTileMoves = FMath::CeilToInt(AffectedAvatar->AvatarData.MaximumTileMoves / 2);
	} else if (StatusEffectReference.Name == FString("Bleeding")) {
		AffectedAvatar->AvatarData.CurrentHealthPoints -= FMath::CeilToInt(AffectedAvatar->AvatarData.BattleStats.MaximumHealthPoints * 0.2);
	} else if (StatusEffectReference.Name == FString("Spellbound")) {
		int HealthToTransfer = HealthToTransfer = FMath::CeilToInt(RememberedAvatarTwo->AvatarData.BattleStats.MaximumHealthPoints * 0.2);
		int ManaToTransfer = ManaToTransfer = FMath::CeilToInt(RememberedAvatarTwo->AvatarData.BattleStats.MaximumManaPoints * 0.2);
		
		RememberedAvatarTwo->AvatarData.CurrentHealthPoints -= HealthToTransfer;
		RememberedAvatarTwo->AvatarData.CurrentManaPoints -= ManaToTransfer;

		RememberedAvatarOne->AvatarData.CurrentHealthPoints += HealthToTransfer;
		if (RememberedAvatarOne->AvatarData.CurrentHealthPoints > RememberedAvatarOne->AvatarData.BattleStats.MaximumHealthPoints)
			RememberedAvatarOne->AvatarData.CurrentHealthPoints = RememberedAvatarOne->AvatarData.BattleStats.MaximumHealthPoints;

		RememberedAvatarOne->AvatarData.CurrentManaPoints += ManaToTransfer;
		if (RememberedAvatarOne->AvatarData.CurrentManaPoints > RememberedAvatarOne->AvatarData.BattleStats.MaximumManaPoints)
			RememberedAvatarOne->AvatarData.CurrentManaPoints = RememberedAvatarOne->AvatarData.BattleStats.MaximumManaPoints;
	}
	else {
		// Do nothing
	}
}


void AActor_StatusEffectsLibrary::OnStatusEffectRemoved_Implementation(ACharacter_Pathfinder* AffectedAvatar, FAvatar_StatusEffect StatusEffectReference)
{
	if (StatusEffectReference.Name == FString("Stone Skin")) {
		AffectedAvatar->AvatarData.BattleStats.Endurance -= RememberedVariableOne;
	} else {
		// Do nothing
	}

	StatusEffectReference.SpecialFunctionsActor->Destroy();

	AffectedAvatar->CurrentStatusEffectsArray.Remove(StatusEffectReference);
}