#include "Actor_AttackEffectsLibrary.h"


#include "Actor_GridTile.h"
#include "Actor_RangedAttackProjectile.h"
#include "Actor_StatusEffectsLibrary.h"
#include "Character_Pathfinder.h"
#include "Character_NonAvatarEntity.h"
#include "EnterTheNetscape_PlayerState.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AActor_AttackEffectsLibrary::AActor_AttackEffectsLibrary()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

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
	case (EBattle_AttackEffects::Jasper_Bonk):
		if (Cast<ACharacter_Pathfinder>(Target))
			Jasper_Bonk(Cast<ACharacter_Pathfinder>(Target));
			break;
	case (EBattle_AttackEffects::Chirp_Scratch):
		if (Cast<ACharacter_Pathfinder>(Target))
			Chirp_Scratch(Attacker, Cast<ACharacter_Pathfinder>(Target));
		break;
	case (EBattle_AttackEffects::Chirp_Peck):
		if (Cast<ACharacter_Pathfinder>(Target))
			Chirp_Peck(Attacker, Cast<ACharacter_Pathfinder>(Target));
		break;
	case (EBattle_AttackEffects::Chirp_Swoop):
		Chirp_Swoop(Attacker, Target);
		break;
	case (EBattle_AttackEffects::Chirp_Backstab):
		Chirp_Backstab(Attacker, Cast<ACharacter_Pathfinder>(Target));
		break;
	case (EBattle_AttackEffects::Spirit_Cut):
		if (Cast<ACharacter_Pathfinder>(Target))
			Spirit_Cut(Attacker, Cast<ACharacter_Pathfinder>(Target));
		break;
	case (EBattle_AttackEffects::Spirit_DashAttack):
		Spirit_DashAttack(Attacker, Target);
		break;
	case (EBattle_AttackEffects::Spirit_Blunderbuss):
		Spirit_Blunderbuss(Attacker, Target);
		break;
	case (EBattle_AttackEffects::Spirit_CrescentSlash):
		Spirit_CrescentSlash(Attacker, Target);
		break;
	case (EBattle_AttackEffects::Sugar_Bash):
		if (Cast<ACharacter_Pathfinder>(Target))
			Sugar_Bash(Attacker, Cast<ACharacter_Pathfinder>(Target));
		break;
	case (EBattle_AttackEffects::Sugar_Concuss):
		if (Cast<ACharacter_Pathfinder>(Target))
			Sugar_Concuss(Attacker, Cast<ACharacter_Pathfinder>(Target));
		break;
	case (EBattle_AttackEffects::Sugar_Sting):
		if (Cast<ACharacter_Pathfinder>(Target))
			Sugar_Sting(Attacker, Cast<ACharacter_Pathfinder>(Target));
		break;
	case (EBattle_AttackEffects::Sugar_HoneyBolt):
		if (Cast<ACharacter_Pathfinder>(Target))
			Sugar_Sting(Attacker, Cast<ACharacter_Pathfinder>(Target));
		break;
	default:
		break;
	}
}


// -------------------------------------------------- Jasper
void AActor_AttackEffectsLibrary::Jasper_Bonk_Implementation(ACharacter_Pathfinder* Defender)
{
	Cast<AEnterTheNetscape_PlayerState>(Defender->GetPlayerState())->Server_SubtractHealth(Defender, 1);
}


// -------------------------------------------------- Chirp
void AActor_AttackEffectsLibrary::Chirp_Scratch_Implementation(ACharacter_Pathfinder* Attacker, ACharacter_Pathfinder* Defender)
{
	int Damage = Attacker->AvatarData.BattleStats.Strength + 1;
	Cast<AEnterTheNetscape_PlayerState>(GetWorld()->GetFirstPlayerController()->PlayerState)->Server_SubtractHealth(Defender, Damage);

	FAvatar_StatusEffect* BleedStatus = StatusEffectsDataTable->FindRow<FAvatar_StatusEffect>("Bleeding", AttackEffectsLibraryContextString);
	if (IsValid(StatusEffectsLibrary_Class)) {
		StatusEffectsLibrary_Reference = GetWorld()->SpawnActor<AActor_StatusEffectsLibrary>(StatusEffectsLibrary_Class, FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);
		StatusEffectsLibrary_Reference->OnStatusEffectApplied(Defender, *BleedStatus);
	}
}


void AActor_AttackEffectsLibrary::Chirp_Peck_Implementation(ACharacter_Pathfinder* Attacker, ACharacter_Pathfinder* Defender)
{
	int Damage = Attacker->AvatarData.BattleStats.Strength + Attacker->AvatarData.BattleStats.Agility;
	Cast<AEnterTheNetscape_PlayerState>(GetWorld()->GetFirstPlayerController()->PlayerState)->Server_SubtractHealth(Defender, Damage);
}


