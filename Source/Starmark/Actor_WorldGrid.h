#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"


#include "Actor_WorldGrid.generated.h"


// Forward Declarations
class AActor_GridTile;


UCLASS()
class STARMARK_API AActor_WorldGrid : public AActor
{
	GENERATED_BODY()

public:
// Variables
// --------------------------------------------------

// ------------------------- 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FIntPoint> GridTileCoordinates;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FIntPoint MapSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector2D GridTileSize;

// Functions
// --------------------------------------------------

// ------------------------- 
	UFUNCTION()
	bool IsValidGridCell(const FIntPoint& Location) const;

	UFUNCTION()
	bool IsGridCellWalkable(const FIntPoint& Location) const;

	UFUNCTION(BlueprintCallable)
	bool ConvertWorldTileToGridCoordinates(const FVector& WorldPos, FIntPoint& GridPos) const;

	UFUNCTION(BlueprintCallable)
	FIntPoint ConvertGridTileLocationToCoordinates(FVector ActorLocation) const;

	UFUNCTION(BlueprintCallable)
	FVector ConvertGridCoordinatesToWorldTile(const FIntPoint& GridCoordinates) const;

	UFUNCTION(BlueprintCallable)
	FVector ConvertGridCoordinatesToWorldTileCenter(const FIntPoint& GridCoordinates) const;

	UFUNCTION(BlueprintCallable)
	AActor_GridTile* GetWorldTileActorAtGridCoordinates(const FIntPoint& GridCoordinates) const;

	UFUNCTION(BlueprintCallable)
	AActor_GridTile* FindGridTileAtCoordinates(FIntPoint GridCoordinates);
};