#include "Actor_GridTile.h"


#include "Character_Pathfinder.h"
#include "PlayerController_Battle.h"
#include "EnterTheNetscape_GameState.h"


// Sets default values
AActor_GridTile::AActor_GridTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Initialize Components
	// Base Tile
	Floor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor"));
	Floor->SetupAttachment(RootComponent);
	Floor->SetRelativeTransform(FTransform(FRotator(0, 0, 0), FVector::ZeroVector, FVector(1.f, 1.f, 1.f)));

	// Highlight Tile
	TileHighlightPlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileHighlightPlane"));
	TileHighlightPlane->SetupAttachment(Floor);
	TileHighlightPlane->SetRelativeTransform(FTransform(FRotator(0, 0, 0), FVector(0, 0, 1), FVector(1.f, 1.f, 1.f)));

	// Hitbox
	GridTileHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("GridTileHitbox"));
	GridTileHitbox->SetupAttachment(Floor);
	GridTileHitbox->SetRelativeScale3D(FVector(3.f, 1.f, 1.f));
	// Set 'Simulation Generates Hit Events'
	GridTileHitbox->SetNotifyRigidBodyCollision(true);
}

// Called when the game starts or when spawned
void AActor_GridTile::BeginPlay()
{
	Super::BeginPlay();

	UMaterialInstanceDynamic* FloorDynamicMaterial = UMaterialInstanceDynamic::Create(RandomlyChosenMaterialsArray[FMath::RandRange(0, RandomlyChosenMaterialsArray.Num() - 1)], this);
	Floor->SetMaterial(0, FloorDynamicMaterial);

	DynamicMaterial = UMaterialInstanceDynamic::Create(TileHighlightMaterial, this);
	TileHighlightPlane->SetMaterial(0, DynamicMaterial);
	TileHighlightPlane->SetVisibility(false);
}

// Called every frame
void AActor_GridTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


// ------------------------- Battle
void AActor_GridTile::UpdateGridTileState()
{
	TArray<AActor*> OverlappingActors;
	GridTileHitbox->GetOverlappingActors(OverlappingActors, ACharacter_Pathfinder::StaticClass());

	if (this) {
		// Clear TraversalProperties that aren't permanent (e.g. Occupied by an Avatar)
		if (OverlappingActors.Num() <= 0)
			Properties.Remove(E_GridTile_Properties::E_Occupied);
	}

	// If the TraversalProperties array is empty, add the default Property
	if (Properties.Num() <= 0)
		Properties.AddUnique(E_GridTile_Properties::E_None);
}


void AActor_GridTile::OnMouseBeginHover(ACharacter_Pathfinder* CurrentAvatar)
{
	// Set AttackTraceActor to this tile's location if it isn't centered around the player
	if (IsValid(CurrentAvatar)) {
		if (CurrentAvatar->CurrentSelectedAttack.AttachAttackTraceActorToMouse) {
			if (CurrentAvatar->PlayerControllerReference) {
				FVector AttackTraceActorLocation = FVector::ZeroVector;

				if (CurrentAvatar->CurrentSelectedAttack.BaseRange > 1)
					AttackTraceActorLocation = FVector(GetActorLocation().X, GetActorLocation().Y - (200 * CurrentAvatar->CurrentSelectedAttack.BaseRange), CurrentAvatar->AttackTraceActor->GetComponentLocation().Z);
				else
					AttackTraceActorLocation = FVector(GetActorLocation().X, GetActorLocation().Y, CurrentAvatar->AttackTraceActor->GetComponentLocation().Z);
				
				CurrentAvatar->AttackTraceActor->SetWorldLocation(AttackTraceActorLocation, true, nullptr, ETeleportType::ResetPhysics);

				// Update MouseCursor location and AttackTrace location
				CurrentAvatar->PlayerControllerReference->CursorLocationSnappedToGrid = GetActorLocation().GridSnap(200.f);
				CurrentAvatar->ShowAttackRange();
			}
		}
	}
}


void AActor_GridTile::SetTileHighlightProperties(bool IsVisible, bool ShouldChangeColourOnMouseOver, E_GridTile_ColourChangeContext ColourChangeContext)
{
	if (TileHighlightPlane->IsValidLowLevel()) {
		// Set visibility
		TileHighlightPlane->SetVisibility(IsVisible);

		// Set colour changing
		ChangeColourOnMouseHover = ShouldChangeColourOnMouseOver;

		// Change colour
		switch (ColourChangeContext)
		{
		case (E_GridTile_ColourChangeContext::Normal):
			// Heirarcy of colours based on factors such as tile properties
			// Lowest priority: White (no properties that change colour)
			DynamicMaterial->SetVectorParameterValue("Colour", FLinearColor::White);
			// Road (free travel)
			if (Properties.Contains(E_GridTile_Properties::E_StoneRoad))
				DynamicMaterial->SetVectorParameterValue("Colour", FLinearColor(0.5f, 0.2f, 0.f, 1.f));
			// Shadow
			if (Properties.Contains(E_GridTile_Properties::Shadow))
				DynamicMaterial->SetVectorParameterValue("Colour", FLinearColor(0.1f, 0.1f, 0.1f, 1.f));
			// Fire
			if (Properties.Contains(E_GridTile_Properties::Fire))
				DynamicMaterial->SetVectorParameterValue("Colour", FLinearColor(0.6f, 0.2f, 0.f, 1.f));
			// Highest priority: Tile is un-traversable
			if (Properties.Contains(E_GridTile_Properties::E_Wall) ||
				Properties.Contains(E_GridTile_Properties::E_Occupied))
				DynamicMaterial->SetVectorParameterValue("Colour", FLinearColor(0.5f, 0.5f, 0.5f, 1.f));
				break;
		case (E_GridTile_ColourChangeContext::OnMouseHover):
			DynamicMaterial->SetVectorParameterValue("Colour", FLinearColor::Green);
			break;
		case (E_GridTile_ColourChangeContext::OnMouseHoverTileUnreachable):
			DynamicMaterial->SetVectorParameterValue("Colour", FLinearColor::Red);
			break;
		case (E_GridTile_ColourChangeContext::WithinAttackRange):
			DynamicMaterial->SetVectorParameterValue("Colour", FLinearColor(1.f, 0.2f, 0.f, 1.f));
			break;
		default:
			break;
		}
	}
}