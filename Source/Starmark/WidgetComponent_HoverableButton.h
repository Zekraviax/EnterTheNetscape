#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetComponent_HoverableButton.generated.h"


// Forward Declarations
class UWidget_OnHoverDescription;


UCLASS()
class STARMARK_API UWidgetComponent_HoverableButton : public UUserWidget
{
	GENERATED_BODY()

public:
// Variables
// --------------------------------------------------

// ------------------------- References
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UWidget_OnHoverDescription> DescriptionWidget_Class;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWidget_OnHoverDescription* DescriptionWidget_Reference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidget* OwnerWidget;

// ------------------------- Widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DescriptionTitle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DescriptionBody;
	
// Functions
// --------------------------------------------------

// ------------------------- Widget
	UFUNCTION(BlueprintCallable)
	UWidget_OnHoverDescription* OnMouseBeginHover(FText TitleText, FText BodyText);

	UFUNCTION(BlueprintCallable)
	void OnMouseEndHover();
};
