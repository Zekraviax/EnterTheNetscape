#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/Image.h"

#include "WidgetComponent_StatusEffectIcon.generated.h"


UCLASS()
class STARMARK_API UWidgetComponent_StatusEffectIcon : public UUserWidget
{
	GENERATED_BODY()

public:
// Variables
// --------------------------------------------------

// ------------------------- Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* StatusEffectIcon;
};
