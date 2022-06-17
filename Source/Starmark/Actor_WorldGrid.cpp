#include "Actor_WorldGrid.h"

#include "Actor_GridTile.h"
#include "Kismet/GameplayStatics.h"


bool AActor_WorldGrid::IsValidGridCell(const FIntPoint& Location) const
{
	bool FoundATile = GetWorldTileActorAtGridCoordinates(Location);

	return (Location.X < MapSize.X && Location.Y < MapSize.Y) && FoundATile;
}


bool AActor_WorldGrid::IsGridCellWalkable(const FIntPoint& Location) const
{
	// Check if the tile has any properties that make it un-traversable
	AActor_GridTile* GridTile = nullptr;
	GridTile = GetWorldTileActorAtGridCoordinates(Location);
	
	if (!IsValid(GridTile))
		return false;

	if (GridTile->Properties.Contains(E_GridTile_Properties::E_Occupied) ||
		GridTile->Properties.Contains(E_GridTile_Properties::E_Wall))
		return false;

	return true;
}


bool AActor_WorldGrid::ConvertWorldTileToGridCoordinates(const FVector& WorldPos, FIntPoint& GridPos) const
{
	FVector MyLocation = GetActorLocation();

	GridPos.X = (WorldPos.X - MyLocation.X) / GridTileSize.X;
	GridPos.Y = (WorldPos.Y - MyLocation.Y) / GridTileSize.Y;

	// Returns a bool, but also outputs the grid coordinates
	return (GridPos.X < MapSize.X && GridPos.Y < MapSize.Y);
}


FVector AActor_WorldGrid::ConvertGridCoordinatesToWorldTile(const FIntPoint& GridCoordinates) const
{
	FVector MyLocation = GetActorLocation();

	return FVector(GridCoordinates.X * GridTileSize.X + MyLocation.X, GridCoordinates.Y * GridTileSize.Y + MyLocation.Y, MyLocation.Z);
}


FVector AActor_WorldGrid::ConvertGridCoordinatesToWorldTileCenter(const FIntPoint& GridCoordinates) const
{
	return ConvertGridCoordinatesToWorldTile(GridCoordinates) + (FVector(GridTileSize.Y, GridTileSize.X, 0) * 0.5f);
}


AActor_GridTile* AActor_WorldGrid::GetWorldTileActorAtGridCoordinates(const FIntPoint& GridCoordinates) const
{
	AActor_GridTile* TileReference = nullptr;
	TArray<AActor*> GridTilesArray;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor_GridTile::StaticClass(), GridTilesArray);
	for (int i = 0; i < GridTilesArray.Num(); i++) {
		AActor_GridTile* FoundTile = Cast<AActor_GridTile>(GridTilesArray[i]);
		FIntPoint TileGridCoordinates;

		ConvertWorldTileToGridCoordinates(FoundTile->GetActorLocation(), TileGridCoordinates);

		if (GridCoordinates == TileGridCoordinates) {
			TileReference = FoundTile;
			break;
		}
	}

	return TileReference;
}
