#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_PartyScreen.generated.h"


UCLASS()
class STARMARK_API UWidget_PartyScreen : public UUserWidget
{
	GENERATED_BODY()

public:
// Variables
// --------------------------------------------------

// ------------------------- Components
	

// Functions
// --------------------------------------------------

// ------------------------- Widget
	UFUNCTION(BlueprintCallable)
	void OnWidgetOpened();

	UFUNCTION(BlueprintCallable)
	void OnPauseButtonPressed();
};
