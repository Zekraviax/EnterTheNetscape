#pragma once

#include "CoreMinimal.h"
#include "NavigationData.h"

#include "File_WorldGridPathfinding.h"
#include "GraphAStar.h"
#include "NavigationData.h"

#include "NavigationData_EngineAStar.generated.h"


// Forward Declarations
class AActor_WorldGrid;


UCLASS(config = Engine, defaultconfig, hidecategories = (Input, Rendering, Tags, "Utilities|Transformation", Actor, Layers, Replication), notplaceable)
class STARMARK_API ANavigationData_EngineAStar : public ANavigationData
{
	GENERATED_BODY()

	ANavigationData_EngineAStar(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

private:
// Variables
// --------------------------------------------------

// ------------------------- 
	UPROPERTY()
	AActor_WorldGrid* WorldGridReference;
	
	// Can't be a UPROPERTY
	WorldGridGraph Graph;

	// Can't be a UPROPERTY
	FGraphAStar<WorldGridGraph>* Pathfinder;

public:
// Functions
// --------------------------------------------------

// ------------------------- 
	static FPathFindingResult FindPath(const FNavAgentProperties& AgentProperties, const FPathFindingQuery& Query);

	bool ProjectPoint(const FVector& Point, FNavLocation& OutLocation, const FVector& Extent, FSharedConstNavQueryFilter Filter = NULL, const UObject* Querier = NULL) const override;
};
