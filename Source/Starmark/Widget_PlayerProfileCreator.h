#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableText.h"
#include "Components/ScrollBox.h"
#include "Engine/DataTable.h"

#include "Widget_PlayerProfileCreator.generated.h"


// Forward Declarations
class USaveData_PlayerProfilesList;
class USaveGame;
class UWidgetComponent_PlayerProfile;
class UWidget_MainMenu;
class UWidget_AvatarLibrary;


UCLASS()
class STARMARK_API UWidget_PlayerProfileCreator : public UUserWidget
{
	GENERATED_BODY()

public:
// Variables
// --------------------------------------------------

// ------------------------- Components
	UPROPERTY(meta = (BindWidget))
	UEditableText* NewProfileNameEntryField;

	UPROPERTY(meta = (BindWidget))
	UEditableText* NewPlayerNameEntryField;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* PlayerProfilesScrollBox;

	UPROPERTY(meta = (BindWidget))
	UButton* AvatarLibraryButton;

// ------------------------- References
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UWidget_MainMenu> MainMenuWidget_Class;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWidget_MainMenu* MainMenuWidget_Reference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UWidgetComponent_PlayerProfile> PlayerProfileWidgetComponent_Class;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWidgetComponent_PlayerProfile* PlayerProfileWidgetComponent_Reference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UWidget_AvatarLibrary> AvatarLibrary_Class;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWidget_AvatarLibrary* AvatarLibrary_Reference;

// ------------------------- Save Data
	UPROPERTY()
	USaveData_PlayerProfilesList* ProfilesList;

// ------------------------- Avatar Data
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* AvatarDataTable;


// Functions
// --------------------------------------------------

// ------------------------- Widget
	UFUNCTION(BlueprintCallable)
	void OnWidgetOpened();

	UFUNCTION(BlueprintCallable)
	void OnSaveGameButtonPressed();

	UFUNCTION(BlueprintCallable)
	void OnExitButtonPressed();

	UFUNCTION(BlueprintCallable)
	void PopulateTeamCreatorDropdowns();

	UFUNCTION(BlueprintCallable)
	void OnAvatarLibraryButtonPressed();

// ------------------------- Delegates
	UFUNCTION()
	void OnPlayerProfileLoadedDelegateBroadcast();
};
