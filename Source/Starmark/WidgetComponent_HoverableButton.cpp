#include "WidgetComponent_HoverableButton.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Widget_OnHoverDescription.h"


UWidget_OnHoverDescription* UWidgetComponent_HoverableButton::OnMouseBeginHover(FText TitleText, FText BodyText)
{
	UWidget_OnHoverDescription* ReturnWidget = nullptr;
	
	// Find an existing HoverDescription widget if this widget doesn't have a reference to one
	if (!IsValid(DescriptionWidget_Reference)) {
		TArray<UUserWidget*> FoundHoverDescriptionWidgets;
		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(this, FoundHoverDescriptionWidgets, DescriptionWidget_Class, false);

		// If there are no widgets, create one
		// Otherwise, get a reference to one and clean-up any others
		if (FoundHoverDescriptionWidgets.Num() > 0) {
			for (int i = 0; i < FoundHoverDescriptionWidgets.Num(); i++) {
				if (i == 0)
					DescriptionWidget_Reference = Cast<UWidget_OnHoverDescription>(FoundHoverDescriptionWidgets[i]);
				else
					FoundHoverDescriptionWidgets[i]->RemoveFromParent();
			}
		} else {
			if (IsValid(DescriptionWidget_Class))
				DescriptionWidget_Reference = CreateWidget<UWidget_OnHoverDescription>(this, DescriptionWidget_Class);

			// Add the widget to the HoverableButton's parent
		}
	}

	if (IsValid(DescriptionWidget_Reference)) {
		ReturnWidget = DescriptionWidget_Reference;
	}
	
	if (IsValid(DescriptionWidget_Reference)) {
		// Add widget to screen
		if (!DescriptionWidget_Reference->IsInViewport() && !DescriptionWidget_Reference->GetParent())
			DescriptionWidget_Reference->AddToViewport();

		if (DescriptionWidget_Reference->GetVisibility() == ESlateVisibility::Collapsed)
			DescriptionWidget_Reference->SetVisibility(ESlateVisibility::HitTestInvisible);

		// Set text
		DescriptionWidget_Reference->Title->SetText(TitleText);
		DescriptionWidget_Reference->Body->SetText(BodyText);
		
		// Set size
		const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
		Cast<UCanvasPanelSlot>(DescriptionWidget_Reference->Slot)->SetSize(FVector2D(FMath::CeilToInt(ViewportSize.X) / 3, FMath::CeilToInt(ViewportSize.Y) / 3)); 
		
		// Set position on screen
	}

	return ReturnWidget;
}


void UWidgetComponent_HoverableButton::OnMouseEndHover()
{
	TArray<UUserWidget*> FoundHoverDescriptionWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(this, FoundHoverDescriptionWidgets, DescriptionWidget_Class, false);

	if (FoundHoverDescriptionWidgets.Num() > 0) {
		for (int i = 0; i < FoundHoverDescriptionWidgets.Num(); i++) {
			FoundHoverDescriptionWidgets[i]	->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}