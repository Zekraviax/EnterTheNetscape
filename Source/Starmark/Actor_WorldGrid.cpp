#include "Actor_WorldGrid.h"

#include "Actor_GridTile.h"
#include "Character_Pathfinder.h"
#include "Kismet/GameplayStatics.h"


bool AActor_WorldGrid::IsValidGridCell(const FIntPoint& Location) const
{
	bool FoundATile = GetWorldTileActorAtGridCoordinates(Location);

	return (Location.X < MapSize.X && Location.Y < MapSize.Y) && FoundATile;
}


bool AActor_WorldGrid::IsGridCellWalkable(const FIntPoint& Location) const
{
	// Check if the tile has any properties that make it un-traversable
	AActor_GridTile* GridTile = GetWorldTileActorAtGridCoordinates(Location);
	
	if (!IsValid(GridTile))
		return false;

	if (GridTile->Properties.Contains(E_GridTile_Properties::E_Occupied) ||
		GridTile->Properties.Contains(E_GridTile_Properties::E_Wall))
		return false;

	return true;
}

// This function is to be used for pathing only
// For other world position to tile coordinate conversions, use the other function:
// ConvertGridTileLocationToCoordinates
bool AActor_WorldGrid::ConvertWorldTileToGridCoordinates(const FVector& WorldPos, FIntPoint& GridPos) const
{
	FVector MyLocation = GetActorLocation();

	GridPos.X = (WorldPos.X - MyLocation.X) / GridTileSize.X;
	GridPos.Y = (WorldPos.Y - MyLocation.Y) / GridTileSize.Y;

	// Returns a bool, but also outputs the grid coordinates
	return (GridPos.X < MapSize.X && GridPos.Y < MapSize.Y);
}


// This function is a more sane version of the previous function:
// ConvertWorldTileToGridCoordinates
FIntPoint AActor_WorldGrid::ConvertGridTileLocationToCoordinates(FVector ActorLocation) const
{
	FIntPoint ReturnCoordinates;

	ReturnCoordinates.X = ActorLocation.X / GridTileSize.X;
	ReturnCoordinates.Y = ActorLocation.Y / GridTileSize.Y;

	return ReturnCoordinates;
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


// This function is to be used for pathing only
// For other world position to tile coordinate conversions, use the other function:
// FindGridTileAtCoordinates
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
			return TileReference;
		}
	}

	return TileReference;
}


AActor_GridTile* AActor_WorldGrid::FindGridTileAtCoordinates(FIntPoint GridCoordinates)
{
	AActor_GridTile* ReturnTileReference = nullptr;
	FIntPoint TileGridCoordinates;
	TArray<AActor*> GridTilesArray;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor_GridTile::StaticClass(), GridTilesArray);
	for (int i = 0; i < GridTilesArray.Num(); i++) {
		AActor_GridTile* FoundTile = Cast<AActor_GridTile>(GridTilesArray[i]);

		TileGridCoordinates = ConvertGridTileLocationToCoordinates(FoundTile->GetActorLocation());

		if (GridCoordinates == TileGridCoordinates) {
			ReturnTileReference = FoundTile;
			return ReturnTileReference;
		}
	}

	return ReturnTileReference;
}


ACharacter_Pathfinder* AActor_WorldGrid::FindCharacterAtCoordinates(FIntPoint GridCoordinates)
{
	ACharacter_Pathfinder* ReturnCharacter = nullptr;
	FIntPoint ActorGridCoordinates;
	TArray<AActor*> CharactersArray;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter_Pathfinder::StaticClass(), CharactersArray);
	for (int i = 0; i < CharactersArray.Num(); i++) {
		ACharacter_Pathfinder* FoundCharacter = Cast<ACharacter_Pathfinder>(CharactersArray[i]);

		ActorGridCoordinates = ConvertGridTileLocationToCoordinates(FoundCharacter->GetActorLocation());

		if (GridCoordinates == ActorGridCoordinates) {
			ReturnCharacter = FoundCharacter;
			return ReturnCharacter;
		}
	}

	return ReturnCharacter;
}