void AActor_AttackEffectsLibrary::Chirp_Swoop_Implementation(ACharacter_Pathfinder* Attacker, AActor* Target)
{
	if (Cast<ACharacter_Pathfinder>(Target)) {
		int Damage = Attacker->AvatarData.BattleStats.Strength;
		Cast<AEnterTheNetscape_PlayerState>(GetWorld()->GetFirstPlayerController()->PlayerState)->Server_SubtractHealth(Cast<ACharacter_Pathfinder>(Target), Damage);
	} else if (Cast<AActor_GridTile>(Target)) {
		Attacker->SetActorLocation(Target->GetActorLocation());
	}
}


void AActor_AttackEffectsLibrary::Chirp_Backstab_Implementation(ACharacter_Pathfinder* Attacker, ACharacter_Pathfinder* Defender)
{
	int Damage = Attacker->AvatarData.BattleStats.Strength + 1;
	Cast<AEnterTheNetscape_PlayerState>(GetWorld()->GetFirstPlayerController()->PlayerState)->Server_SubtractHealth(Defender, Damage);
}


// -------------------------------------------------- Spirit
void AActor_AttackEffectsLibrary::Spirit_Cut_Implementation(ACharacter_Pathfinder* Attacker, ACharacter_Pathfinder* Defender)
{

}


void AActor_AttackEffectsLibrary::Spirit_DashAttack_Implementation(ACharacter_Pathfinder* Attacker, AActor* Target)
{
	if (Cast<ACharacter_Pathfinder>(Target)) {
		int Damage = Attacker->AvatarData.BattleStats.Agility;
		Cast<AEnterTheNetscape_PlayerState>(GetWorld()->GetFirstPlayerController()->PlayerState)->Server_SubtractHealth(Cast<ACharacter_Pathfinder>(Target), Damage);
	} else if (Cast<AActor_GridTile>(Target)) {
		Attacker->SetActorLocation(Target->GetActorLocation());
	}
}


void AActor_AttackEffectsLibrary::Spirit_Blunderbuss_Implementation(ACharacter_Pathfinder* Attacker, AActor* Target)
{
	// Spawn projectile
	if (!RangedProjectileActor_Reference->IsValidLowLevel() && RangedProjectileActor_Class) {
		RangedProjectileActor_Reference = GetWorld()->SpawnActor<AActor_RangedAttackProjectile>(RangedProjectileActor_Class, Attacker->GetActorLocation(), Attacker->GetActorRotation(), SpawnInfo);
	}

	// Set owner
	RangedProjectileActor_Reference->EntityOwner = Attacker;

	// Launch projectile
	float XValue = 0;
	float YValue = 0;
	float ZValue = 0;

	//UE_LOG(LogTemp, Warning, TEXT("Spirit_Blunderbuss_Implementation / Actor rotation is: %f %f %f"), Attacker->GetActorRotation().Roll, GetActorRotation().Pitch, Attacker->GetActorRotation().Yaw);
	UE_LOG(LogTemp, Warning, TEXT("Spirit_Blunderbuss_Implementation / Actor forward vector is: %f %f %f"), Attacker->GetActorForwardVector().X, Attacker->GetActorForwardVector().Y, Attacker->GetActorForwardVector().Z);

	if (Attacker->GetActorForwardVector().X >= 0.1f) {
		XValue = 10.f;
	} else if (Attacker->GetActorForwardVector().X <= -0.1f) {
		XValue = -10.f;
	}

	if (Attacker->GetActorForwardVector().Y >= 0.1f) {
		YValue = 10.f;
	} else if (Attacker->GetActorForwardVector().Y <= -0.1f) {
		YValue = -10.f;
	}

	ZValue = Attacker->GetActorForwardVector().Z;

	UE_LOG(LogTemp, Warning, TEXT("Spirit_Blunderbuss_Implementation / projectile velocity is: %f %f %f"), XValue, YValue, ZValue);
	RangedProjectileActor_Reference->VelocityEachTick = FVector(XValue, YValue, ZValue);

	// just deal damage to each entity in range immediately
	if (Cast<ACharacter_Pathfinder>(Target)) {
		int Damage = Attacker->AvatarData.BattleStats.Strength + 2;
		Cast<AEnterTheNetscape_PlayerState>(GetWorld()->GetFirstPlayerController()->PlayerState)->Server_SubtractHealth(Cast<ACharacter_Pathfinder>(Target), Damage);
	}
}


