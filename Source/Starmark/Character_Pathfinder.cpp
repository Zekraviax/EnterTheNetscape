#include "Character_Pathfinder.h"

#include "Actor_GridTile.h"
#include "AIController_Avatar.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/EngineTypes.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerController_Battle.h"
#include "Widget_HUD_Battle.h"
#include "WidgetComponent_AvatarBattleData.h"


ACharacter_Pathfinder::ACharacter_Pathfinder()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 69.0f);

	// Don't rotate character to camera
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to face direction they move
	GetCharacterMovement()->RotationRate = FRotator(0.f, 1000.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Actor Selected Decal (Don't delete this)
	ActorSelected = CreateDefaultSubobject<UDecalComponent>("ActorSelected");
	ActorSelected->SetupAttachment(RootComponent);
	ActorSelected->SetVisibility(false);
	ActorSelected->SetHiddenInGame(true);
	ActorSelected->DecalSize = FVector(32.0f, 64.0f, 64.0f);
	ActorSelected->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
	ActorSelected->SetRelativeLocation(FVector(0.f, 0.f, -90.f));

	// Actor Selected Plane
	ActorSelectedPlane = CreateDefaultSubobject<UStaticMeshComponent>("ActorSelectedPlane");
	ActorSelectedPlane->SetupAttachment(RootComponent);
	ActorSelectedPlane->SetVisibility(true);
	ActorSelectedPlane->SetHiddenInGame(false);

	// AvatarBattleData WidgetComponent
	AvatarBattleData_Component = CreateDefaultSubobject<UWidgetComponent>("AvatarBattleData_Component");
	AvatarBattleData_Component->SetupAttachment(RootComponent);
	AvatarBattleData_Component->SetVisibility(true);
	AvatarBattleData_Component->SetHiddenInGame(true);

	// Attack Trace Actor Component
	AttackTraceActor = CreateDefaultSubobject<UStaticMeshComponent>("AttackTraceActor");
	AttackTraceActor->SetupAttachment(RootComponent);
	AttackTraceActor->SetVisibility(true);
	AttackTraceActor->SetHiddenInGame(false);

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	//Hitbox Component
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetVisibility(true);
	BoxComponent->SetHiddenInGame(false);
	BoxComponent->SetRelativeScale3D(FVector(1.5f, 1.5f, 3.f));
	// Set 'Simulation Generates Hit Events'
	BoxComponent->SetNotifyRigidBodyCollision(true);

	// Multiplayer
	bReplicates = true; 
	//bReplicateMovement = true; 
	bNetUseOwnerRelevancy = true;
	IndexInPlayerParty = -1;
	//ActorSelected->SetIsReplicated(true);
	ActorSelectedPlane->SetIsReplicated(true);
}


// Replicated variables
void ACharacter_Pathfinder::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps); 

	DOREPLIFETIME(ACharacter_Pathfinder, AvatarData);
	DOREPLIFETIME(ACharacter_Pathfinder, PlayerControllerReference);
	DOREPLIFETIME(ACharacter_Pathfinder, CurrentKnownAttacks);
	DOREPLIFETIME(ACharacter_Pathfinder, CurrentSelectedAttack);
	DOREPLIFETIME(ACharacter_Pathfinder, IndexInPlayerParty);
}


void ACharacter_Pathfinder::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}


// ------------------------- Base
void ACharacter_Pathfinder::BeginPlayWorkaroundFunction_Implementation(UWidget_HUD_Battle* BattleHUDReference)
{
	FString ContextString;

	// Snap Actor to Grid
	// The Z Value needs to be retained or else the character will probably clip through the floor
	FVector ActorLocationSnappedToGrid = GetActorLocation().GridSnap(200.f);
	ActorLocationSnappedToGrid.Z = GetActorLocation().Z;
	SetActorLocation(ActorLocationSnappedToGrid);

	AvatarData.CurrentHealthPoints = AvatarData.BattleStats.MaximumHealthPoints;
	AvatarData.CurrentManaPoints = AvatarData.BattleStats.MaximumManaPoints;
	AvatarData.CurrentTileMoves = AvatarData.MaximumTileMoves;

	// Set default selected attack
	if (CurrentKnownAttacks.Num() > 0)
		CurrentSelectedAttack = CurrentKnownAttacks[0];

	IndexInPlayerParty = 0;
}


