#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/UniformGridPanel.h"

#include "Widget_AvatarSelection.generated.h"


// Forward Declarations
class UWidgetComponent_Avatar;


UCLASS()
class STARMARK_API UWidget_AvatarSelection : public UUserWidget
{
	GENERATED_BODY()
	
public:

// Variables
// --------------------------------------------------

// ------------------------- Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UUniformGridPanel* AvatarLibraryUniformGridPanel;

// ------------------------- References
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UWidgetComponent_Avatar> AvatarWidgetComponent_Class;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWidgetComponent_Avatar* AvatarWidgetComponent_Reference;

// Functions
// --------------------------------------------------

// ------------------------- Widget
	UFUNCTION(BlueprintCallable)
	void OnWidgetOpened();

};
