#include "Widget_HUD_Battle.h"


#include "Kismet/GameplayStatics.h"
#include "PlayerController_Battle.h"
#include "WidgetComponent_AvatarAttack.h"


// ------------------------- Widget
void UWidget_HUD_Battle::UpdateAvatarAttacksComponents()
{
	if (IsValid(this)) {
		if (AvatarAttacksBox) {
			for (int i = 0; i < AvatarAttacksBox->GetChildrenCount(); i++) {
				if (PlayerControllerReference->CurrentSelectedAvatar->CurrentKnownAttacks.IsValidIndex(i)) {
					if (PlayerControllerReference->CurrentSelectedAvatar->CurrentKnownAttacks[i].Name == "Default" ||
						PlayerControllerReference->CurrentSelectedAvatar->CurrentKnownAttacks[i].Name == "None") {
						AvatarAttacksBox->GetChildAt(i)->SetVisibility(ESlateVisibility::Hidden);
					} else {
						Cast<UWidgetComponent_AvatarAttack>(AvatarAttacksBox->GetChildAt(i))->AttackNameText->SetText(FText::FromString(PlayerControllerReference->CurrentSelectedAvatar->CurrentKnownAttacks[i].Name));

						Cast<UWidgetComponent_AvatarAttack>(AvatarAttacksBox->GetChildAt(i))->PlayerControllerReference = PlayerControllerReference;
						Cast<UWidgetComponent_AvatarAttack>(AvatarAttacksBox->GetChildAt(i))->AvatarAttackIndex = i;

						AvatarAttacksBox->GetChildAt(i)->SetVisibility(ESlateVisibility::Visible);
					}
				} else {
					AvatarAttacksBox->GetChildAt(i)->SetVisibility(ESlateVisibility::Hidden);
				}
			}
		}
	}
}


void UWidget_HUD_Battle::UpdateTurnOrderText(FString NewText)
{
	if (IsValid(this)) {
		if (TurnOrderTextBlock->IsValidLowLevel())
			TurnOrderTextBlock->SetText(FText::FromString("Turn Order:\n" + NewText));

		UpdateAvatarAttacksComponents();
	}
}


// ------------------------- Commands
void UWidget_HUD_Battle::MoveCommand()
{
	PlayerControllerReference->PlayerClickMode = E_PlayerCharacter_ClickModes::E_MoveCharacter;
}


void UWidget_HUD_Battle::EndCommand()
{
	PlayerControllerReference->SendEndOfTurnCommandToServer_Implementation();
}