void AActor_AttackEffectsLibrary::Spirit_CrescentSlash_Implementation(ACharacter_Pathfinder* Attacker, AActor* Target)
{
	int Damage = Attacker->AvatarData.BattleStats.Strength;
	Cast<AEnterTheNetscape_PlayerState>(GetWorld()->GetFirstPlayerController()->PlayerState)->Server_SubtractHealth(Cast<ACharacter_Pathfinder>(Target), Damage);
}


// -------------------------------------------------- Sugar
void AActor_AttackEffectsLibrary::Sugar_Bash_Implementation(ACharacter_Pathfinder* Attacker, ACharacter_Pathfinder* Defender)
{
	int Damage = Attacker->AvatarData.BattleStats.Strength + 2;
	Cast<AEnterTheNetscape_PlayerState>(GetWorld()->GetFirstPlayerController()->PlayerState)->Server_SubtractHealth(Defender, Damage);
}


void AActor_AttackEffectsLibrary::Sugar_Concuss_Implementation(ACharacter_Pathfinder* Attacker, ACharacter_Pathfinder* Defender)
{
	int Damage = Attacker->AvatarData.BattleStats.Strength;
	Cast<AEnterTheNetscape_PlayerState>(GetWorld()->GetFirstPlayerController()->PlayerState)->Server_SubtractHealth(Defender, Damage);

	FAvatar_StatusEffect* StunnedStatus = StatusEffectsDataTable->FindRow<FAvatar_StatusEffect>("Stunned", AttackEffectsLibraryContextString);
	Defender->CurrentStatusEffectsArray.Add(FAvatar_StatusEffect(
		"Stunned",
		StunnedStatus->Image,
		StunnedStatus->Description,
		StunnedStatus->MaximumTurns,
		StunnedStatus->TurnsRemaining)
	);
}


void AActor_AttackEffectsLibrary::Sugar_Sting_Implementation(ACharacter_Pathfinder* Attacker, ACharacter_Pathfinder* Defender)
{
	int Damage = Attacker->AvatarData.BattleStats.Strength + 2;
	Cast<AEnterTheNetscape_PlayerState>(GetWorld()->GetFirstPlayerController()->PlayerState)->Server_SubtractHealth(Defender, Damage);
}


void AActor_AttackEffectsLibrary::Sugar_HoneyBolt_Implementation(ACharacter_Pathfinder* Attacker, AActor* Target)
{
	// Spawn projectile
	if (!RangedProjectileActor_Reference->IsValidLowLevel() && RangedProjectileActor_Class) {
		RangedProjectileActor_Reference = GetWorld()->SpawnActor<AActor_RangedAttackProjectile>(RangedProjectileActor_Class, Attacker->GetActorLocation(), Attacker->GetActorRotation(), SpawnInfo);
	}

	// Set owner
	RangedProjectileActor_Reference->EntityOwner = Attacker;

	// Launch projectile
	float XValue = 0;
	float YValue = 0;
	float ZValue = 0;

	//UE_LOG(LogTemp, Warning, TEXT("Sugar_HoneyBolt_Implementation / Actor rotation is: %f %f %f"), Attacker->GetActorRotation().Roll, GetActorRotation().Pitch, Attacker->GetActorRotation().Yaw);
	UE_LOG(LogTemp, Warning, TEXT("Sugar_HoneyBolt_Implementation / Actor forward vector is: %f %f %f"), Attacker->GetActorForwardVector().X, Attacker->GetActorForwardVector().Y, Attacker->GetActorForwardVector().Z);

	if (Attacker->GetActorForwardVector().X >= 0.1f) {
		XValue = 10.f;
	}
	else if (Attacker->GetActorForwardVector().X <= -0.1f) {
		XValue = -10.f;
	}

	if (Attacker->GetActorForwardVector().Y >= 0.1f) {
		YValue = 10.f;
	}
	else if (Attacker->GetActorForwardVector().Y <= -0.1f) {
		YValue = -10.f;
	}

	ZValue = Attacker->GetActorForwardVector().Z;

	UE_LOG(LogTemp, Warning, TEXT("Sugar_HoneyBolt_Implementation / projectile velocity is: %f %f %f"), XValue, YValue, ZValue);
	RangedProjectileActor_Reference->VelocityEachTick = FVector(XValue, YValue, ZValue);

	// just deal damage to each entity in range immediately
	if (Cast<ACharacter_Pathfinder>(Target)) {
		int Damage = Attacker->AvatarData.BattleStats.Strength;
		Cast<AEnterTheNetscape_PlayerState>(GetWorld()->GetFirstPlayerController()->PlayerState)->Server_SubtractHealth(Cast<ACharacter_Pathfinder>(Target), Damage);
	}
}