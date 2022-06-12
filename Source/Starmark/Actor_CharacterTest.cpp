#include "Actor_CharacterTest.h"


#include "PlayerController_Battle.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AActor_CharacterTest::AActor_CharacterTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create Static Mesh Component
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AActor_CharacterTest::BeginPlay()
{
	Super::BeginPlay();

	// Dynamic Material
	DynamicMaterial = UMaterialInstanceDynamic::Create(StaticMaterial, this);
}

// Called every frame
void AActor_CharacterTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}