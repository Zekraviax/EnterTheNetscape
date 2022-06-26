#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "WidgetComponent_AvatarAttack.generated.h"


// Forward Declarations
class ACharacter_Pathfinder;
class APlayerController_Battle;


UCLASS()
class STARMARK_API UWidgetComponent_AvatarAttack : public UUserWidget
{
	GENERATED_BODY()

public:
// Variables
// --------------------------------------------------

// ------------------------- Components
	UPROPERTY(meta = (BindWidget))
	UButton* SelectButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AttackNameText;

// ------------------------- References
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	APlayerController_Battle* PlayerControllerReference;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int AvatarAttackIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SlotNumber;

// Functions
// --------------------------------------------------

// ------------------------- Widget
	UFUNCTION(BlueprintCallable)
	void OnButtonPressed();
};
