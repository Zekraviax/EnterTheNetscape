#include "Character_NonAvatarEntity.h"


#include "Actor_GridTile.h"
#include "AIController_Avatar.h"
#include "Character_Pathfinder.h"
#include "Kismet/KismetMathLibrary.h"


ACharacter_NonAvatarEntity::ACharacter_NonAvatarEntity()
{
	// Spawn 'Mesh'
	CubeComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube Component"));
	CubeComponent->AttachTo(RootComponent);
}


// ------------------------- Hurricane
void ACharacter_NonAvatarEntity::HurricaneOnSpawn()
{
	// Destroy unneeded components
	ActorSelectedPlane->DestroyComponent();
	ActorHighlightedDecal->DestroyComponent();
	AttackTraceActor->DestroyComponent();
	AvatarBattleData_Component->DestroyComponent();
	GetMesh()->DestroyComponent();

	// Set the Hitbox to overlap all actors
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	EntityType = E_NonAvatarEntity_EntityType::Hurricane;

	// Draw in all avatars one tile towards the center of the hurricane
	FRotator KnockbackDirection = FRotator::ZeroRotator;
	FVector KnockbackVector = FVector::ZeroVector;
	TArray<AActor*> Avatars;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter_Pathfinder::StaticClass(), Avatars);

	for (int i = 0; i < Avatars.Num(); i++) {
		if (!Cast<ACharacter_NonAvatarEntity>(Avatars[i])) {
			ACharacter_Pathfinder* Avatar = Cast<ACharacter_Pathfinder>(Avatars[i]);
			KnockbackDirection = UKismetMathLibrary::FindLookAtRotation(Avatar->GetActorLocation(), GetActorLocation());
			KnockbackVector = KnockbackDirection.Vector();

			// Knockback distance = 125 * desired number of tiles
			KnockbackVector.X = KnockbackVector.X * 125;
			KnockbackVector.Y = KnockbackVector.Y * 125;
			KnockbackVector = FVector((KnockbackVector.X + Avatar->GetActorLocation().X), (KnockbackVector.Y + Avatar->GetActorLocation().Y), Avatar->GetActorLocation().Z);
			KnockbackVector = KnockbackVector.GridSnap(200.f);

			Avatar->SetActorLocation(FVector(KnockbackVector.X, KnockbackVector.Y, Avatar->GetActorLocation().Z), true);
		}
	}
}


void ACharacter_NonAvatarEntity::HurricaneOnAvatarCollision(ACharacter_Pathfinder* Avatar)
{
	// Get all grid tiles that aren't occupied
	TArray<AActor*> GridTilesArray;
	AActor_GridTile* LaunchLocation = nullptr;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor_GridTile::StaticClass(), GridTilesArray);

	// Discard all tiles that are occupied or un-traversable
	for (int i = GridTilesArray.Num() - 1; i >= 0; i--) {
		AActor_GridTile* FoundTile = Cast<AActor_GridTile>(GridTilesArray[i]);

		if (FoundTile->Properties.Contains(E_GridTile_Properties::E_Occupied) ||
			FoundTile->Properties.Contains(E_GridTile_Properties::E_Wall)) {
			GridTilesArray.RemoveAt(i);
		} else if (FoundTile->GetActorLocation().X == GetActorLocation().X &&
			FoundTile->GetActorLocation().Y == GetActorLocation().Y) {
			GridTilesArray.RemoveAt(i);
		}
	}

	if (GridTilesArray.Num() >= 1) {
		LaunchLocation = Cast<AActor_GridTile>(GridTilesArray[FMath::RandRange(0, (GridTilesArray.Num() - 1))]);
		FVector LaunchVelocity = FVector::ZeroVector;

		Avatar->SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, Avatar->GetActorLocation().Z));

		LaunchVelocity = LaunchLocation->GetActorLocation() - Avatar->GetActorLocation().GridSnap(200.f);
		LaunchVelocity.Z = 400.f;

		Cast<AAIController_Avatar>(Avatar->GetController())->StopMovement();

		Avatar->LaunchCharacter(LaunchVelocity, false, true);
		Avatar->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	}
}