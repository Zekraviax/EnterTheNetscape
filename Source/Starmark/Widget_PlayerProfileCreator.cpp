#include "Widget_PlayerProfileCreator.h"

#include "Components/TextBlock.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Player_SaveData.h"
#include "SaveData_PlayerProfilesList.h"
#include "EnterTheNetscape_GameInstance.h"
#include "EnterTheNetscape_PlayerState.h"
#include "EnterTheNetscape_Variables.h"
#include "Widget_AvatarLibrary.h"
#include "Widget_MainMenu.h"
#include "WidgetComponent_PlayerProfile.h"


// ------------------------- Widget
void UWidget_PlayerProfileCreator::OnWidgetOpened()
{
	AEnterTheNetscape_PlayerState* PlayerStateReference = Cast<AEnterTheNetscape_PlayerState>(GetOwningPlayerState());
	ProfilesList = Cast<USaveData_PlayerProfilesList>(UGameplayStatics::LoadGameFromSlot("PlayerProfilesList", 0));

	// Get the list of Player Profiles
	if (!ProfilesList->IsValidLowLevel()) {
		ProfilesList = Cast<USaveData_PlayerProfilesList>(UGameplayStatics::CreateSaveGameObject(USaveData_PlayerProfilesList::StaticClass()));
		UGameplayStatics::SaveGameToSlot(ProfilesList, "PlayerProfilesList", 0);
	}

	// Populate the scroll box with the players' profiles.
	if (ProfilesList->IsValidLowLevel() && PlayerProfilesScrollBox->IsValidLowLevel()) {
		PlayerProfilesScrollBox->ClearChildren();

		for (int i = 0; i < ProfilesList->PlayerProfileNames.Num(); i++) {
			PlayerProfileWidgetComponent_Reference = CreateWidget<UWidgetComponent_PlayerProfile>(this, PlayerProfileWidgetComponent_Class);

			PlayerProfileWidgetComponent_Reference->ProfileName = ProfilesList->PlayerProfileNames[i];
			PlayerProfileWidgetComponent_Reference->ProfileNameText->SetText(FText::FromString(ProfilesList->PlayerProfileNames[i]));

			//PlayerProfileWidgetComponent_Reference->ProfileName = ProfilesList->PlayerProfileNames[i];
			//const TCHAR* ProfileNameChar = *;
			//PlayerProfileWidgetComponent_Reference->ProfileNameWithStars = PlayerProfileWidgetComponent_Reference->ProfileNameWithStars.Replace(TEXT("[ProfileName]"), *PlayerProfileWidgetComponent_Reference->ProfileName);

			// Bind delegate
			PlayerProfileWidgetComponent_Reference->OnPlayerProfileLoadedDelegate.AddDynamic(this, &UWidget_PlayerProfileCreator::OnPlayerProfileLoadedDelegateBroadcast);

			PlayerProfilesScrollBox->AddChild(PlayerProfileWidgetComponent_Reference);
		}
	}

	// Only enable the Avatar Library if the player has a profile that the library can be saved to
	AvatarLibraryButton->SetIsEnabled(PlayerStateReference->PlayerProfileReference->IsValidLowLevel());
}


void UWidget_PlayerProfileCreator::OnSaveGameButtonPressed()
{
	UPlayer_SaveData* PlayerProfileData = Cast<UPlayer_SaveData>(UGameplayStatics::LoadGameFromSlot(NewProfileNameEntryField->GetText().ToString(), 0));

	if (!PlayerProfileData)
		PlayerProfileData = Cast<UPlayer_SaveData>(UGameplayStatics::CreateSaveGameObject(UPlayer_SaveData::StaticClass()));

	// Profile variables
	// Profile
	PlayerProfileData->ProfileName = NewProfileNameEntryField->GetText().ToString();
	// Player
	PlayerProfileData->Name = NewPlayerNameEntryField->GetText().ToString();

	UGameplayStatics::SaveGameToSlot(PlayerProfileData, NewProfileNameEntryField->GetText().ToString(), 0);

	// Add this profile to the ProfilesList if it isn't already there
	ProfilesList = Cast<USaveData_PlayerProfilesList>(UGameplayStatics::LoadGameFromSlot("PlayerProfilesList", 0));

	if (!ProfilesList->IsValidLowLevel())
		ProfilesList = Cast<USaveData_PlayerProfilesList>(UGameplayStatics::CreateSaveGameObject(USaveData_PlayerProfilesList::StaticClass()));

	if (ProfilesList->IsValidLowLevel()) {
		ProfilesList->PlayerProfileNames.AddUnique(NewProfileNameEntryField->GetText().ToString());
		UGameplayStatics::SaveGameToSlot(ProfilesList, "PlayerProfilesList", 0);
	}

	OnWidgetOpened();
}


void UWidget_PlayerProfileCreator::OnExitButtonPressed()
{
	if (!MainMenuWidget_Reference && MainMenuWidget_Class)
		MainMenuWidget_Reference = CreateWidget<UWidget_MainMenu>(this, MainMenuWidget_Class);

	if (MainMenuWidget_Reference) {
		MainMenuWidget_Reference->AddToViewport();
		this->RemoveFromParent();
	}
}


void UWidget_PlayerProfileCreator::PopulateTeamCreatorDropdowns()
{
	if (AvatarDataTable) {
		TArray<FNetscapeExplorer_Struct> AvatarsArray;
		FString ContextString;
		TArray<FName> RowNames;
		RowNames = AvatarDataTable->GetRowNames();

		for (auto& Name : RowNames) {
			if (Name != "BigBoxBoy") {
				FNetscapeExplorer_Struct* Avatar = AvatarDataTable->FindRow<FNetscapeExplorer_Struct>(Name, ContextString);

				if (Avatar)
					AvatarsArray.Add(*Avatar);
			}
		}
	}
}


void UWidget_PlayerProfileCreator::OnAvatarLibraryButtonPressed()
{
	if (AvatarLibrary_Class->IsValidLowLevel()) {
		AvatarLibrary_Reference = CreateWidget<UWidget_AvatarLibrary>(this, AvatarLibrary_Class);
		AvatarLibrary_Reference->OnWidgetOpened();
		AvatarLibrary_Reference->AddToViewport();
	}
}


// ------------------------- Delegates
void UWidget_PlayerProfileCreator::OnPlayerProfileLoadedDelegateBroadcast()
{
	AEnterTheNetscape_PlayerState* PlayerStateReference = Cast<AEnterTheNetscape_PlayerState>(GetOwningPlayerState());
	AvatarLibraryButton->SetIsEnabled(PlayerStateReference->PlayerProfileReference->IsValidLowLevel());
}