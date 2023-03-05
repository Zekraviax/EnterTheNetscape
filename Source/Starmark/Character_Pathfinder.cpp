#include "Character_Pathfinder.h"

#include "Actor_GridTile.h"
#include "AIController_Avatar.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/EngineTypes.h"
#include "Engine/World.h"
#include "EnterTheNetscape_GameState.h"
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
	ActorHighlightedDecal = CreateDefaultSubobject<UDecalComponent>("ActorHighlightedDecal");
	ActorHighlightedDecal->SetupAttachment(RootComponent);
	ActorHighlightedDecal->SetVisibility(true);
	ActorHighlightedDecal->SetHiddenInGame(false);
	ActorHighlightedDecal->DecalSize = FVector(32.0f, 64.0f, 64.0f);
	//ActorHighlightedDecal->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
	ActorHighlightedDecal->SetRelativeScale3D(FVector(1.25f, 1.f, 2.f));

	// Actor Selected Plane
	ActorSelectedPlane = CreateDefaultSubobject<UStaticMeshComponent>("ActorSelectedPlane");
	ActorSelectedPlane->SetupAttachment(RootComponent);
	ActorSelectedPlane->SetVisibility(true);
	ActorSelectedPlane->SetHiddenInGame(false);

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
	// Snap Actor to Grid
	// The Z Value needs to be retained or else the character will probably clip through the floor
	FVector ActorLocationSnappedToGrid = GetActorLocation().GridSnap(200.f);
	ActorLocationSnappedToGrid.Z = GetActorLocation().Z;
	SetActorLocation(ActorLocationSnappedToGrid);

	AvatarData.CurrentHealthPoints = AvatarData.BattleStats.MaximumHealthPoints;
	AvatarData.CurrentManaPoints = AvatarData.BattleStats.MaximumManaPoints;
	AvatarData.CurrentTileMoves = AvatarData.MaximumTileMoves;

	// Set default selected attack
	//if (CurrentKnownAttacks.Num() > 0)
	//	CurrentSelectedAttack = CurrentKnownAttacks[0];

	IndexInPlayerParty = 0;

	// Highlight dynamic material
	ActorHighlightedDecalDynamicMaterial = UMaterialInstanceDynamic::Create(ActorHighlightMaterial, this);
	ActorHighlightedDecal->SetMaterial(0, ActorHighlightedDecalDynamicMaterial);
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

	ActorHighlightedDecal->SetVisibility(true);
}


void ACharacter_Pathfinder::OnAvatarCursorOverEnd()
{
	AvatarBattleData_Component->SetHiddenInGame(true);

	ActorHighlightedDecal->SetVisibility(false);
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
	//// Get all tiles and overlapping tiles
	//TArray<AActor*> GridTilesArray;
	//TArray<UPrimitiveComponent*> OverlappingActors;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor_GridTile::StaticClass(), GridTilesArray);

	//// Check for tiles that overlap the AttackTraceActor
	//for (int i = 0; i < GridTilesArray.Num(); i++) {
	//	AActor_GridTile* GridTile = Cast<AActor_GridTile>(GridTilesArray[i]);

	//	GridTile->GetOverlappingComponents(OverlappingActors);
	//	if (OverlappingActors.Contains(AttackTraceActor)) {
	//		GridTile->ChangeColourOnMouseHover = false;
	//		GridTile->DynamicMaterial->SetVectorParameterValue(FName("Colour"), FLinearColor(1.f, 0.2f, 0.0f));
	//	} else {
	//		GridTile->ChangeColourOnMouseHover = true;
	//		GridTile->DynamicMaterial->SetVectorParameterValue(FName("Colour"), FLinearColor(1.f, 1.f, 1.f));
	//	}
	//}
}

