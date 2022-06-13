#include "Actor_AttackEffectsLibrary.h"


#include "Actor_GridTile.h"
#include "Character_HatTrick.h"
#include "Actor_StatusEffectsLibrary.h"
#include "Character_Pathfinder.h"
#include "Character_NonAvatarEntity.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AActor_AttackEffectsLibrary::AActor_AttackEffectsLibrary()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AActor_AttackEffectsLibrary::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActor_AttackEffectsLibrary::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Functions
// --------------------------------------------------
void AActor_AttackEffectsLibrary::SwitchOnAttackEffect_Implementation(EBattle_AttackEffects AttackEffect, ACharacter_Pathfinder* Attacker, AActor* Target)
{
	FString ContextString;

	switch (AttackEffect)
	{
	case (EBattle_AttackEffects::AddParalyzeStatus):
		if (Cast<ACharacter_Pathfinder>(Target))
			Attack_AddParalyze(Attacker, Cast<ACharacter_Pathfinder>(Target));
		break;
	case (EBattle_AttackEffects::AddBurnStatus):
		if (Cast<ACharacter_Pathfinder>(Target))
			Attack_AddBurn(Attacker, Cast<ACharacter_Pathfinder>(Target));
		break;
	case (EBattle_AttackEffects::AddDrowningStatus):
		if (Cast<ACharacter_Pathfinder>(Target))
			Attack_AddDrowning(Attacker, Cast<ACharacter_Pathfinder>(Target));
		break;
	case (EBattle_AttackEffects::AddStoneSkinStatus):
		if (Cast<ACharacter_Pathfinder>(Target))
			Attack_AddStoneSkin(Attacker, Cast<ACharacter_Pathfinder>(Target));
		break;
	case (EBattle_AttackEffects::AddBleedStatus):
		if (Cast<ACharacter_Pathfinder>(Target))
			Attack_AddBleed(Attacker, Cast<ACharacter_Pathfinder>(Target));
		break;
	case (EBattle_AttackEffects::AddSoakStatus):
		if (Cast<ACharacter_Pathfinder>(Target))
			Attack_AddSoak(Attacker, Cast<ACharacter_Pathfinder>(Target));
		break;
	case (EBattle_AttackEffects::AddSpellboundStatus):
		if (Cast<ACharacter_Pathfinder>(Target))
			Attack_AddSpellbound(Attacker, Cast<ACharacter_Pathfinder>(Target));
		break;
	case (EBattle_AttackEffects::KnockbackTarget):
		if (Cast<ACharacter_Pathfinder>(Target))
			Attack_KnockbackTarget(Attacker, Cast<ACharacter_Pathfinder>(Target));
		break;
	case (EBattle_AttackEffects::RefundManaPointsPerTargetHit):
		if (Cast<ACharacter_Pathfinder>(Target))
			Attack_RefundMana(Attacker);
		break;
	case (EBattle_AttackEffects::TransferManaPoints):
		if (Cast<ACharacter_Pathfinder>(Target))
			Attack_TransferMana(Attacker, Cast<ACharacter_Pathfinder>(Target));
		break;
	case (EBattle_AttackEffects::SpawnWall):
		if (Cast<AActor_GridTile>(Target))
			Spawn_RockWall(Cast<AActor_GridTile>(Target));
		break;
	case (EBattle_AttackEffects::SpawnHurricane):
		if (Cast<AActor_GridTile>(Target))
			Spawn_Hurricane(Cast<AActor_GridTile>(Target));
		break;
	case (EBattle_AttackEffects::SpawnHats):
		if (Cast<AActor_GridTile>(Target))
			Spawn_Hats(Cast<AActor_GridTile>(Target));
		break;
	case (EBattle_AttackEffects::AddPropertyStoneRoad):
		if (Cast<AActor_GridTile>(Target))
			AddProperty_StoneRoad(Cast<AActor_GridTile>(Target));
		break;
	case (EBattle_AttackEffects::AddPropertyShadow):
		if (Cast<AActor_GridTile>(Target))
			AddProperty_Shadow(Cast<AActor_GridTile>(Target));
		break;
	case (EBattle_AttackEffects::AddPropertyFire):
		if (Cast<AActor_GridTile>(Target))
			AddProperty_Fire(Cast<AActor_GridTile>(Target));
		break;
	default:
		break;
	}
}


void AActor_AttackEffectsLibrary::RepeatingSwitchOnAttackEffect_Implementation(FAvatar_AttackStruct Attack, ACharacter_Pathfinder* Attacker, AActor* Target)
{
	
}


