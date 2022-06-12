#include "WidgetComponent_PlayerProfile.h"

#include "Player_SaveData.h"
#include "Kismet/GameplayStatics.h"
#include "Widget_PlayerProfileCreator.h"


// ------------------------- Components
void UWidgetComponent_PlayerProfile::OnSelectProfileButtonPressed()
{
	//UPlayer_SaveData* ReturnProfile = Cast<UPlayer_SaveData>(UGameplayStatics::LoadGameFromSlot(ProfileNameText->GetText().ToString(), 0));

	// Set the profile in the PlayerStateReference in blueprints (for now)

	//return ReturnProfile;
	if (OnPlayerProfileLoadedDelegate.IsBound())
		OnPlayerProfileLoadedDelegate.Broadcast();
}