// ------------------------- Cursor
void ACharacter_Pathfinder::OnAvatarCursorOverBegin()
{
	UE_LOG(LogTemp, Warning, TEXT("OnAvatarCursorOverBegin / Cursor over begin on actor: %s"), *GetName());

	if (ActorSelectedPlane)
		ActorSelectedPlane->SetWorldLocation(FVector(this->GetActorLocation().X, this->GetActorLocation().Y, 1));

	if (!AvatarBattleDataComponent_Reference)
		AvatarBattleDataComponent_Reference = Cast<UWidgetComponent_AvatarBattleData>(AvatarBattleData_Component->GetUserWidgetObject());

	AvatarBattleDataComponent_Reference->LinkedAvatar = AvatarData;
	AvatarBattleDataComponent_Reference->UpdateAvatarData(AvatarData);

	AvatarBattleData_Component->SetHiddenInGame(false);
}


void ACharacter_Pathfinder::OnAvatarCursorOverEnd()
{
	AvatarBattleData_Component->SetHiddenInGame(true);
}


void ACharacter_Pathfinder::OnAvatarClicked()
{
	if (!PlayerControllerReference)
		PlayerControllerReference = Cast<APlayerController_Battle>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	PlayerControllerReference->CurrentSelectedAvatar = this;

	if (ActorSelectedPlane) {
		ActorSelectedPlane->SetWorldLocation(FVector(this->GetActorLocation().X, this->GetActorLocation().Y, 1));

		for (TObjectIterator<ACharacter_Pathfinder> Itr; Itr; ++Itr) {
			ACharacter_Pathfinder* FoundActor = *Itr;

			if (PlayerControllerReference->CurrentSelectedAvatar != FoundActor)
				FoundActor->ActorSelectedPlane->SetHiddenInGame(false);
		}
	}
}


void ACharacter_Pathfinder::SetAttackTraceActorLocationSnappedToGrid()
{
	// Get all tiles and overlapping tiles
	TArray<AActor*> GridTilesArray;
	TArray<UPrimitiveComponent*> OverlappingActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor_GridTile::StaticClass(), GridTilesArray);

	// Check for tiles that overlap the AttackTraceActor
	for (int i = 0; i < GridTilesArray.Num(); i++) {
		AActor_GridTile* GridTile = Cast<AActor_GridTile>(GridTilesArray[i]);

		GridTile->GetOverlappingComponents(OverlappingActors);
		if (OverlappingActors.Contains(AttackTraceActor)) {
			GridTile->ChangeColourOnMouseHover = false;
			GridTile->DynamicMaterial->SetVectorParameterValue(FName("Colour"), FLinearColor(1.f, 0.2f, 0.0f));
		} else {
			GridTile->ChangeColourOnMouseHover = true;
			GridTile->DynamicMaterial->SetVectorParameterValue(FName("Colour"), FLinearColor(1.f, 1.f, 1.f));
		}
	}
}

