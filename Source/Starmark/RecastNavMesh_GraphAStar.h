#pragma once

#include "CoreMinimal.h"
#include "NavMesh/RecastNavMesh.h"

#include "Engine.h"

#include "RecastNavMesh_GraphAStar.generated.h"

// Horizontal Length of the map
const static int n = 32;
// Vertical Length of the map
const static int m = 32;
// Directions a player can traverse. 4 = Cardinal Directions only. 8 = Cardinal Directions + Diagonals
const static int dir = 4;
// if using 8-directions, be sure to include the definitions here
const static int dx[dir] = { 1, 0, -1, 0 };
const static int dy[dir] = { 0, 1, 0, -1 };


UCLASS()
class STARMARK_API ARecastNavMesh_GraphAStar : public ARecastNavMesh
{
	GENERATED_BODY()
	
	ARecastNavMesh_GraphAStar(const FObjectInitializer& ObjectInitializer);

	static FPathFindingResult FindPath(const FNavAgentProperties& AgentProperties, const FPathFindingQuery& Query);
};