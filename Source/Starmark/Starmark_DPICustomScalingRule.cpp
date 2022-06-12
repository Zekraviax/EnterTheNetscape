#include "Starmark_DPICustomScalingRule.h"


float UStarmark_DPICustomScalingRule::GetDPIScaleBasedOnSize(FIntPoint Size) const
{
	#if PLATFORM_WINDOWS
	if (Size.X > 0 && Size.Y > 0) {
		float ax = Size.X / 1920.f;
		float ay = Size.Y / 1080.f;


		return FMath::Min(ax, ay);
	}
	#endif
	return 1.f;
}
