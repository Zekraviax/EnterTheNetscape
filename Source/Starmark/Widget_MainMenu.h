#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_MainMenu.generated.h"

// Forward Declarations
class UWidget_ServerCreator;
class UWidget_ServerBrowser;


UCLASS()
class STARMARK_API UWidget_MainMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
// Variables
// --------------------------------------------------

// -------------------------

	
// Function
// --------------------------------------------------

// ------------------------- Widget
	UFUNCTION(BlueprintCallable)
	void OnHostGameButtonPressed();

	UFUNCTION(BlueprintCallable)
	void OnSearchGameButtonPressed();
};