// ------------------------- Battle
void ACharacter_Pathfinder::ShowAttackRange()
{
	//AttackTraceActor->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
	//AttackTraceActor->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//AttackTraceActor->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	//// Turn off all highlights
	//TArray<AActor*> GridTilesArray;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor_GridTile::StaticClass(), GridTilesArray);
	//for (AActor* GridTileInArray : GridTilesArray) {
	//	AActor_GridTile* Tile = Cast<AActor_GridTile>(GridTileInArray);
	//	Tile->SetTileHighlightProperties(false, true, E_GridTile_ColourChangeContext::Normal);
	//}

	//AttackTraceActor->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//AttackTraceActor->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	//if (CurrentSelectedAttack.AttackPattern == EBattle_AttackPatterns::WideWall) {
	//	// Set the StaticMesh
	//	for (int i = 0; i < AttackTraceStaticMeshes.Num(); i++) {
	//		if (AttackTraceStaticMeshes[i]->GetName().Contains("Rectangle")) {
	//			AttackTraceActor->SetStaticMesh(AttackTraceStaticMeshes[i]);
	//			break;
	//		}
	//	}

	//	FVector WideWallLocation = FVector(150, 0, -100);
	//	FVector WideWallScale = FVector(0.5f, 2.f, 0.2f);

	//	AttackTraceActor->SetRelativeLocation(WideWallLocation);
	//	AttackTraceActor->SetRelativeScale3D(WideWallScale);
	//}		
	//// Four-Way and Eight-Way Line Traces
	//else if (CurrentSelectedAttack.AttackPattern == EBattle_AttackPatterns::FourWayCross || CurrentSelectedAttack.AttackPattern == EBattle_AttackPatterns::EightWayCross) {
	//	// Set the StaticMesh
	//	for (int i = 0; i < AttackTraceStaticMeshes.Num(); i++) {
	//		if (AttackTraceStaticMeshes[i]->GetName().Contains("Rectangle")) {
	//			AttackTraceActor->SetStaticMesh(AttackTraceStaticMeshes[i]);
	//			break;
	//		}
	//	}

	//	// Adjust the Rotation Snap degree
	//	if (CurrentSelectedAttack.AttackPattern == EBattle_AttackPatterns::EightWayCross)
	//		AttackRotationSnapToDegrees = 45;
	//	else
	//		AttackRotationSnapToDegrees = 90;

	//	int DefaultRectangleLocationX = 350; // Add 100 for every tile range
	//	int DefaultRectangleScale = CurrentSelectedAttack.BaseRange - 1;

	//	FVector RectangleLocation = FVector(200, 0, -100);
	//	FVector RectangleScale = FVector(1, 0.1, DefaultRectangleScale);

	//	AttackTraceActor->SetRelativeLocation(RectangleLocation);
	//	AttackTraceActor->SetRelativeScale3D(RectangleScale);
	//}
}


void ACharacter_Pathfinder::SetActorHighlightProperties(bool IsVisible, E_GridTile_ColourChangeContext ColourChangeContext)
{
	if (ActorHighlightedDecal->IsValidLowLevel() && ActorHighlightedDecalDynamicMaterial->IsValidLowLevel()) {
		ActorHighlightedDecal->SetVisibility(IsVisible);

		switch (ColourChangeContext)
		{
		case (E_GridTile_ColourChangeContext::Normal):
			// Heirarcy of colours based on factors such as tile properties
			// Lowest priority: White (no properties that change colour)
			ActorHighlightedDecalDynamicMaterial->SetVectorParameterValue("Colour", FLinearColor(1.f, 1.f, 1.f, 1.f));
			break;
		case (E_GridTile_ColourChangeContext::OnMouseHover):
			ActorHighlightedDecalDynamicMaterial->SetVectorParameterValue("Colour", FLinearColor(0.f, 1.f, 0.f, 1.f));
			break;
		case (E_GridTile_ColourChangeContext::OnMouseHoverTileUnreachable):
			ActorHighlightedDecalDynamicMaterial->SetVectorParameterValue("Colour", FLinearColor(1.f, 0.f, 0.f, 1.f));
			break;
		case (E_GridTile_ColourChangeContext::WithinAttackRange):
			ActorHighlightedDecalDynamicMaterial->SetVectorParameterValue("Colour", FLinearColor(1.f, 0.2f, 0.f, 1.f));
			break;
		default:
			break;
		}
	}
}


