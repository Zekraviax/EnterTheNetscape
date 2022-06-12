#include "PlayerPawn_Static.h"


// Sets default values
APlayerPawn_Static::APlayerPawn_Static()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerPawn_Static::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerPawn_Static::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerPawn_Static::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}