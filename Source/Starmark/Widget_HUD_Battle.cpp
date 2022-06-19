#include "Widget_HUD_Battle.h"


#include "Blueprint/WidgetTree.h"
#include "Character_Pathfinder.h"
#include "Containers/UnrealString.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerController_Battle.h"
#include "WidgetComponent_AvatarAttack.h"
//#include "Widget"


// ------------------------- Widget
void UWidget_HUD_Battle::UpdateAvatarAttacksComponents()
{
	if (IsValid(this)) {
		if (AvatarAttacksBox->IsValidLowLevel()) {
			for (int i = 0; i < AvatarAttacksBox->GetChildrenCount(); i++) {
				/*
				if (Cast<>()) {

				}
				*/

				if (PlayerControllerReference->CurrentSelectedAvatar->CurrentKnownAttacks.IsValidIndex(i)) {
					if (PlayerControllerReference->CurrentSelectedAvatar->CurrentKnownAttacks[i].Name == "Default" ||
						PlayerControllerReference->CurrentSelectedAvatar->CurrentKnownAttacks[i].Name == "None") {
						AvatarAttacksBox->GetChildAt(i)->SetVisibility(ESlateVisibility::Hidden);
					} else {
						Cast<UWidgetComponent_AvatarAttack>(AvatarAttacksBox->GetChildAt(i))->AttackNameText->SetText(FText::FromString(PlayerControllerReference->CurrentSelectedAvatar->CurrentKnownAttacks[i].Name.ToUpper()));

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

	for (int i = 1; i < TurnOrderArray.Num(); i++) {
		ACharacter_Pathfinder* Character = TurnOrderArray[i];
		if (Character->AvatarData.UiImages.Num() > 0) {
			UImage* CharacterIcon = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
			CharacterIcon->SetBrushFromTexture(Character->AvatarData.UiImages[0]);
			CharacterIcon->SetBrushSize(FVector2D(125.f, 125.f));
			CharacterIcon->SetBrushTintColor(FSlateColor(FLinearColor(1.f, 1.f, 1.f, (1 - (i * 0.15)))));
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

	HealthText->SetText(FText::FromString(FString::FromInt(CurrentActingEntity->AvatarData.CurrentHealthPoints) + " / " + FString::FromInt(CurrentActingEntity->AvatarData.BattleStats.MaximumHealthPoints)));
	ManaText->SetText(FText::FromString(FString::FromInt(CurrentActingEntity->AvatarData.CurrentManaPoints) + " / " + FString::FromInt(CurrentActingEntity->AvatarData.BattleStats.MaximumManaPoints)));

	HealthBar->SetPercent(float(CurrentActingEntity->AvatarData.CurrentHealthPoints) / float(CurrentActingEntity->AvatarData.BattleStats.MaximumHealthPoints));
	ManaBar->SetPercent(float(CurrentActingEntity->AvatarData.CurrentManaPoints) / float(CurrentActingEntity->AvatarData.BattleStats.MaximumManaPoints));

	CurrentEntityNameText->SetText(FText::FromString(CurrentActingEntity->AvatarData.Nickname.ToUpper() + "'S TURN"));
}


void UWidget_HUD_Battle::ResetBattleHud()
{
	CommandsBox->SetVisibility(ESlateVisibility::Visible);
	AvatarAttacksBox->SetVisibility(ESlateVisibility::Collapsed);
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


void UWidget_HUD_Battle::AttackCommand()
{
	CommandsBox->SetVisibility(ESlateVisibility::Collapsed);
	AvatarAttacksBox->SetVisibility(ESlateVisibility::Visible);
}