void ACharacter_Pathfinder::GetValidActorsForAttack_Implementation(FAvatar_AttackStruct Attack, AActor* CurrentlyHoveredActor)
{
	if (Cast<AEnterTheNetscape_GameState>(GetWorld()->GetGameState())->DynamicAvatarTurnOrder.Num() > 0) {
		if (this == Cast<AEnterTheNetscape_GameState>(GetWorld()->GetGameState())->DynamicAvatarTurnOrder[0]) {
			TArray<FVector2D> ValidVectors;
			TArray<AActor*> GridTilesArray, EntitiesArray;

			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor_GridTile::StaticClass(), GridTilesArray);
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter_Pathfinder::StaticClass(), EntitiesArray);

			ValidAttackTargetsArray.Empty();

			switch (Attack.AttackPattern)
			{
			case(EBattle_AttackPatterns::SingleTile):
				if (Cast<AActor_GridTile>(CurrentlyHoveredActor) || Cast<ACharacter_Pathfinder>(CurrentlyHoveredActor)) {
					ValidAttackTargetsArray.Add(CurrentlyHoveredActor);
				}
				break;
			case(EBattle_AttackPatterns::FourWayCross):
				ValidVectors.Add(FVector2D(this->GetActorLocation().X, this->GetActorLocation().Y));

				for (int i = 1; i <= Attack.BaseRange; i++) {
					ValidVectors.Add(FVector2D(this->GetActorLocation().X + (200 * i), this->GetActorLocation().Y));
					ValidVectors.Add(FVector2D(this->GetActorLocation().X - (200 * i), this->GetActorLocation().Y));
					ValidVectors.Add(FVector2D(this->GetActorLocation().X, this->GetActorLocation().Y + (200 * i)));
					ValidVectors.Add(FVector2D(this->GetActorLocation().X, this->GetActorLocation().Y - (200 * i)));
				}

				for (AActor* GridTile : GridTilesArray) {
					if (ValidVectors.Contains(FVector2D(GridTile->GetActorLocation().X, GridTile->GetActorLocation().Y))) {
						ValidAttackTargetsArray.Add(GridTile);
					}
				}

				for (AActor* Entity : EntitiesArray) {
					if (ValidVectors.Contains(FVector2D(Entity->GetActorLocation().X, Entity->GetActorLocation().Y))) {
						ValidAttackTargetsArray.Add(Entity);
					}
				}

				break;
			default:
				break;
			}
		}
	}
}


void ACharacter_Pathfinder::LaunchAttack_Implementation(AActor* Target)
{
	// Tell the client to update the server
	UE_LOG(LogTemp, Warning, TEXT("LaunchAttack / Attack chosen: %s"), *CurrentSelectedAttack.Name);
	PlayerControllerReference->Client_SendLaunchAttackToServer(this, Target, CurrentSelectedAttack.Name);
}


