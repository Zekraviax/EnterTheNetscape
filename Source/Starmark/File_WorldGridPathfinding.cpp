#include "File_WorldGridPathfinding.h"

#include "Actor_GridTile.h"
#include "Actor_WorldGrid.h"


// ------------------------- WorldGridGraphQueryFilter Functions
void WorldGridGraphQueryFiler::SetWorldGrid(AActor_WorldGrid* SetWorldGridReference)
{
	WorldGridReference = SetWorldGridReference;
}


float WorldGridGraphQueryFiler::GetHeuristicScale() const
{
	return 1.f;
}


float WorldGridGraphQueryFiler::GetHeuristicCost(FIntPoint StartLocation, FIntPoint EndLocation) const
{
	return FMath::Abs(StartLocation.X - EndLocation.X) + FMath::Abs(StartLocation.Y - EndLocation.Y);
}


float WorldGridGraphQueryFiler::GetTraversalCost(FIntPoint StartLocation, FIntPoint EndLocation) const
{
	float TraversalCost = 1.f;
	AActor_GridTile* TileReference = nullptr;
	TileReference = WorldGridReference->GetWorldTileActorAtGridCoordinates(StartLocation);

	if (TileReference->IsValidLowLevel()) {
		if (TileReference->Properties.Contains(E_GridTile_Properties::E_StoneRoad))
			TraversalCost = 0.f;
		else
			TraversalCost = TileReference->MovementCost;
	}

	return TraversalCost;
}


bool WorldGridGraphQueryFiler::IsTraversalAllowed(const FIntPoint FirstPoint, const FIntPoint SecondPoint) const
{
	//if (!WorldGridReference->IsGridCellWalkable(FirstPoint) || !WorldGridReference->IsGridCellWalkable(SecondPoint))
	if (!WorldGridReference->IsGridCellWalkable(SecondPoint))
		return false;

	static const FIntPoint XOffset(1, 0);
	static const FIntPoint YOffset(0, 1);

	if (SecondPoint.X < FirstPoint.X) {
		if (SecondPoint.Y < FirstPoint.Y) {
			if ((!WorldGridReference->IsGridCellWalkable(FirstPoint - XOffset)) || (!WorldGridReference->IsGridCellWalkable(FirstPoint - YOffset)))
				return false;
		} else if (SecondPoint.Y > FirstPoint.Y) {
			if ((!WorldGridReference->IsGridCellWalkable(FirstPoint - XOffset)) || (!WorldGridReference->IsGridCellWalkable(FirstPoint + YOffset)))
				return false;
		}
	} else if (SecondPoint.X > FirstPoint.X) {
		if (SecondPoint.Y < FirstPoint.Y) {
			if ((!WorldGridReference->IsGridCellWalkable(FirstPoint + XOffset)) || (!WorldGridReference->IsGridCellWalkable(FirstPoint - YOffset)))
				return false;
		}
		else if (SecondPoint.Y > FirstPoint.Y) {
			if ((!WorldGridReference->IsGridCellWalkable(FirstPoint + XOffset)) || (!WorldGridReference->IsGridCellWalkable(FirstPoint + YOffset)))
				return false;
		}
	}

	return true;
}


bool WorldGridGraphQueryFiler::WantsPartialSolution() const
{
	return AllowPartialPaths;
}


void WorldGridGraphQueryFiler::SetAllowPartialPaths(bool InPartialPaths)
{
	AllowPartialPaths = InPartialPaths;
}


// ------------------------- WorldGridGraph Functions
void WorldGridGraph::SetWorldGrid(AActor_WorldGrid* InWorldGrid)
{
	WorldGridReference = InWorldGrid;
}


int32 WorldGridGraph::GetNeighbourCount(FNodeRef NodeRef) const
{
	return 4;
}


bool WorldGridGraph::IsValidRef(FNodeRef NodeRef) const
{
	return WorldGridReference->IsValidGridCell(NodeRef);
}


WorldGridGraph::FNodeRef WorldGridGraph::GetNeighbour(const FNodeRef NodeRef, const int32 NeighbourIndex) const
{
	static const FIntPoint NeighbourCoordinateOffsets[4] =
	{
		{ 1, 0 },
		{ 0, 1 },
		{ 0, -1 },
		{ -1, 0 },
	};

	return NodeRef + NeighbourCoordinateOffsets[NeighbourIndex];
}