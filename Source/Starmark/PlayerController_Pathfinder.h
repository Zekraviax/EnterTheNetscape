#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerController_Pathfinder.generated.h"


UCLASS()
class STARMARK_API APlayerController_Pathfinder : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

public:
	APlayerController_Pathfinder();

};
