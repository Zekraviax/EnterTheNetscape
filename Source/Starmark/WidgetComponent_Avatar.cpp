#include "WidgetComponent_Avatar.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Widget_AvatarCreation.h"
#include "Widget_AvatarLibrary.h"
#include "WidgetComponent_RightClickMenu.h"


// ------------------------- Widget
void UWidgetComponent_Avatar::OnInteractButtonPressed()
{
	switch (CurrentFunction)
	{
		// Open AvatarCreator
		case (E_AvatarWidgetComponent_Function::E_CreateNewAvatarInLibrary):
			if (AvatarCreationWidget_Class) {
				AvatarCreationWidget_Reference = CreateWidget<UWidget_AvatarCreation>(this, AvatarCreationWidget_Class);
				AvatarCreationWidget_Reference->PopulateDropDowns();
				AvatarCreationWidget_Reference->AddToViewport();

				// Bind delegate in the AvatarLibrary
				if (Cast<UWidget_AvatarLibrary>(PairedWidget))
					Cast<UWidget_AvatarLibrary>(PairedWidget)->BindAvatarCreatedDelegate(this);
			}
			break;
		//case (E_AvatarWidgetComponent_Function::E_AddAvatarToChosenSlot):
		//	Cast<APlayerController_Lobby>(GetWorld()->GetFirstPlayerController())->OnAvatarWidgetComponentClicked(this);
		//	break;
		case (E_AvatarWidgetComponent_Function::E_Nothing):
			break;
		default:
			break;
	}

	if (OnAvatarSelectedDelegate.IsBound())
		OnAvatarSelectedDelegate.Broadcast();
}


void UWidgetComponent_Avatar::OnRightMouseButtonDown()
{
	TArray<UUserWidget*> FoundRightClickMenuWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(this, FoundRightClickMenuWidgets, UWidgetComponent_RightClickMenu::StaticClass(), true);

	if (FoundRightClickMenuWidgets.Num() > 0) {
		for (int i = 0; i < FoundRightClickMenuWidgets.Num(); i++) {
			if (i == 0)
				RightClickMenu_Reference = Cast<UWidgetComponent_RightClickMenu>(FoundRightClickMenuWidgets[i]);
			else
				FoundRightClickMenuWidgets[i]->RemoveFromParent();
		}
	} else {
		if (IsValid(RightClickMenu_Class))
			RightClickMenu_Reference = CreateWidget<UWidgetComponent_RightClickMenu>(this, RightClickMenu_Class);
	}


	if (IsValid(RightClickMenu_Reference)) {
		RightClickMenu_Reference->Commands.Empty();
		RightClickMenu_Reference->Commands.Append(RightClickMenuCommands);

		RightClickMenu_Reference->OwnerWidget = this;

		if (!RightClickMenu_Reference->IsInViewport())
			RightClickMenu_Reference->AddToViewport();

		RightClickMenu_Reference->OnWidgetCreated();
	}
}


void UWidgetComponent_Avatar::UpdateWidgetMaterials_Implementation()
{
	// Implemented in blueprints
}


void UWidgetComponent_Avatar::ApplyNewAvatarData(FNetscapeExplorer_Struct NewAvatarData)
{
	// Empty Slot
	if (NewAvatarData.Nickname.Len() == 0 &&
		NewAvatarData.NetscapeExplorerName == "Default") {
		NetscapeExplorerName->SetText(FText::FromString("Empty Slot"));
	} else {
		AvatarData = NewAvatarData;
		AvatarMaterial = AvatarData.DefaultImage;
		NetscapeExplorerName->SetText(FText::FromString(AvatarData.NetscapeExplorerName));

		UpdateWidgetMaterials();
	}
}


void UWidgetComponent_Avatar::RightClickMenuFunction_EditAvatar()
{
	if (AvatarCreationWidget_Class) {
		AvatarCreationWidget_Reference = CreateWidget<UWidget_AvatarCreation>(this, AvatarCreationWidget_Class);
		AvatarCreationWidget_Reference->IsEditingExistingAvatar = true;
		AvatarCreationWidget_Reference->PopulateDropDownsWithAvatarData(AvatarData);
		AvatarCreationWidget_Reference->AddToViewport();

		// Bind delegate in the AvatarLibrary
		if (Cast<UWidget_AvatarLibrary>(PairedWidget))
			Cast<UWidget_AvatarLibrary>(PairedWidget)->BindAvatarCreatedDelegate(this);
	}
}