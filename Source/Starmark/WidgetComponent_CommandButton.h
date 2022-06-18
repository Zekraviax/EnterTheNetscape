#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "WidgetComponent_CommandButton.generated.h"


UCLASS()
class STARMARK_API UWidgetComponent_CommandButton : public UUserWidget
{
	GENERATED_BODY()
	
public:
// Functions
// --------------------------------------------------

// ------------------------- Commands
	UFUNCTION(BlueprintCallable)
	void AttackCommand();
};