// ------------------------- Battle
void ACharacter_Pathfinder::ShowAttackRange()
{
	AttackTraceActor->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
	AttackTraceActor->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AttackTraceActor->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	// Center the attack component on a single tile
	if (CurrentSelectedAttack.AttackTargetsInRange == EBattle_AttackTargetsInRange::Self ||
		CurrentSelectedAttack.AttackPattern == EBattle_AttackPatterns::SingleTile) {
		AttackTraceActor->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AttackTraceActor->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

		FVector OriginCoordinates = FVector::ZeroVector;
		ValidAttackTargetsArray.Empty();

		// Get the current mouse/avatar position
		if (CurrentSelectedAttack.AttachAttackTraceActorToMouse)
			OriginCoordinates = PlayerControllerReference->CursorLocationSnappedToGrid;
		else
			OriginCoordinates = GetActorLocation().GridSnap(200.f);

		// Get all of the tiles and avatars around the origin coordinates
		TArray<AActor*> GridTilesArray, AvatarsArray, ActorsArray;

		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor_GridTile::StaticClass(), GridTilesArray);
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter_Pathfinder::StaticClass(), AvatarsArray);

		ActorsArray.Append(GridTilesArray);
		ActorsArray.Append(AvatarsArray);

		for (int i = 0; i < ActorsArray.Num(); i++) {
			if (ActorsArray[i]->GetActorLocation().Equals(OriginCoordinates, 100.f)) {
				if (Cast<AActor_GridTile>(ActorsArray[i])) {
					Cast<AActor_GridTile>(ActorsArray[i])->ChangeColourOnMouseHover = false;
					Cast<AActor_GridTile>(ActorsArray[i])->UpdateTileColour(E_GridTile_ColourChangeContext::WithinAttackRange);

					if (CurrentSelectedAttack.AttackPattern == EBattle_AttackPatterns::SingleTile)
						ValidAttackTargetsArray.Add(ActorsArray[i]);
				} else if (Cast<ACharacter_Pathfinder>(ActorsArray[i])) {
					ValidAttackTargetsArray.Add(ActorsArray[i]);
				}
			} else {
				if (Cast<AActor_GridTile>(ActorsArray[i])) {
					Cast<AActor_GridTile>(ActorsArray[i])->ChangeColourOnMouseHover = true;
					Cast<AActor_GridTile>(ActorsArray[i])->UpdateTileColour(E_GridTile_ColourChangeContext::Normal);
				}
			}
		}
	} else {
		// Circle/Sphere Trace
		if (CurrentSelectedAttack.AttackPattern == EBattle_AttackPatterns::Circle ||
			CurrentSelectedAttack.AttackPattern == EBattle_AttackPatterns::AOE_Circle) {
			// Set the StaticMesh
			for (int i = 0; i < AttackTraceStaticMeshes.Num(); i++) {
				if (AttackTraceStaticMeshes[i]->GetName().Contains("Sphere")) {
					AttackTraceActor->SetStaticMesh(AttackTraceStaticMeshes[i]);
					break;
				}
			}

			if (AttackRotationSnapToDegrees != 90)
				AttackRotationSnapToDegrees = 90;
			
			int DefaultSphereScale = 2 * CurrentSelectedAttack.BaseRange;		// Add 2 for every tile range

			FVector SphereLocation = FVector(-200 * CurrentSelectedAttack.BaseRange, 0, -100);
			FRotator SphereRotation = FRotator(0, 0, 0);
			FVector SphereScale = FVector(DefaultSphereScale, DefaultSphereScale, DefaultSphereScale);

			AttackTraceActor->SetRelativeLocation(SphereLocation);
			AttackTraceActor->SetRelativeScale3D(SphereScale);
		}
		// Cone Trace
		else if (CurrentSelectedAttack.AttackPattern == EBattle_AttackPatterns::Cone) {
			// Set the StaticMesh
			for (int i = 0; i < AttackTraceStaticMeshes.Num(); i++) {
				if (AttackTraceStaticMeshes[i]->GetName().Contains("Cone")) {
					AttackTraceActor->SetStaticMesh(AttackTraceStaticMeshes[i]);
					break;
				}
			}

			if (AttackRotationSnapToDegrees != 90)
				AttackRotationSnapToDegrees = 90;

			// Set the relative location and scale through maths
			int DefaultConeLocationX = 300; // Add 190 for every tile range the cone should reach
			int DefaultConeScaleX = 1;		// Add 1 for every tile range
			int DefaultConeScaleY = 0;		// Add 2 for every tile range
			int DefaultConeScaleZ = 0;		// Add 1 for every tile range

			FVector ConeLocation = FVector(DefaultConeLocationX + (CurrentSelectedAttack.BaseRange * 190), 0, -100);
			FVector ConeScale = FVector(DefaultConeScaleX + CurrentSelectedAttack.BaseRange, DefaultConeScaleY + CurrentSelectedAttack.BaseRange * 2, DefaultConeScaleZ + CurrentSelectedAttack.BaseRange);

			AttackTraceActor->SetRelativeLocation(ConeLocation);
			AttackTraceActor->SetRelativeScale3D(ConeScale);
		}
		// Ring attack pattern
		else if (CurrentSelectedAttack.AttackPattern == EBattle_AttackPatterns::Ring) {
			AttackTraceActor->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			AttackTraceActor->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

			FVector OriginCoordinates = FVector::ZeroVector;

			// Get the current mouse/avatar position
			if (CurrentSelectedAttack.AttachAttackTraceActorToMouse)
				OriginCoordinates = PlayerControllerReference->CursorLocationSnappedToGrid;
			else 
				OriginCoordinates = GetActorLocation().GridSnap(200.f);

			// Get all of the tiles and Avatars around the origin coordinates
			TArray<AActor*> GridTilesArray, AvatarsArray, ActorsArray;

			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor_GridTile::StaticClass(), GridTilesArray);
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter_Pathfinder::StaticClass(), AvatarsArray);

			ActorsArray.Append(GridTilesArray);
			ActorsArray.Append(AvatarsArray);

			for (int i = 0; i < ActorsArray.Num(); i++) {
				if ((ActorsArray[i]->GetActorLocation().X == OriginCoordinates.X || ActorsArray[i]->GetActorLocation().X == OriginCoordinates.X + 200.f || ActorsArray[i]->GetActorLocation().X == OriginCoordinates.X - 200.f) &&
					(ActorsArray[i]->GetActorLocation().Y == OriginCoordinates.Y || ActorsArray[i]->GetActorLocation().Y == OriginCoordinates.Y + 200.f || ActorsArray[i]->GetActorLocation().Y == OriginCoordinates.Y - 200.f) &&
					ActorsArray[i]->GetActorLocation() != OriginCoordinates) {
					ValidAttackTargetsArray.Add(ActorsArray[i]);

					if (Cast<AActor_GridTile>(ActorsArray[i])) {
						Cast<AActor_GridTile>(ActorsArray[i])->ChangeColourOnMouseHover = false;
						Cast<AActor_GridTile>(ActorsArray[i])->UpdateTileColour(E_GridTile_ColourChangeContext::WithinAttackRange);
					}
				} else {
					if (Cast<AActor_GridTile>(ActorsArray[i])) {
						Cast<AActor_GridTile>(ActorsArray[i])->ChangeColourOnMouseHover = true;
						Cast<AActor_GridTile>(ActorsArray[i])->UpdateTileColour(E_GridTile_ColourChangeContext::Normal);
					}
				}
			}
		}
		// Four-Way and Eight-Way Line Traces
		else if (CurrentSelectedAttack.AttackPattern == EBattle_AttackPatterns::FourWayCross || CurrentSelectedAttack.AttackPattern == EBattle_AttackPatterns::EightWayCross) {
			// Set the StaticMesh
			for (int i = 0; i < AttackTraceStaticMeshes.Num(); i++) {
				if (AttackTraceStaticMeshes[i]->GetName().Contains("Rectangle")) {
					AttackTraceActor->SetStaticMesh(AttackTraceStaticMeshes[i]);
					break;
				}
			}

			// Adjust the Rotation Snap degree
			if (CurrentSelectedAttack.AttackPattern == EBattle_AttackPatterns::EightWayCross)
				AttackRotationSnapToDegrees = 45;
			else
				AttackRotationSnapToDegrees = 90;

			int DefaultRectangleLocationX = 350;									// Add 100 for every tile range
			//int DefaultRectangleScale = 2 * CurrentSelectedAttack.BaseRange;		// Add 2 for every tile range
			int DefaultRectangleScale = CurrentSelectedAttack.BaseRange - 1;

			FVector RectangleLocation = FVector(200, 0, -100);
			FVector RectangleScale = FVector(1, 0.5, DefaultRectangleScale);

			AttackTraceActor->SetRelativeLocation(RectangleLocation);
			AttackTraceActor->SetRelativeScale3D(RectangleScale);
		}
		// Rock Wall attack pattern
		else if (CurrentSelectedAttack.AttackPattern == EBattle_AttackPatterns::WideWall) {
			// Set the StaticMesh
			for (int i = 0; i < AttackTraceStaticMeshes.Num(); i++) {
				if (AttackTraceStaticMeshes[i]->GetName().Contains("Rectangle")) {
					AttackTraceActor->SetStaticMesh(AttackTraceStaticMeshes[i]);
					break;
				}
			}

			FVector WideWallLocation = FVector(150, 0, -100);
			FVector WideWallScale = FVector(1.f, 2.f, 0.5f);

			AttackTraceActor->SetRelativeLocation(WideWallLocation);
			AttackTraceActor->SetRelativeScale3D(WideWallScale);
		}
	}
}


