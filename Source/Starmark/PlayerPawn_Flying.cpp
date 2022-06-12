#include "PlayerPawn_Flying.h"


// Sets default values
APlayerPawn_Flying::APlayerPawn_Flying()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerPawn_Flying::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerPawn_Flying::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerPawn_Flying::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

