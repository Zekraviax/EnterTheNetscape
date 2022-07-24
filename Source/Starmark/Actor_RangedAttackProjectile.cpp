#include "Actor_RangedAttackProjectile.h"

#include "EnterTheNetscape_PlayerState.h"


// Sets default values
AActor_RangedAttackProjectile::AActor_RangedAttackProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AActor_RangedAttackProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActor_RangedAttackProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// --------------------------------------------------
void AActor_RangedAttackProjectile::AddToLocationCoordinates(float AddToX, float AddToY, float AddToZ)
{
	SetActorLocation(FVector(GetActorLocation().X + AddToX, GetActorLocation().Y + AddToY, GetActorLocation().Z), true);
}


void AActor_RangedAttackProjectile::BeginOverlapEntity(ACharacter_Pathfinder* OverlappingEntity)
{
	if (OverlappingEntity != EntityOwner) {
		int Damage = 2;

		// Inflict damage upon overlapping entity
		Cast<AEnterTheNetscape_PlayerState>(GetWorld()->GetFirstPlayerController()->PlayerState)->Server_SubtractHealth(OverlappingEntity, Damage);

		// End the turn (?)

		// Destroy this

	}
}