void ACharacter_Pathfinder::LaunchAttack_Implementation(AActor* Target)
{
	// Tell the client to update the server
	UE_LOG(LogTemp, Warning, TEXT("LaunchAttack / Attack chosen: %s"), *CurrentSelectedAttack.Name);
	PlayerControllerReference->Client_SendLaunchAttackToServer(this, Target, CurrentSelectedAttack.Name);
}


void ACharacter_Pathfinder::SetTilesOccupiedBySize()
{
	TArray<AActor*> OverlappingActors;
	FVector Start = GetActorLocation();

	for (int i = 0; i < AvatarData.OccupiedTiles.Num(); i++) {
		BoxComponent->SetWorldLocation(FVector(Start.X + (200 * AvatarData.OccupiedTiles[i].X), Start.Y + (200 * AvatarData.OccupiedTiles[i].Y), 0.f));
		BoxComponent->GetOverlappingActors(OverlappingActors, AActor_GridTile::StaticClass());
	}

	for (int j = 0; j < OverlappingActors.Num(); j++) {
		Cast<AActor_GridTile>(OverlappingActors[j])->Properties.AddUnique(E_GridTile_Properties::E_Occupied);
		UE_LOG(LogTemp, Warning, TEXT("SetTilesOccupiedBySize / Set Tile to be occupied."));

		if (Cast<AActor_GridTile>(OverlappingActors[j])->Properties.Contains(E_GridTile_Properties::E_None))
			Cast<AActor_GridTile>(OverlappingActors[j])->Properties.Remove(E_GridTile_Properties::E_None);
	}

	BoxComponent->SetWorldLocation(Start);
}