void ACharacter_Pathfinder::SetTilesOccupiedBySize(bool ClearTiles)
{
	TArray<AActor*> OverlappingActors;
	FVector Start = GetActorLocation();

	// Clear out tiles that were overlapped
	if (ClearTiles) {
		TArray<AActor*> GridTilesArray;

		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor_GridTile::StaticClass(), GridTilesArray);
		for (AActor* TileInArray : GridTilesArray) {
			AActor_GridTile* ActorAsGridTile = Cast<AActor_GridTile>(TileInArray);

			if (ActorAsGridTile->OccupyingActor == this) {
				ActorAsGridTile->OccupyingActor = nullptr;

				if (ActorAsGridTile->Properties.Contains(E_GridTile_Properties::E_Occupied)) {
					ActorAsGridTile->Properties.Remove(E_GridTile_Properties::E_Occupied);
				}

				if (ActorAsGridTile->Properties.Num() <= 0) {
					ActorAsGridTile->Properties.AddUnique(E_GridTile_Properties::E_None);
				}
			}
		}
	}

	// Set overlapping tiles to 'Occupied'
	for (int j = 0; j < OverlappingActors.Num(); j++) {
		Cast<AActor_GridTile>(OverlappingActors[j])->Properties.AddUnique(E_GridTile_Properties::E_Occupied);
		Cast<AActor_GridTile>(OverlappingActors[j])->OccupyingActor = this;

		UE_LOG(LogTemp, Warning, TEXT("SetTilesOccupiedBySize / Set Tile to be occupied."));

		if (Cast<AActor_GridTile>(OverlappingActors[j])->Properties.Contains(E_GridTile_Properties::E_None)) {
			Cast<AActor_GridTile>(OverlappingActors[j])->Properties.Remove(E_GridTile_Properties::E_None);
		}
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


ECharacter_FacingDirections ACharacter_Pathfinder::GetCharacterFacingDirection()
{
	// Normalize yaw rotation value to be between 0 and 360
	//while (GetActorRotation().Yaw < 0.f) {
	//	SetActorRotation(FRotator(GetActorRotation().Pitch, GetActorRotation().Yaw + 360.f, GetActorRotation().Roll));
	//	UE_LOG(LogTemp, Warning, TEXT("Spirit_Blunderbuss_Implementation / Actor rotation is: %f %f %f"), GetActorRotation().Pitch, (FMath::FloorToInt(GetActorRotation().Yaw) % 360), GetActorRotation().Roll);
	//}

	//if (GetActorRotation().Yaw > 0.f) {
	//	SetActorRotation(FRotator(GetActorRotation().Pitch, GetActorRotation().Yaw - 360.f, GetActorRotation().Roll));
	//	UE_LOG(LogTemp, Warning, TEXT("Spirit_Blunderbuss_Implementation / Actor rotation is: %f %f %f"), GetActorRotation().Pitch, GetActorRotation().Yaw, GetActorRotation().Roll);
	//}

	ECharacter_FacingDirections ReturnDirection;

	if (GetActorRotation().Yaw >= -315.f && GetActorRotation().Yaw <= -225.f) {
		// Bottom Right
		ReturnDirection = ECharacter_FacingDirections::BottomRight;
	} else if (GetActorRotation().Yaw >= -225.f && GetActorRotation().Yaw <= -135.f) {
		// Bottom Left
		ReturnDirection = ECharacter_FacingDirections::BottomLeft;
	} else if (GetActorRotation().Yaw >= -135.f && GetActorRotation().Yaw <= -45.f) {
		// Top Left
		ReturnDirection = ECharacter_FacingDirections::TopLeft;
	} else if (GetActorRotation().Yaw >= -45.f && GetActorRotation().Yaw <= 45.f) {
		// Top Right
		ReturnDirection = ECharacter_FacingDirections::TopRight;
	} else if (GetActorRotation().Yaw >= 45.f && GetActorRotation().Yaw <= 135.f) {
		// Bottom Right
		ReturnDirection = ECharacter_FacingDirections::BottomRight;
	} else if (GetActorRotation().Yaw >= 135.f && GetActorRotation().Yaw <= 225.f) {
		// Bottom Left
		ReturnDirection = ECharacter_FacingDirections::BottomLeft;
	} else if (GetActorRotation().Yaw >= 225.f && GetActorRotation().Yaw <= 315.f) {
		// Top Left
		ReturnDirection = ECharacter_FacingDirections::TopLeft;
	} else if (GetActorRotation().Yaw >= 315.f && GetActorRotation().Yaw <= 405.f) {
		// Top Right
		ReturnDirection = ECharacter_FacingDirections::TopRight;
	} else {
		UE_LOG(LogTemp, Warning, TEXT("ACharacter_Pathfinder  /  GetCharacterFacingDirection()  /  Error: Character Yaw Value Too High Or Low"));
	}

	return ReturnDirection;
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