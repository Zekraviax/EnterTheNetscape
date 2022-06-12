#include "Widget_AvatarLibrary.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/PanelWidget.h"
#include "Components/UniformGridSlot.h"
#include "Kismet/GameplayStatics.h"
#include "EnterTheNetscape_GameInstance.h"
#include "EnterTheNetscape_PlayerState.h"
#include "Widget_AvatarCreation.h"
#include "WidgetComponent_Avatar.h"


// ------------------------- Widget
void UWidget_AvatarLibrary::OnWidgetOpened()
{
	UEnterTheNetscape_GameInstance* GameInstanceReference = Cast<UEnterTheNetscape_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	AEnterTheNetscape_PlayerState* PlayerStateReference = Cast<AEnterTheNetscape_PlayerState>(GetOwningPlayerState());
	UWidgetComponent_Avatar* AvatarWidgetComponent_Reference = nullptr;
	UWidgetTree* LibraryWidgetTree = this->WidgetTree;
	TArray<UWidget*> FoundChildWidgetComponents;
	int Column = -1;
	int Row = 0;

	if (PlayerStateReference->PlayerProfileReference && AvatarWidgetComponent_Class) {
		// Populate Avatar Team Slots
		FoundChildWidgetComponents = Cast<UPanelWidget>(LibraryWidgetTree->RootWidget)->GetAllChildren();

		// Add Avatars to the team slots, making sure that the index in the players team is preserved
		for (int i = 0; i < PlayerStateReference->PlayerProfileReference->CurrentAvatarTeam.Num(); i++) {
			for (int j = 0; j < FoundChildWidgetComponents.Num(); j++) {
				if (Cast<UWidgetComponent_Avatar>(FoundChildWidgetComponents[j]) ) {
					AvatarWidgetComponent_Reference = Cast<UWidgetComponent_Avatar>(FoundChildWidgetComponents[j]);
					if (AvatarWidgetComponent_Reference->IndexInPlayerTeam == i) {
						if (PlayerStateReference->PlayerProfileReference->CurrentAvatarTeam[i].IndexInPlayerLibrary != i)
							PlayerStateReference->PlayerProfileReference->CurrentAvatarTeam[i].IndexInPlayerLibrary = i;

						AvatarWidgetComponent_Reference->PairedWidget = this;
						AvatarWidgetComponent_Reference->ApplyNewAvatarData(PlayerStateReference->PlayerProfileReference->CurrentAvatarTeam[i]);

						AvatarWidgetComponent_Reference->CurrentFunction = E_AvatarWidgetComponent_Function::E_AddAvatarToChosenSlot;

						AvatarWidgetComponent_Reference->RightClickMenuCommands.Empty();

						if (AvatarWidgetComponent_Reference->AvatarData.AvatarName != "None") {
							AvatarWidgetComponent_Reference->RightClickMenuCommands.Add(E_RightClickMenu_Commands::EditAvatar);
							AvatarWidgetComponent_Reference->RightClickMenuCommands.Add(E_RightClickMenu_Commands::UnequipAvatar);
							AvatarWidgetComponent_Reference->RightClickMenuCommands.Add(E_RightClickMenu_Commands::DeleteAvatar);
						}
						AvatarWidgetComponent_Reference->RightClickMenuCommands.Add(E_RightClickMenu_Commands::Cancel);

						break;
					}
				}
			}
		}

		// Clear old widgets
		AvatarLibraryUniformGridPanel->ClearChildren();

		// Populate the Avatar Library (Avatars not currently in the players' team)
		for (int i = 0; i < PlayerStateReference->PlayerProfileReference->AvatarLibrary.Num(); i++) {
			AvatarWidgetComponent_Reference = CreateWidget<UWidgetComponent_Avatar>(this, AvatarWidgetComponent_Class);
			AvatarWidgetComponent_Reference->ApplyNewAvatarData(PlayerStateReference->PlayerProfileReference->AvatarLibrary[i]);

			Column++;
			if (Column >= 4) {
				Column = 0;
				Row++;
			}

			AvatarWidgetComponent_Reference->PairedWidget = this;
			AvatarWidgetComponent_Reference->CurrentFunction = E_AvatarWidgetComponent_Function::E_AddAvatarToChosenSlot;

			AvatarWidgetComponent_Reference->RightClickMenuCommands.Add(E_RightClickMenu_Commands::EquipAvatar);
			AvatarWidgetComponent_Reference->RightClickMenuCommands.Add(E_RightClickMenu_Commands::EditAvatar);
			AvatarWidgetComponent_Reference->RightClickMenuCommands.Add(E_RightClickMenu_Commands::DeleteAvatar);
			AvatarWidgetComponent_Reference->RightClickMenuCommands.Add(E_RightClickMenu_Commands::Cancel);

			AvatarLibraryUniformGridPanel->AddChildToUniformGrid(AvatarWidgetComponent_Reference, Row, Column);
		}

		// Create one move Avatar WidgetComponent that's used to add new Avatars to the Library
		AvatarWidgetComponent_Reference = CreateWidget<UWidgetComponent_Avatar>(this, AvatarWidgetComponent_Class);
		AvatarWidgetComponent_Reference->PairedWidget = this;
		AvatarWidgetComponent_Reference->CurrentFunction = E_AvatarWidgetComponent_Function::E_CreateNewAvatarInLibrary;

		Column++;
		if (Column >= 4) {
			Column = 0;
			Row++;
		}

		AvatarWidgetComponent_Reference->UpdateWidgetMaterials();
		AvatarWidgetComponent_Reference->AvatarName->SetText(FText::FromString("Create New"));
		AvatarLibraryUniformGridPanel->AddChildToUniformGrid(AvatarWidgetComponent_Reference, Row, Column);
	}

	// Add hidden children to enforce the layout of the grid panel, if there are less than four children
	for (int k = 0; k < 3; k++) {
		Column++;
		if (Column >= 4) {
			Column = 0;
			Row++;
		}

		AvatarWidgetComponent_Reference = CreateWidget<UWidgetComponent_Avatar>(this, AvatarWidgetComponent_Class);
		AvatarWidgetComponent_Reference->SetVisibility(ESlateVisibility::Hidden);
		AvatarLibraryUniformGridPanel->AddChildToUniformGrid(AvatarWidgetComponent_Reference, Row, Column);
	}

	// Adjust all children in the Uniform Grid Panel
	for (int j = 0; j < AvatarLibraryUniformGridPanel->GetChildrenCount(); j++) {
		Cast<UUniformGridSlot>(AvatarLibraryUniformGridPanel->GetChildAt(j)->Slot)->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
	}
}


