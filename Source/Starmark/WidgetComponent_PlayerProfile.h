#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/TextBlock.h"

#include "WidgetComponent_PlayerProfile.generated.h"


// Forward Declarations
class UPlayer_SaveData;


// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerProfileLoaded);


UCLASS()
class STARMARK_API UWidgetComponent_PlayerProfile : public UUserWidget
{
	GENERATED_BODY()

public:
// Variables
// --------------------------------------------------

// ------------------------- Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ProfileNameText;

// ------------------------- Save Data
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ProfileName;

// ------------------------- Delegates
	UPROPERTY()
	FPlayerProfileLoaded OnPlayerProfileLoadedDelegate;


// Functions
// --------------------------------------------------

// ------------------------- Components
	UFUNCTION(BlueprintCallable)
	void OnSelectProfileButtonPressed();
};