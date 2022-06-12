#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/Button.h"
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
	UUniformGridPanel* AvatarAttacksBox;

	UPROPERTY(meta = (BindWidget))
	UWidgetComponent_AvatarBattleData* AvatarBattleDataWidget;

	UPROPERTY(meta = (BindWidget))
	UButton* SwitchCommandButton;

	UPROPERTY(meta = (BindWidget))
	UButton* EndTurnCommandButton;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* CombatLog;

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

// ------------------------- Commands
	UFUNCTION(BlueprintCallable)
	void MoveCommand();

	UFUNCTION(BlueprintCallable)
	void EndCommand();
};
