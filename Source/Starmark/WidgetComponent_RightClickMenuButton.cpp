#include "WidgetComponent_RightClickMenuButton.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Player_SaveData.h"
#include "EnterTheNetscape_PlayerState.h"
#include "Widget_AvatarLibrary.h"
#include "WidgetComponent_Avatar.h"


void UWidgetComponent_RightClickMenuButton::OnButtonClicked()
{
	RightClickMenuWidget->RemoveFromParent();

	switch (Command)
	{
	case (E_RightClickMenu_Commands::EditAvatar):
		Cast<UWidgetComponent_Avatar>(RightClickMenuWidget->OwnerWidget)->RightClickMenuFunction_EditAvatar();
		break;
	case (E_RightClickMenu_Commands::EquipAvatar):
		EquipAvatar();
		break;
	case (E_RightClickMenu_Commands::UnequipAvatar):
		UnequipAvatar();
		break;
	case (E_RightClickMenu_Commands::DeleteAvatar):
		DeleteAvatar();
		break;
	case (E_RightClickMenu_Commands::Cancel):
		// do nothing except remove this widget.
		break;
	default:
		break;
	}
}


void UWidgetComponent_RightClickMenuButton::EquipAvatar()
{
	int FirstEmptyIndex = 6;
	TArray<int> OccupiedIndices;
	AEnterTheNetscape_PlayerState* PlayerStateReference = Cast<AEnterTheNetscape_PlayerState>(GetOwningPlayerState());

	for (int i = 0; i < PlayerStateReference->PlayerProfileReference->CurrentExplorerTeam.Num(); i++) {
		if (PlayerStateReference->PlayerProfileReference->CurrentExplorerTeam[i].NetscapeExplorerName != "None" &&
			PlayerStateReference->PlayerProfileReference->CurrentExplorerTeam[i].NetscapeExplorerName != "Default")
			OccupiedIndices.Add(PlayerStateReference->PlayerProfileReference->CurrentExplorerTeam[i].IndexInPlayerLibrary);
	}

	for (int j = 5; j >= 0; j--) {
		if (!OccupiedIndices.Contains(j))
			FirstEmptyIndex = j;
	}

	if (FirstEmptyIndex < 6) {
		// Get the avatar
		FNetscapeExplorer_Struct ChosenAvatar = Cast<UWidgetComponent_Avatar>(RightClickMenuWidget->OwnerWidget)->AvatarData;
		ChosenAvatar.IndexInPlayerLibrary = FirstEmptyIndex;

		PlayerStateReference->PlayerProfileReference->CurrentExplorerTeam.Insert(ChosenAvatar, FirstEmptyIndex);
		PlayerStateReference->PlayerProfileReference->Explorers.Remove(ChosenAvatar);

		TArray<UUserWidget*> FoundAvatarLibraryWidgets, FoundAvatarComponents;
		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(this, FoundAvatarLibraryWidgets, UWidget_AvatarLibrary::StaticClass(), true);
		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(this, FoundAvatarComponents, UWidgetComponent_Avatar::StaticClass(), true);

		// Update all Avatar widgets
		for (int i = 0; i < FoundAvatarLibraryWidgets.Num(); i++) {
			Cast<UWidget_AvatarLibrary>(FoundAvatarLibraryWidgets[i])->OnWidgetOpened();
			Cast<UWidget_AvatarLibrary>(FoundAvatarLibraryWidgets[i])->UpdateAllAvatarsInTeam();
		}

		PlayerStateReference->SaveToCurrentProfile();
	}
}


void UWidgetComponent_RightClickMenuButton::UnequipAvatar()
{
	int FirstEmptyIndex = 0;
	TArray<int> OccupiedIndices;
	AEnterTheNetscape_PlayerState* PlayerStateReference = Cast<AEnterTheNetscape_PlayerState>(GetOwningPlayerState());

	for (int i = 0; i < PlayerStateReference->PlayerProfileReference->CurrentExplorerTeam.Num(); i++) {
		if (PlayerStateReference->PlayerProfileReference->CurrentExplorerTeam[i].IndexInPlayerLibrary < FirstEmptyIndex) {
			PlayerStateReference->PlayerProfileReference->CurrentExplorerTeam[i].IndexInPlayerLibrary = FirstEmptyIndex;
		}
	}

	FNetscapeExplorer_Struct ChosenAvatar = Cast<UWidgetComponent_Avatar>(RightClickMenuWidget->OwnerWidget)->AvatarData;
	ChosenAvatar.IndexInPlayerLibrary = FirstEmptyIndex;

	PlayerStateReference->PlayerProfileReference->CurrentExplorerTeam.Remove(ChosenAvatar);
	PlayerStateReference->PlayerProfileReference->Explorers.Add(ChosenAvatar);

	TArray<UUserWidget*> FoundAvatarLibraryWidgets, FoundAvatarComponents;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(this, FoundAvatarLibraryWidgets, UWidget_AvatarLibrary::StaticClass(), true);
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(this, FoundAvatarComponents, UWidgetComponent_Avatar::StaticClass(), true);

	// Update all Avatar widgets
	for (int i = 0; i < FoundAvatarLibraryWidgets.Num(); i++) {
		Cast<UWidget_AvatarLibrary>(FoundAvatarLibraryWidgets[i])->OnWidgetOpened();
		Cast<UWidget_AvatarLibrary>(FoundAvatarLibraryWidgets[i])->UpdateAllAvatarsInTeam();
	}

	PlayerStateReference->SaveToCurrentProfile();
}


void UWidgetComponent_RightClickMenuButton::DeleteAvatar()
{
	TArray<UUserWidget*> FoundAvatarLibraryWidgets;
	AEnterTheNetscape_PlayerState* PlayerStateReference = Cast<AEnterTheNetscape_PlayerState>(GetOwningPlayerState());
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(this, FoundAvatarLibraryWidgets, UWidget_AvatarLibrary::StaticClass(), true);
	UWidgetComponent_Avatar* AvatarSlot = Cast<UWidgetComponent_Avatar>(RightClickMenuWidget->OwnerWidget);

	if (AvatarSlot->IndexInPlayerTeam >= 0) {
		PlayerStateReference->PlayerProfileReference->CurrentExplorerTeam.Remove(AvatarSlot->AvatarData);
	} else {
		PlayerStateReference->PlayerProfileReference->Explorers.Remove(AvatarSlot->AvatarData);
	}

	AvatarSlot->AvatarData = FNetscapeExplorer_Struct();

	// Update all Avatar widgets
	for (int i = 0; i < FoundAvatarLibraryWidgets.Num(); i++) {
		Cast<UWidget_AvatarLibrary>(FoundAvatarLibraryWidgets[i])->OnWidgetOpened();
		Cast<UWidget_AvatarLibrary>(FoundAvatarLibraryWidgets[i])->UpdateAllAvatarsInTeam();
	}

	PlayerStateReference->SaveToCurrentProfile();
}