#include "Actor_MorphTargetTest.h"


// Sets default values
AActor_MorphTargetTest::AActor_MorphTargetTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AActor_MorphTargetTest::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AActor_MorphTargetTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}