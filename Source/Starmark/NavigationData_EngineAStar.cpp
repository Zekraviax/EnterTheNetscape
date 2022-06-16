#include "NavigationData_EngineAStar.h"

#include "Actor_WorldGrid.h"
#include "AI/Navigation/NavigationTypes.h"
#include "Character_Pathfinder.h"
#include "EngineUtils.h"
#include "File_WorldGridPathfinding.h"
#include "Kismet/GameplayStatics.h"
#include "EnterTheNetscape_GameState.h"


ANavigationData_EngineAStar::ANavigationData_EngineAStar(const FObjectInitializer& ObjectInitializer)
{
	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		FindPathImplementation = FindPath;
		FindHierarchicalPathImplementation = FindPath;

		Pathfinder = new FGraphAStar<WorldGridGraph>(Graph);
	}
}


void ANavigationData_EngineAStar::BeginPlay()
{
	Super::BeginPlay();

	// Get the WorldGrid actor
	TArray<AActor*> WorldGridArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor_WorldGrid::StaticClass(), WorldGridArray);

	for (int i = 0; i < WorldGridArray.Num(); i++) {
		if (Cast<AActor_WorldGrid>(WorldGridArray[i])) {
			WorldGridReference = Cast<AActor_WorldGrid>(WorldGridArray[i]);
			Graph.SetWorldGrid(WorldGridReference);

			break;
		}
	}
}


FPathFindingResult ANavigationData_EngineAStar::FindPath(const FNavAgentProperties& AgentProperties, const FPathFindingQuery& Query)
{
	// Variable to return
	FPathFindingResult Result(ENavigationQueryResult::Error);

	// Get a reference to self
	const ANavigationData* Self = Query.NavData.Get();
	const ANavigationData_EngineAStar* AStar = (const ANavigationData_EngineAStar*)Self;

	check(AStar != nullptr);
	if (AStar == nullptr)
		return Result;

	// If the Query.PathInstanceToFill is valid, get a reference to it.
	// Otherwise, create a new instance.
	Result.Path = Query.PathInstanceToFill.IsValid() ? Query.PathInstanceToFill : Self->CreatePathInstance<FNavigationPath>(Query);

	//
	FNavigationPath* NavPath = Result.Path.Get();

	if (NavPath != nullptr) {
		// If the start point and end point are the same, just return a successful navigation
		if ((Query.StartLocation - Query.EndLocation).IsNearlyZero()) {
			Result.Path->GetPathPoints().Reset();
			Result.Path->GetPathPoints().Add(FNavPathPoint(Query.EndLocation));
			Result.Result = ENavigationQueryResult::Success;
		} else if (Query.QueryFilter.IsValid()) {
			FIntPoint StartPositionOnGrid, EndPositionOnGrid;
			AStar->WorldGridReference->ConvertWorldTileToGridCoordinates(Query.StartLocation, StartPositionOnGrid);
			AStar->WorldGridReference->ConvertWorldTileToGridCoordinates(Query.EndLocation, EndPositionOnGrid);

			UE_LOG(LogTemp, Warning, TEXT("NaviationData_EngineAStar / Start position on grid: %s"), *StartPositionOnGrid.ToString());
			UE_LOG(LogTemp, Warning, TEXT("NaviationData_EngineAStar / End position on grid: %s"), *EndPositionOnGrid.ToString());

			WorldGridGraphQueryFiler QueryFilter;
			QueryFilter.SetWorldGrid(AStar->WorldGridReference);
			QueryFilter.SetAllowPartialPaths(Query.bAllowPartialPaths);

			// Get the path ?
			TArray<FIntPoint> Path;
			EGraphAStarResult AStarResult = AStar->Pathfinder->FindPath(StartPositionOnGrid, EndPositionOnGrid, QueryFilter, Path);

			if (AStarResult == EGraphAStarResult::SearchFail || AStarResult == EGraphAStarResult::InfiniteLoop || AStarResult == EGraphAStarResult::GoalUnreachable) {
				UE_LOG(LogTemp, Warning, TEXT("NaviationData_EngineAStar / A* failed to find path: %d"), AStarResult);
				UE_LOG(LogTemp, Warning, TEXT("NaviationData_EngineAStar / Did search reach limit?: %s"), NavPath->DidSearchReachedLimit() ? TEXT("true") : TEXT("false"));
				//NavPath.
				

				Result.Result = ENavigationQueryResult::Fail;
				return Result;
			}

			Path.Insert(StartPositionOnGrid, 0);

			// Get the avatar's data so we can simulate movement without spending move points ?
			AEnterTheNetscape_GameState* GameStateReference = Cast<AEnterTheNetscape_GameState>(AStar->GetWorld()->GetGameState());

			// Insert all points from the path into the navpath
			for (auto& Point : Path) {
				// Subtract movement points only if the avatar is moving
				//if (GameStateReference->AvatarTurnOrder[GameStateReference->CurrentAvatarTurnIndex]->AvatarData.CurrentTileMoves >= QueryFilter.GetTraversalCost(Point, Point))
				//	GameStateReference->AvatarTurnOrder[GameStateReference->CurrentAvatarTurnIndex]->AvatarData.CurrentTileMoves -= QueryFilter.GetTraversalCost(Point, Point);

				NavPath->GetPathPoints().Add(FNavPathPoint(AStar->WorldGridReference->ConvertGridCoordinatesToWorldTile(Point)));

				UE_LOG(LogTemp, Warning, TEXT("NaviationData_EngineAStar / A* path coordinate converted to world tile: %s"), *AStar->WorldGridReference->ConvertGridCoordinatesToWorldTile(Point).ToString());
			}

			UE_LOG(LogTemp, Warning, TEXT("NaviationData_EngineAStar / A* path length: %d"), Path.Num());

			//
			NavPath->MarkReady();
			Result.Result = ENavigationQueryResult::Success;
		}
	}

	return Result;
}


bool ANavigationData_EngineAStar::ProjectPoint(const FVector & Point, FNavLocation & OutLocation, const FVector & Extent, FSharedConstNavQueryFilter Filter, const UObject * Querier) const
{
	return false;
}