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

	for (int i = 0; i < PlayerStateReference->PlayerProfileReference->CurrentAvatarTeam.Num(); i++) {
		if (PlayerStateReference->PlayerProfileReference->CurrentAvatarTeam[i].AvatarName != "None" &&
			PlayerStateReference->PlayerProfileReference->CurrentAvatarTeam[i].AvatarName != "Default")
			OccupiedIndices.Add(PlayerStateReference->PlayerProfileReference->CurrentAvatarTeam[i].IndexInPlayerLibrary);
	}

	for (int j = 5; j >= 0; j--) {
		if (!OccupiedIndices.Contains(j))
			FirstEmptyIndex = j;
	}

	if (FirstEmptyIndex < 6) {
		// Get the avatar
		FAvatar_Struct ChosenAvatar = Cast<UWidgetComponent_Avatar>(RightClickMenuWidget->OwnerWidget)->AvatarData;
		ChosenAvatar.IndexInPlayerLibrary = FirstEmptyIndex;

		PlayerStateReference->PlayerProfileReference->CurrentAvatarTeam.Insert(ChosenAvatar, FirstEmptyIndex);
		PlayerStateReference->PlayerProfileReference->AvatarLibrary.Remove(ChosenAvatar);

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

	for (int i = 0; i < PlayerStateReference->PlayerProfileReference->AvatarLibrary.Num(); i++) {
		if (PlayerStateReference->PlayerProfileReference->AvatarLibrary[i].IndexInPlayerLibrary < FirstEmptyIndex) {
			PlayerStateReference->PlayerProfileReference->AvatarLibrary[i].IndexInPlayerLibrary = FirstEmptyIndex;
		}
	}

	FAvatar_Struct ChosenAvatar = Cast<UWidgetComponent_Avatar>(RightClickMenuWidget->OwnerWidget)->AvatarData;
	ChosenAvatar.IndexInPlayerLibrary = FirstEmptyIndex;

	PlayerStateReference->PlayerProfileReference->CurrentAvatarTeam.Remove(ChosenAvatar);
	PlayerStateReference->PlayerProfileReference->AvatarLibrary.Add(ChosenAvatar);

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
		PlayerStateReference->PlayerProfileReference->CurrentAvatarTeam.Remove(AvatarSlot->AvatarData);
	} else {
		PlayerStateReference->PlayerProfileReference->AvatarLibrary.Remove(AvatarSlot->AvatarData);
	}

	AvatarSlot->AvatarData = FAvatar_Struct();

	// Update all Avatar widgets
	for (int i = 0; i < FoundAvatarLibraryWidgets.Num(); i++) {
		Cast<UWidget_AvatarLibrary>(FoundAvatarLibraryWidgets[i])->OnWidgetOpened();
		Cast<UWidget_AvatarLibrary>(FoundAvatarLibraryWidgets[i])->UpdateAllAvatarsInTeam();
	}

	PlayerStateReference->SaveToCurrentProfile();
}