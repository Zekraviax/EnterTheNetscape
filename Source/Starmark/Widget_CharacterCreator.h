#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/TextBlock.h"
#include "Components/ComboBoxString.h"

#include "Widget_CharacterCreator.generated.h"


// Forward Declarations
class UEnterTheNetscape_GameInstance;


UCLASS()
class STARMARK_API UWidget_CharacterCreator : public UUserWidget
{
	GENERATED_BODY()

public:
// Base Variables
// --------------------------------------------------

// ------------------------- Components
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* Birthday_Day_DropDown;
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* Birthday_Month_DropDown;
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* Birthday_Year_DropDown;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HoroscopeText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MarkText;

// ------------------------- References
	UPROPERTY()
	UEnterTheNetscape_GameInstance* GameInstanceReference;

// ------------------------- Other
	UPROPERTY()
	int BirthDayAsNumber;
	UPROPERTY()
	int BirthMonthAsNumber;
	UPROPERTY()
	int BirthYearAsNumber;

	UPROPERTY()
	FString BirthYearAsString;
	UPROPERTY()
	FString BirthYearAsString2;

	UPROPERTY()
	TArray<FString> BirthDayAsCharArray;
	UPROPERTY()
	TArray<FString> BirthMonthAsCharArray;
	UPROPERTY()
	TArray<FString> BirthYearAsCharArray;

	UPROPERTY()
	int MarkNumberAsInt;
	UPROPERTY()
	TArray<int> MarkNumberAsIntArray;
	UPROPERTY()
	FString MarkNumberAsString;
	UPROPERTY()
	TArray<FString> MarkNumberAsCharArray;

// Function
// --------------------------------------------------

// ------------------------- Widget
	UFUNCTION(BlueprintCallable)
	void OnWidgetOpened();

// ------------------------- Player
	UFUNCTION(BlueprintCallable)
	void CalculateHoroscope();

	UFUNCTION(BlueprintCallable)
	void CalculateMark();
};
