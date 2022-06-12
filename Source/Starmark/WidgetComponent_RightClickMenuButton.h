#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "WidgetComponent_RightClickMenu.h"

#include "WidgetComponent_RightClickMenuButton.generated.h"



UCLASS()
class STARMARK_API UWidgetComponent_RightClickMenuButton : public UUserWidget
{
	GENERATED_BODY()
	
public:
// Variables
// --------------------------------------------------

// ------------------------- Components
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	UButton* RightClickMenuButton;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	UTextBlock* ButtonLabel;

// ------------------------- References
	UPROPERTY()
	UWidgetComponent_RightClickMenu* RightClickMenuWidget;

// -------------------------
	UPROPERTY()
	E_RightClickMenu_Commands Command;

// Functions
// --------------------------------------------------

// ------------------------- 
	UFUNCTION(BlueprintCallable)
	void OnButtonClicked();

	UFUNCTION()
	void EquipAvatar();

	UFUNCTION()
	void UnequipAvatar();

	UFUNCTION()
	void DeleteAvatar();
};
