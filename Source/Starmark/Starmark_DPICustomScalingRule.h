#pragma once

#include "CoreMinimal.h"
#include "Engine/DPICustomScalingRule.h"
#include "Starmark_DPICustomScalingRule.generated.h"


UCLASS()
class STARMARK_API UStarmark_DPICustomScalingRule : public UDPICustomScalingRule
{
	GENERATED_BODY()
	
public:
	virtual float GetDPIScaleBasedOnSize(FIntPoint Size) const override;
};
