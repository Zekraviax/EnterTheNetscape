#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "Widget_AttackSelectMenu.generated.h"


// Forward Declarations
class ACharacter_Pathfinder;


UCLASS()
class STARMARK_API UWidget_AttackSelectMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
// Variables
// --------------------------------------------------

// ------------------------- Avatar
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ACharacter_Pathfinder* AvatarReference;

// ------------------------- Widgets
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UButton> ButtonClass;


// Functions
// --------------------------------------------------

// ------------------------- Widget
	UFUNCTION()
	void OnWidgetOpened();

};