// ------------------------- Status Effect Attacks
void AActor_AttackEffectsLibrary::Attack_AddParalyze_Implementation(ACharacter_Pathfinder* Attacker, ACharacter_Pathfinder* Defender)
{
	const FString ContextString;

	const FAvatar_StatusEffect* ParalyzeStatus = StatusEffectsDataTable->FindRow<FAvatar_StatusEffect>("Paralyzed", ContextString);
	Defender->CurrentStatusEffectsArray.Add(FAvatar_StatusEffect(
		"Paralyzed", 
		ParalyzeStatus->Image, 
		ParalyzeStatus->Description, 
		ParalyzeStatus->MaximumTurns,
		ParalyzeStatus->TurnsRemaining)
	);
}


void AActor_AttackEffectsLibrary::Attack_AddBurn_Implementation(ACharacter_Pathfinder* Attacker, ACharacter_Pathfinder* Defender)
{
	FString ContextString;

	FAvatar_StatusEffect* BurnStatus = StatusEffectsDataTable->FindRow<FAvatar_StatusEffect>("Burned", ContextString);
	Defender->CurrentStatusEffectsArray.Add(FAvatar_StatusEffect(
		"Burned", 
		BurnStatus->Image, 
		BurnStatus->Description,
		BurnStatus->MaximumTurns,
		BurnStatus->TurnsRemaining)
	);
}


void AActor_AttackEffectsLibrary::Attack_AddDrowning_Implementation(ACharacter_Pathfinder* Attacker, ACharacter_Pathfinder* Defender)
{
	FString ContextString;

	FAvatar_StatusEffect* DrowningStatus = StatusEffectsDataTable->FindRow<FAvatar_StatusEffect>("Drowning", ContextString);
	Defender->CurrentStatusEffectsArray.Add(FAvatar_StatusEffect(
		"Drowning", 
		DrowningStatus->Image,
		DrowningStatus->Description,
		DrowningStatus->MaximumTurns,
		DrowningStatus->TurnsRemaining)
	);
}


void AActor_AttackEffectsLibrary::Attack_AddStoneSkin_Implementation(ACharacter_Pathfinder* Attacker, ACharacter_Pathfinder* Defender)
{
	FString ContextString;
	FAvatar_StatusEffect* StoneSkinStatus = StatusEffectsDataTable->FindRow<FAvatar_StatusEffect>("StoneSkin", ContextString);

	if (IsValid(StatusEffectsLibrary_Class)) {
		FActorSpawnParameters SpawnInfo;
		StatusEffectsLibrary_Reference = GetWorld()->SpawnActor<AActor_StatusEffectsLibrary>(StatusEffectsLibrary_Class, FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);
		StatusEffectsLibrary_Reference->OnStatusEffectApplied(Defender, *StoneSkinStatus);
	}
}


void AActor_AttackEffectsLibrary::Attack_AddBleed_Implementation(ACharacter_Pathfinder* Attacker, ACharacter_Pathfinder* Defender)
{
	FString ContextString;
	FAvatar_StatusEffect* BleedStatus = StatusEffectsDataTable->FindRow<FAvatar_StatusEffect>("Bleeding", ContextString);

	if (IsValid(StatusEffectsLibrary_Class)) {
		FActorSpawnParameters SpawnInfo;
		StatusEffectsLibrary_Reference = GetWorld()->SpawnActor<AActor_StatusEffectsLibrary>(StatusEffectsLibrary_Class, FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);
		StatusEffectsLibrary_Reference->OnStatusEffectApplied(Defender, *BleedStatus);
	}
}


void AActor_AttackEffectsLibrary::Attack_AddSoak_Implementation(ACharacter_Pathfinder* Attacker, ACharacter_Pathfinder* Defender)
{
	FString ContextString;
	FAvatar_StatusEffect* SoakStatus = StatusEffectsDataTable->FindRow<FAvatar_StatusEffect>("Soaking", ContextString);

	Defender->CurrentStatusEffectsArray.Add(FAvatar_StatusEffect(
		"Soaking",
		SoakStatus->Image,
		SoakStatus->Description,
		SoakStatus->MaximumTurns,
		SoakStatus->TurnsRemaining)
	);
}


void AActor_AttackEffectsLibrary::Attack_AddSpellbound_Implementation(ACharacter_Pathfinder* Attacker, ACharacter_Pathfinder* Defender)
{
	FString ContextString;
	FAvatar_StatusEffect* SpellboundStatus = StatusEffectsDataTable->FindRow<FAvatar_StatusEffect>("Spellbound", ContextString);

	if (IsValid(StatusEffectsLibrary_Class)) {
		FActorSpawnParameters SpawnInfo;
		StatusEffectsLibrary_Reference = GetWorld()->SpawnActor<AActor_StatusEffectsLibrary>(StatusEffectsLibrary_Class, FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);

		StatusEffectsLibrary_Reference->RememberedAvatarOne = Attacker;
		StatusEffectsLibrary_Reference->RememberedAvatarTwo = Defender;

		StatusEffectsLibrary_Reference->OnStatusEffectApplied(Defender, *SpellboundStatus);
	}
}


