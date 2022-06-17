#include "Widget_HUD_Battle.h"


#include "Blueprint/WidgetTree.h"
#include "Character_Pathfinder.h"
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


void UWidget_HUD_Battle::SetUiIconsInTurnOrder(TArray<ACharacter_Pathfinder*> TurnOrderArray, int IndexOfCurrentlyActingEntity)
{
	EntityIconsInTurnOrder->ClearChildren();

	for (ACharacter_Pathfinder* Character : TurnOrderArray) {
		if (Character->AvatarData.UiImages.Num() > 0) {
			UImage* CharacterIcon = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
			CharacterIcon->SetBrushFromTexture(Character->AvatarData.UiImages[0]);
			CharacterIcon->SetBrushSize(FVector2D(100.f, 100.f));
			EntityIconsInTurnOrder->AddChild(CharacterIcon);
		}
	}

	SetCurrentActingEntityInfo(TurnOrderArray[0]);
}


void UWidget_HUD_Battle::SetCurrentActingEntityInfo(ACharacter_Pathfinder* CurrentActingEntity)
{
	if (CurrentActingEntity->AvatarData.UiImages.Num() > 0) {
		CurrentEntityIcon->SetBrushFromTexture(CurrentActingEntity->AvatarData.UiImages[0]);
	}

	HealthText->SetText((FText::FromString("%s / %s"), FText::FromString(FString::FromInt(CurrentActingEntity->AvatarData.CurrentHealthPoints)), FText::FromString(FString::FromInt(CurrentActingEntity->AvatarData.BattleStats.MaximumHealthPoints))));
	ManaText->SetText((FText::FromString("%s / %s"), FText::FromString(FString::FromInt(CurrentActingEntity->AvatarData.CurrentManaPoints)), FText::FromString(FString::FromInt(CurrentActingEntity->AvatarData.BattleStats.MaximumManaPoints))));

	HealthBar->SetPercent(CurrentActingEntity->AvatarData.BattleStats.MaximumHealthPoints / CurrentActingEntity->AvatarData.CurrentHealthPoints);
	ManaBar->SetPercent(CurrentActingEntity->AvatarData.BattleStats.MaximumManaPoints / CurrentActingEntity->AvatarData.CurrentManaPoints);
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