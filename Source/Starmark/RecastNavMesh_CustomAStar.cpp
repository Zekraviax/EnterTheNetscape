#include "RecastNavMesh_CustomAStar.h"


#include "Engine/World.h"
#include "NavigationData.h"
#include "Actor_GridTile.h"
#include "PlayerController_Battle.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"


DECLARE_CYCLE_STAT(TEXT("Custom Pathfinding"), STAT_Navigation_CustomPathfinding, STATGROUP_Navigation)



ARecastNavMesh_CustomAStar::ARecastNavMesh_CustomAStar(const FObjectInitializer& ObjectInitializer)
	:ARecastNavMesh(ObjectInitializer)
{
	FindPathImplementation = FindPath;
}


FPathFindingResult ARecastNavMesh_CustomAStar::FindPath(const FNavAgentProperties& AgentProperties, const FPathFindingQuery& Query)
{
	SCOPE_CYCLE_COUNTER(STAT_Navigation_CustomPathfinding);

	// Get a reference to Self
	const ANavigationData* Self = Query.NavData.Get();
	const ARecastNavMesh_CustomAStar* RecastNavMesh = (const ARecastNavMesh_CustomAStar*)Self;

	check(RecastNavMesh != nullptr);

	if (RecastNavMesh == nullptr)
		return ENavigationQueryResult::Error;

	// Return variable
	FPathFindingResult Result(ENavigationQueryResult::Error);
	Result.Path = Query.PathInstanceToFill.IsValid() ? Query.PathInstanceToFill : Self->CreatePathInstance<FNavigationPath>(Query);

	FNavigationPath* NavPath = Query.PathInstanceToFill.Get();
	//FNavMeshPath* NavMeshPath = NavPath ? NavPath->CastPath<FNavMeshPath>() : nullptr;

	////
	//if (NavMeshPath) {
	//	Result.Path = Query.PathInstanceToFill;
	//	NavMeshPath->ResetForRepath();
	//} else {
	//	Result.Path = Self->CreatePathInstance<FNavMeshPath>(Query);
	//	NavPath = Result.Path.Get();
	//	NavMeshPath = NavPath ? NavPath->CastPath<FNavMeshPath>() : nullptr;
	//}

	//const FNavigationQueryFilter* NavFilter = Query.QueryFilter.Get();
	if (NavPath != nullptr)
	{
		//
		//NavMeshPath->ApplyFlags(Query.NavDataFlags);

		if (Query.QueryFilter.IsValid()) {
			// Array of neighbours of the tile that is currently being checked
			TArray<AActor_GridTile*> TileNeighbours;

			// List of tiles with priorities based on total movement cost
			// The key is the Priority, and the value is the Tile
			TArray<AActor_GridTile*> Frontier;
			TMap<AActor_GridTile*, int> PriorityQueue;

			// Progressive map of the cost to travel to the destination
			TMap<AActor_GridTile*, int> CostSoFarMap;

			// Map of tiles that point to the tile that the path 'comes from'
			TMap<AActor_GridTile*, AActor_GridTile*> CameFromMap;

			// The selected destination tile
			FVector GoalTileLocation = (Query.EndLocation.GridSnap(200) - Query.StartLocation.GridSnap(200)) / 200 + FVector(16.f, 16.f, 0.f);
			AActor_GridTile* GoalTile = nullptr;
			Result.Path->GetPathPoints().Add(FNavPathPoint(GoalTileLocation));

			// Get an array of all the tiles here, so we don't have to do it every time we check for a tile's neighbours
			TArray<AActor_GridTile*> AllTiles;
			TArray<AActor*> Actors;
			UGameplayStatics::GetAllActorsOfClass(RecastNavMesh->GetWorld(), AActor_GridTile::StaticClass(), Actors);
			for (int i = 0; i < Actors.Num(); i++) {
				if (Cast<AActor_GridTile>(Actors[i])) {
					AActor_GridTile* FoundTile = Cast<AActor_GridTile>(Actors[i]);

					AllTiles.Add(FoundTile);

					// If a tile matches the GoalTilePosition, save it as the GoalTile
					if (FoundTile->GetActorLocation().X == GoalTileLocation.X &&
						FoundTile->GetActorLocation().Y == GoalTileLocation.Y) {
						GoalTile = FoundTile;
					}
				}
			}

			// Add the start tile to grid tile array
			FVector StartLocation = Query.EndLocation;
			AActor_GridTile* StartTile = NULL;
			for (int i = 0; i < AllTiles.Num(); i++) {
				if (AllTiles[i]->GetActorLocation().X / 200 == Query.EndLocation.X && AllTiles[i]->GetActorLocation().Y / 200 == Query.EndLocation.Y) {
					StartTile = AllTiles[i];
					Frontier.Add(StartTile);
					break;
				}
			}

			// Add the start tile to the CameFromMap and set the 'came from' tile to itself
			CameFromMap.Add(StartTile, StartTile);

			// Add the start tile to the CostSoFarMap and set the cost to zero
			CostSoFarMap.Add(StartTile, 0);

			// Begin the A* search
			while (Frontier.Num() > 0) {
				// Get the current tile
				AActor_GridTile* CurrentTile = Frontier[0];

				// Break out of the while loop if we reach the destination
				if (CurrentTile == GoalTile) {
					Frontier.Empty();
				}
				else {
					// Remove the first element in the queue
					Frontier.RemoveAt(0);

					// Get the current tile's neighbours
					TileNeighbours.Empty();
					for (int i = 0; i < AllTiles.Num(); i++) {
						if (CurrentTile->GetActorLocation().X == AllTiles[i]->GetActorLocation().X + 200 &&
							CurrentTile->GetActorLocation().Y == AllTiles[i]->GetActorLocation().Y) {
							TileNeighbours.Add(AllTiles[i]);
						}
						else if (CurrentTile->GetActorLocation().X == AllTiles[i]->GetActorLocation().X &&
							CurrentTile->GetActorLocation().Y == AllTiles[i]->GetActorLocation().Y + 200) {
							TileNeighbours.Add(AllTiles[i]);
						}
						else if (CurrentTile->GetActorLocation().X == AllTiles[i]->GetActorLocation().X - 200 &&
							CurrentTile->GetActorLocation().Y == AllTiles[i]->GetActorLocation().Y) {
							TileNeighbours.Add(AllTiles[i]);
						}
						else if (CurrentTile->GetActorLocation().X == AllTiles[i]->GetActorLocation().X &&
							CurrentTile->GetActorLocation().Y == AllTiles[i]->GetActorLocation().Y - 200) {
							TileNeighbours.Add(AllTiles[i]);
						}
					}

					// test
					//Result.Path->GetPathPoints().Add(FNavPathPoint(CurrentTile->GetActorLocation()));

					// Add each neighbour to the frontier/priority queue if...
					// is equal to the last entry in the CameFromMap
					for (AActor_GridTile* NextTile : TileNeighbours) {
						//
						//int NewCost = CostSoFarMap[CurrentTile] + 1;

						//if (*CostSoFarMap.Find(NextTile) == CostSoFarMap.end().Value() || NewCost < *CostSoFarMap.Find(NextTile)) {
						/*
						//
						CostSoFarMap.Add(NextTile, NewCost);

						//
						PriorityQueue.Add(NextTile, NewCost);
						*/

						CameFromMap.Add(NextTile, CurrentTile);
						Frontier.Add(NextTile);
						//}
					}
				}
			}

			// Remove the Z location from the tile's location


			/*
			// Construct the path
			TArray<FVector> ConstructedPath;
			AActor_GridTile* CurrentTile = GoalTile;

			// Start from the end of the path and work our way backwards,
			// adding one element at a time to the path array until we reach the start
			while (CurrentTile->GetActorLocation() != StartLocation) {
				CurrentTile->DynamicMaterial->SetVectorParameterValue("Colour", FLinearColor::Blue);

				ConstructedPath.Insert(CurrentTile->GetActorLocation(), ConstructedPath.Num());
				CurrentTile = Cast<AActor_GridTile>(*CameFromMap.Find(CurrentTile));

				//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Path Length: %d"), ConstructedPath.Num()));
				UE_LOG(LogTemp, Warning, TEXT("RecastNavMesh_CustomAStar / Path Length: %d"), ConstructedPath.Num());
			}

			Algo::Reverse(ConstructedPath);

			for (int i = 0; i < ConstructedPath.Num(); i++) {
				Result.Path->GetPathPoints().Add(FNavPathPoint(ConstructedPath[i]));
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Path Length: %d"), ConstructedPath.Num()));
			}
			*/

			// Garbage collection
			//delete CurrentTile;

			//
			Result.Path->MarkReady();
			Result.Result = ENavigationQueryResult::Success;
		}
	}

	return Result;
}