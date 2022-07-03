#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/Button.h"
#include "Components/GridPanel.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Components/VerticalBox.h"

#include "Widget_HUD_Battle.generated.h"


// Forward Declarations
class APlayerController_Battle;
class UWidgetComponent_AvatarBattleData;


UCLASS()
class STARMARK_API UWidget_HUD_Battle : public UUserWidget
{
	GENERATED_BODY()

public:
// Variables
// --------------------------------------------------

// ------------------------- Components
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TurnOrderTextBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TurnAndRoundCounterTextBlock;

	UPROPERTY(meta = (BindWidget))
	UGridPanel* CommandsBox;

	UPROPERTY(meta = (BindWidget))
	UGridPanel* AvatarAttacksBox;

	UPROPERTY(meta = (BindWidget))
	UWidgetComponent_AvatarBattleData* AvatarBattleDataWidget;

	//UPROPERTY(meta = (BindWidget))
	UButton* SwitchCommandButton;

	//UPROPERTY(meta = (BindWidget))
	UButton* EndTurnCommand;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* CombatLog;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* EntityIconsInTurnOrder;

	UPROPERTY(meta = (BindWidget))
	UImage* CurrentEntityIcon;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* ManaBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HealthText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ManaText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentEntityNameText;

// ------------------------- References
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APlayerController_Battle* PlayerControllerReference;

// Functions
// --------------------------------------------------

// ------------------------- Widget
	UFUNCTION(BlueprintCallable)
	void UpdateAvatarAttacksComponents();

	UFUNCTION(BlueprintCallable)
	void UpdateTurnOrderText(FString NewText);

	UFUNCTION()
	void SetUiIconsInTurnOrder(TArray<ACharacter_Pathfinder*> TurnOrderArray, int IndexOfCurrentlyActingEntity);

	UFUNCTION()
	void SetCurrentActingEntityInfo(ACharacter_Pathfinder* CurrentActingEntity);

	UFUNCTION(BlueprintCallable)
	void ResetBattleHud();

// ------------------------- Commands
	UFUNCTION(BlueprintCallable)
	void MoveCommand();

	UFUNCTION(BlueprintCallable)
	void EndCommand();

	UFUNCTION(BlueprintCallable)
	void AttackCommand();
};
