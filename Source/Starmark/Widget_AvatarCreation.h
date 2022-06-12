#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"
#include "Engine/DataTable.h"
#include "EnterTheNetscape_Variables.h"
#include "Widgets/Input/SComboBox.h"

#include "Widget_AvatarCreation.generated.h"


// Forward Declarations
class AEnterTheNetscape_PlayerState;


// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAvatarCreated);


UCLASS()
class STARMARK_API UWidget_AvatarCreation : public UUserWidget
{
	GENERATED_BODY()

public:

// Variables
// --------------------------------------------------

// ------------------------- Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UComboBoxString* SpeciesDropDown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UComboBoxString* AbilityDropDown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UComboBoxString* AccessoryDropDown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UComboBoxString* MoveOneDropDown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UComboBoxString* MoveTwoDropDown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UComboBoxString* MoveThreeDropDown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UComboBoxString* MoveFourDropDown;

	UPROPERTY(meta = (BindWidget))
	UEditableText* AvatarNameEditableText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AvatarNameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SpeciesText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ElementsText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AbilityText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AccessoryText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MoveOneNameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MoveTwoNameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MoveThreeNameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MoveFourNameText;

	UPROPERTY(meta = (BindWidget))
	UButton* SaveButton;

// ------------------------- References
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* AvatarDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* SimpleAttacksDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* ComplexAttacksDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* AbilitiesDataTable;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* ItemsDataTable;

	UPROPERTY()
	TArray<FName> AvatarRowNames;

	UPROPERTY()
	TArray<FName> SimpleAttacksRowNames;

	UPROPERTY()
	TArray<FName> ComplexAttacksRowNames;

	UPROPERTY()
	TArray<FName> AbilityRowNames;

	UPROPERTY()
	TArray<FName> ItemRowNames;

	UPROPERTY()
	AEnterTheNetscape_PlayerState* PlayerStateReference;

	UPROPERTY()
	FString ContextString;

// ------------------------- Avatar
	UPROPERTY()
	FAvatar_Struct CurrentAvatar;

	UPROPERTY()
	bool IsEditingExistingAvatar = false;

// ------------------------- Delegates
	UPROPERTY()
	FAvatarCreated OnAvatarCreatedDelegate;

// Functions
// --------------------------------------------------

// ------------------------- Widget
	UFUNCTION(BlueprintCallable)
	void PopulateDropDowns();

	UFUNCTION()
	void PopulateDropDownsWithAvatarData(FAvatar_Struct Avatar);

	UFUNCTION(BlueprintCallable)
	void OnAvatarNicknameTextChanged(FText NewNickname);

	UFUNCTION(BlueprintCallable)
	void OnSpeciesDropDownChanged(FString Option);

	UFUNCTION(BlueprintCallable)
	void OnAbilityDropDownChanged(FString Option);

	UFUNCTION(BlueprintCallable)
	void OnAccessoryDropDownChanged(FString Option);

	UFUNCTION(BlueprintCallable)
	void OnMoveOneDropDownChanged(FString Option);

	UFUNCTION(BlueprintCallable)
	void OnMoveTwoDropDownChanged(FString Option);

	UFUNCTION(BlueprintCallable)
	void OnMoveThreeDropDownChanged(FString Option);

	UFUNCTION(BlueprintCallable)
	void OnMoveFourDropDownChanged(FString Option);

	UFUNCTION(BlueprintCallable)
	void OnSaveButtonPressed();

	UFUNCTION(BlueprintCallable)
	void OnExitButtonPressed();
};