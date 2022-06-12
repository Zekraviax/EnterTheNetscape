#include "WidgetComponent_RightClickMenu.h"

#include "Components/CanvasPanelSlot.h"
#include "WidgetComponent_Avatar.h"
#include "WidgetComponent_RightClickMenuButton.h"


void UWidgetComponent_RightClickMenu::OnWidgetCreated()
{
	if (!Commands.Contains(E_RightClickMenu_Commands::Cancel))
		Commands.Add(E_RightClickMenu_Commands::Cancel);

	if (IsValid(RightClickMenuButton_Class)) {
		ButtonsVerticalBox->ClearChildren();

		for (int i = 0; i < Commands.Num(); i++) {
			RightClickMenuButton_Reference = CreateWidget<UWidgetComponent_RightClickMenuButton>(this, RightClickMenuButton_Class);
			RightClickMenuButton_Reference->ButtonLabel->SetText(UEnum::GetDisplayValueAsText(Commands[i]));
			RightClickMenuButton_Reference->Command = Commands[i];
			RightClickMenuButton_Reference->RightClickMenuWidget = this;
			
			Cast<UCanvasPanelSlot>(RightClickMenuButton_Reference->RightClickMenuButton->Slot)->SetSize(FVector2D(256.f, 48.f));

			ButtonsVerticalBox->AddChildToVerticalBox(RightClickMenuButton_Reference);
		}
	}
	
	Cast<UCanvasPanelSlot>(ButtonsVerticalBox->Slot)->SetPosition(GetRightClickMenuPosition(OwnerWidget));
}


FVector2D UWidgetComponent_RightClickMenu::GetRightClickMenuPosition(UWidget* Widget)
{
	const FGeometry Geometry = this->GetCachedGeometry();
	//const FVector2D WidgetPosition = Cast<UCanvasPanelSlot>(Widget->Slot)->GetPosition();

	const FVector2D Position = Geometry.AbsoluteToLocal(FVector2D(Widget->GetCachedGeometry().GetAbsolutePosition().X + Widget->GetCachedGeometry().GetLocalSize().X, Widget->GetCachedGeometry().GetAbsolutePosition().Y + Widget->GetCachedGeometry().GetLocalSize().Y / 2));
	return Position;
}