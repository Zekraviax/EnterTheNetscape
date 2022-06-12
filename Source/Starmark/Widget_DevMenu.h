#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/ComboBoxString.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Materials/Material.h"
#include "Engine/DataTable.h"
#include "Engine/CurveTable.h"
#include "Math/Color.h"

#include "Widget_DevMenu.generated.h"


// Forward Declarations
class AActor_CharacterTest;
class AActor_MorphTargetTest;


// Unique Variables
UENUM(BlueprintType)
enum class E_DevMenu_ColourChangeDropdowns : uint8
{
	E_Colour1,
	E_Colour2,
	E_Colour3,
};


UCLASS()
class STARMARK_API UWidget_DevMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
// Variables
// --------------------------------------------------

// ------------------------- Widget
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* DevSubMenuDropdown;

// ------------------------- Dye-Changing
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ColourOneDropDown;
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ColourTwoDropDown;
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ColourThreeDropDown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dye-Changing")
	TMap<FLinearColor, FString> DyeColorsMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dye-Changing")
	TSoftObjectPtr<AActor> DyeChangeActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dye-Changing")
	TSoftObjectPtr<AActor_CharacterTest> DyeChangeCharacter;

// ------------------------- Type Calculator
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Type Calculator", meta = (BindWidget))
	UComboBoxString* PrimaryTypeDropdown;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Type Calculator", meta = (BindWidget))
	UComboBoxString* SecondaryTypeDropdown;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Type Calculator", meta = (BindWidget))
	UTextBlock* CombinationTypeText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Type Calculator", meta = (BindWidget))
	UUniformGridPanel* TypeTextsGridPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Type Calculator")
	TArray<UComboBoxString*> TypeChartTextBoxes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Type Calculator")
	UCurveTable* MoveEffectiveness_NumberedBaseTypeChartDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Type Calculator")
	UCurveTable* AvatarResistances_NumberedBaseTypeChartDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Type Calculator")
	UDataTable* CombinationTypesDataTable;

// ------------------------- Face Customizer
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* Face_DropDown;
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* Eyes_DropDown;
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* EyeColour_DropDown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Face Customizer")
	UMaterial* PizzaEyes_Material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Face Customizer")
	UMaterial* RoundEyes_Material;

	
// Function
// --------------------------------------------------

// ------------------------- Widget
	UFUNCTION(BlueprintCallable)
	void OnWidgetOpened();

	UFUNCTION(BlueprintCallable)
	void OnDevSubMenuDropdownChanged();

// ------------------------- Dye-Changing
	UFUNCTION(BlueprintCallable)
	void OnColourDropdownChanged(E_DevMenu_ColourChangeDropdowns Dropdown);

// ------------------------- Face Customizer
	UFUNCTION(BlueprintCallable)
	void OnFaceChanged(AActor_MorphTargetTest* MorphTargetTestCharacter);

	UFUNCTION(BlueprintCallable)
	void OnEyesChanged(AActor_MorphTargetTest* MorphTargetTestCharacter);
};