void UWidget_AvatarLibrary::UpdateAllAvatarsInTeam()
{
	UWidgetTree* LibraryWidgetTree = this->WidgetTree;
	TArray<UWidget*> FoundChildWidgetComponents;
	TArray<UTexture*> QuestionMarkMaterials;
	AEnterTheNetscape_PlayerState* PlayerStateReference = Cast<AEnterTheNetscape_PlayerState>(GetOwningPlayerState());
	UWidgetComponent_Avatar* AvatarWidgetComponent_Reference = nullptr;
	FoundChildWidgetComponents = Cast<UPanelWidget>(LibraryWidgetTree->RootWidget)->GetAllChildren();
	QuestionMarkMaterial->GetUsedTextures(QuestionMarkMaterials, EMaterialQualityLevel::Medium, true, ERHIFeatureLevel::ES3_1, true);

	for (int j = 0; j < FoundChildWidgetComponents.Num(); j++) {
		if (Cast<UWidgetComponent_Avatar>(FoundChildWidgetComponents[j])) {
			AvatarWidgetComponent_Reference = Cast<UWidgetComponent_Avatar>(FoundChildWidgetComponents[j]);

			if (AvatarWidgetComponent_Reference->IndexInPlayerTeam >= 0) {
				bool FoundAvatarInSlot = false;
				for (int k = 0; k < PlayerStateReference->PlayerProfileReference->CurrentAvatarTeam.Num(); k++) {
					if (PlayerStateReference->PlayerProfileReference->CurrentAvatarTeam[k].IndexInPlayerLibrary == AvatarWidgetComponent_Reference->IndexInPlayerTeam) {
						FoundAvatarInSlot = true;
						AvatarWidgetComponent_Reference->AvatarData = PlayerStateReference->PlayerProfileReference->CurrentAvatarTeam[k];
						break;
					}
				}

				AvatarWidgetComponent_Reference->RightClickMenuCommands.Empty();	

				if (FoundAvatarInSlot) {
					AvatarWidgetComponent_Reference->RightClickMenuCommands.Add(E_RightClickMenu_Commands::EditAvatar);
					AvatarWidgetComponent_Reference->RightClickMenuCommands.Add(E_RightClickMenu_Commands::UnequipAvatar);

					AvatarWidgetComponent_Reference->UpdateWidgetMaterials();
				} else {
					AvatarWidgetComponent_Reference->AvatarData = FAvatar_Struct();
					AvatarWidgetComponent_Reference->AvatarName->SetText(FText::FromString("Empty Slot"));
					AvatarWidgetComponent_Reference->AvatarImage->SetBrushFromTexture(Cast<UTexture2D>(QuestionMarkMaterials[0]), true);
				}

				AvatarWidgetComponent_Reference->RightClickMenuCommands.Add(E_RightClickMenu_Commands::Cancel);
			}
		}
	}
}


// ------------------------- Delegates
void UWidget_AvatarLibrary::BindAvatarCreatedDelegate(UWidgetComponent_Avatar* AvatarWidgetComponentReference)
{
	AvatarWidgetComponentReference->AvatarCreationWidget_Reference->OnAvatarCreatedDelegate.AddDynamic(this, &UWidget_AvatarLibrary::OnAvatarCreatedDelegateBroadcast);
}


void UWidget_AvatarLibrary::OnAvatarCreatedDelegateBroadcast()
{
	OnWidgetOpened();
}


void UWidget_AvatarLibrary::OnAvatarChangedSlotDelegateBroadcast()
{
	AEnterTheNetscape_PlayerState* PlayerStateReference = Cast<AEnterTheNetscape_PlayerState>(GetOwningPlayer()->PlayerState);
	UWidgetTree* LibraryWidgetTree = this->WidgetTree;
	TArray<UWidget*> FoundChildWidgetComponents;

	PlayerStateReference->PlayerProfileReference->CurrentAvatarTeam.Empty();

	FoundChildWidgetComponents = Cast<UPanelWidget>(LibraryWidgetTree->RootWidget)->GetAllChildren();

	for (int i = 0; i < FoundChildWidgetComponents.Num(); i++) {
		if (Cast<UWidgetComponent_Avatar>(FoundChildWidgetComponents[i])) {
			UWidgetComponent_Avatar* AvatarWidgetComponentReference = Cast<UWidgetComponent_Avatar>(FoundChildWidgetComponents[i]);
			if (AvatarWidgetComponentReference->IndexInPlayerTeam >= 0) {
				PlayerStateReference->PlayerProfileReference->CurrentAvatarTeam.Insert(AvatarWidgetComponentReference->AvatarData, AvatarWidgetComponentReference->IndexInPlayerTeam);
				PlayerStateReference->PlayerProfileReference->AvatarLibrary.Remove(AvatarWidgetComponentReference->AvatarData);
			}
		}
	}

	PlayerStateReference->SaveToCurrentProfile();
}