// ------------------------- Other
void AActor_AttackEffectsLibrary::Attack_KnockbackTarget_Implementation(ACharacter_Pathfinder* Attacker, ACharacter_Pathfinder* Defender)
{
	FRotator KnockbackDirection = UKismetMathLibrary::FindLookAtRotation(Attacker->GetActorLocation(), Defender->GetActorLocation());
	FVector KnockbackVector = KnockbackDirection.Vector();

	// Multiply the number of tiles to knock the target back by 200
	KnockbackVector.X = KnockbackVector.X * 200;
	KnockbackVector.Y = KnockbackVector.Y * 200;

	KnockbackVector = FVector((KnockbackVector.X + Defender->GetActorLocation().X), (KnockbackVector.Y + Defender->GetActorLocation().Y), Defender->GetActorLocation().Z);

	Defender->SetActorLocation(FVector(KnockbackVector.X * 1, KnockbackVector.Y * 1, Defender->GetActorLocation().Z), true);
}


void AActor_AttackEffectsLibrary::Attack_RefundMana_Implementation(ACharacter_Pathfinder* Attacker)
{
	int RefundAmount = FMath::CeilToInt(Attacker->CurrentSelectedAttack.ManaCost / Attacker->CurrentSelectedAttack.BaseRange);

	Attacker->AvatarData.CurrentManaPoints += RefundAmount;

	if (Attacker->AvatarData.CurrentManaPoints > Attacker->AvatarData.BattleStats.MaximumManaPoints)
		Attacker->AvatarData.CurrentManaPoints = Attacker->AvatarData.BattleStats.MaximumManaPoints;
}


void AActor_AttackEffectsLibrary::Attack_TransferMana_Implementation(ACharacter_Pathfinder* Attacker, ACharacter_Pathfinder* Defender)
{
	int ManaToTransfer = 0;

	ManaToTransfer = FMath::CeilToInt(Attacker->AvatarData.CurrentManaPoints / 2);
	Attacker->AvatarData.CurrentManaPoints -= ManaToTransfer;

	Defender->AvatarData.CurrentManaPoints += ManaToTransfer;
}


// ------------------------- Grid Tile Effects
void AActor_AttackEffectsLibrary::Spawn_RockWall_Implementation(AActor_GridTile* TargetTile)
{
	FVector Location = FVector(TargetTile->GetActorLocation().X, TargetTile->GetActorLocation().Y, TargetTile->GetActorLocation().Z + 50);

	ACharacter_NonAvatarEntity* NewRockWallActor = GetWorld()->SpawnActor<ACharacter_NonAvatarEntity>(RockWall_Class, Location, FRotator::ZeroRotator, SpawnInfo);
	TargetTile->Properties.Add(E_GridTile_Properties::E_Wall);
}


void AActor_AttackEffectsLibrary::Spawn_Hurricane_Implementation(AActor_GridTile* TargetTile)
{
	FVector Location = FVector(TargetTile->GetActorLocation().X, TargetTile->GetActorLocation().Y, TargetTile->GetActorLocation().Z + 50);

	ACharacter_NonAvatarEntity* NewHurricaneActor = GetWorld()->SpawnActor<ACharacter_NonAvatarEntity>(NonAvatarEntity_Class, Location, FRotator::ZeroRotator, SpawnInfo);
	NewHurricaneActor->HurricaneOnSpawn();
}


void AActor_AttackEffectsLibrary::Spawn_Hats_Implementation(AActor_GridTile* TargetTile)
{
	ACharacter_HatTrick* HatTrickActor = nullptr;
	FVector SpawnLocation = FVector::ZeroVector;

	// Spawn three hats
	for (int i = 0; i < HatTilesArray.Num(); i++) {
		SpawnLocation = FVector(HatTilesArray[i]->GetActorLocation().X, HatTilesArray[i]->GetActorLocation().Y, HatTilesArray[i]->GetActorLocation().Z + 100);
		HatTrickActor = GetWorld()->SpawnActor<ACharacter_HatTrick>(HatTrick_Class, SpawnLocation, FRotator::ZeroRotator, SpawnInfo);
	}

	// Make the player choose one hat to hide their avatar in

	Destroy();
}


void AActor_AttackEffectsLibrary::AddProperty_StoneRoad_Implementation(AActor_GridTile* TargetTile)
{
	TargetTile->Properties.Add(E_GridTile_Properties::E_StoneRoad);
}


void AActor_AttackEffectsLibrary::AddProperty_Shadow_Implementation(AActor_GridTile* TargetTile)
{
	TargetTile->Properties.Add(E_GridTile_Properties::Shadow);
}


void AActor_AttackEffectsLibrary::AddProperty_Fire_Implementation(AActor_GridTile* TargetTile)
{
	TargetTile->Properties.Add(E_GridTile_Properties::Fire);
}