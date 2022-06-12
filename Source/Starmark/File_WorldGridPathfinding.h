#pragma once

#include "CoreMinimal.h"
#include "GraphAStar.h"

#include "Actor_WorldGrid.h"


class STARMARK_API WorldGridGraphQueryFiler
{
public:
	// Constructor ???
	WorldGridGraphQueryFiler() : WorldGridReference(nullptr), AllowPartialPaths(false) {}

private:
// Variables
// --------------------------------------------------

// ------------------------- 
	AActor_WorldGrid* WorldGridReference;

	bool AllowPartialPaths;

public:
// Functions
// --------------------------------------------------

// ------------------------- 
	UFUNCTION()
	void SetWorldGrid(AActor_WorldGrid* SetWorldGridReference);

	UFUNCTION()
	float GetHeuristicScale() const;

	UFUNCTION()
	float GetHeuristicCost(FIntPoint StartLocation, FIntPoint EndLocation) const;

	UFUNCTION()
	float GetTraversalCost(FIntPoint StartLocation, FIntPoint EndLocation) const;

	UFUNCTION()
	bool IsTraversalAllowed(const FIntPoint FirstPoint, const FIntPoint SecondPoint) const;

	UFUNCTION()
	bool WantsPartialSolution() const;

	UFUNCTION()
	void SetAllowPartialPaths(bool InPartialPaths);
};


class STARMARK_API WorldGridGraph
{

private:
// Variables
// --------------------------------------------------

// ------------------------- 
	UPROPERTY()
	AActor_WorldGrid* WorldGridReference;

public:
	typedef FIntPoint FNodeRef;

// Functions
// --------------------------------------------------

// ------------------------- 
	void SetWorldGrid(AActor_WorldGrid* InWorldGrid);

	int32 GetNeighbourCount(FNodeRef NodeRef) const;

	bool IsValidRef(FNodeRef NodeRef) const;

	FNodeRef GetNeighbour(const FNodeRef NodeRef, const int32 NeighbourIndex) const;
};
