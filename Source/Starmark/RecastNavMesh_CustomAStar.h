#pragma once

#include "CoreMinimal.h"
#include "NavMesh/RecastNavMesh.h"

#include "Engine.h"

#include "RecastNavMesh_CustomAStar.generated.h"


UCLASS()
class STARMARK_API ARecastNavMesh_CustomAStar : public ARecastNavMesh
{
	GENERATED_BODY()
	
	ARecastNavMesh_CustomAStar(const FObjectInitializer& ObjectInitializer);

	static FPathFindingResult FindPath(const FNavAgentProperties& AgentProperties, const FPathFindingQuery& Query);
};