void ACharacter_Pathfinder::UpdateAvatarDataInPlayerParty()
{
	if (PlayerControllerReference)
		if (PlayerControllerReference->PlayerParty.IsValidIndex(IndexInPlayerParty))
			PlayerControllerReference->PlayerParty[IndexInPlayerParty] = AvatarData;
}


void ACharacter_Pathfinder::ActorSelectedDynamicMaterialColourUpdate_Implementation() 
{
	// Implemented in Blueprints
}


void ACharacter_Pathfinder::AvatarBeginTileOverlap_Implementation()
{
	// Implemented in Blueprints
}


void ACharacter_Pathfinder::AvatarStopMoving(bool SnapToGrid)
{
	// Make sure this Avatar's position is snapped to the grid
	if (GetActorLocation() != GetActorLocation().GridSnap(200) && SnapToGrid) {
		Cast<AAIController_Avatar>(GetController())->MoveToLocation((GetActorLocation().GridSnap(200)), -1.f, false, false, false, true, 0, false);
	}
}


// ------------------------- Multiplayer
void ACharacter_Pathfinder::Client_GetAvatarData_Implementation(FNetscapeExplorer_Struct NewAvatarData)
{
	Local_GetAvatarData(NewAvatarData);
}


void ACharacter_Pathfinder::Local_GetAvatarData(FNetscapeExplorer_Struct NewAvatarData)
{
	AvatarData = NewAvatarData;
}