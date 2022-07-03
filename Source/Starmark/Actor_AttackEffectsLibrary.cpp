#include "Actor_AttackEffectsLibrary.h"


#include "Actor_GridTile.h"
#include "Actor_StatusEffectsLibrary.h"
#include "Character_Pathfinder.h"
#include "Character_NonAvatarEntity.h"
#include "EnterTheNetscape_PlayerState.h"
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
			Chirp_Scratch(Attacker, Cast<ACharacter_Pathfinder>(Target));
		break;
	case (EBattle_AttackEffects::Spirit_Cut):
		if (Cast<ACharacter_Pathfinder>(Target))
			Spirit_Cut(Attacker, Cast<ACharacter_Pathfinder>(Target));
		break;
	case (EBattle_AttackEffects::Spirit_DashAttack):
		Spirit_DashAttack(Attacker, Target);
		break;
	case (EBattle_AttackEffects::Sugar_Bash):
		if (Cast<ACharacter_Pathfinder>(Target))
			Sugar_Bash(Attacker, Cast<ACharacter_Pathfinder>(Target));
		break;
	case (EBattle_AttackEffects::Sugar_Concuss):
		if (Cast<ACharacter_Pathfinder>(Target))
			Sugar_Concuss(Attacker, Cast<ACharacter_Pathfinder>(Target));
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
		FActorSpawnParameters SpawnInfo;
		StatusEffectsLibrary_Reference = GetWorld()->SpawnActor<AActor_StatusEffectsLibrary>(StatusEffectsLibrary_Class, FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);
		StatusEffectsLibrary_Reference->OnStatusEffectApplied(Defender, *BleedStatus);
	}
}


void AActor_AttackEffectsLibrary::Chirp_Peck_Implementation(ACharacter_Pathfinder* Attacker, ACharacter_Pathfinder* Defender)
{
	int Damage = Attacker->AvatarData.BattleStats.Strength + Attacker->AvatarData.BattleStats.Agility;
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