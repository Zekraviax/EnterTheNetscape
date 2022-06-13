#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Materials/Material.h"
#include "EnterTheNetscape_Variables.h"

#include "WidgetComponent_Avatar.generated.h"


// Forward Declarations
class UWidget_AvatarCreation;
class UWidgetComponent_RightClickMenu;


// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAvatarSelected);


// Unique Enums
UENUM(BlueprintType)
enum class E_AvatarWidgetComponent_Function : uint8
{
	E_Nothing,
	E_CreateNewAvatarInLibrary,
	E_AddAvatarToChosenSlot,
};


UCLASS()
class STARMARK_API UWidgetComponent_Avatar : public UUserWidget
{
	GENERATED_BODY()

public:

// Variables
// --------------------------------------------------

// ------------------------- Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* NetscapeExplorerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* AvatarImage;

// ------------------------- Avatars
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FNetscapeExplorer_Struct AvatarData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterial* AvatarMaterial;

	// 0 - 3 for Active Avatars, 4 - 5 for Reserve Avatars
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int IndexInPlayerTeam = -1;

// ------------------------- References
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UWidget_AvatarCreation> AvatarCreationWidget_Class;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWidget_AvatarCreation* AvatarCreationWidget_Reference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UWidgetComponent_RightClickMenu> RightClickMenu_Class;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWidgetComponent_RightClickMenu* RightClickMenu_Reference;

// ------------------------- Widget
	UPROPERTY()
	UUserWidget* PairedWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	E_AvatarWidgetComponent_Function CurrentFunction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<E_RightClickMenu_Commands> RightClickMenuCommands;

// ------------------------- Delegates
	UPROPERTY()
	FAvatarSelected OnAvatarSelectedDelegate;


// Functions
// --------------------------------------------------

// ------------------------- Widget
	UFUNCTION(BlueprintCallable)
	void OnInteractButtonPressed();

	UFUNCTION(BlueprintCallable)
	void OnRightMouseButtonDown();

	UFUNCTION(BlueprintNativeEvent)
	void UpdateWidgetMaterials();

	UFUNCTION()
	void ApplyNewAvatarData(FNetscapeExplorer_Struct NewAvatarData);

// ------------------------- Right Click Menu
	UFUNCTION()
	void RightClickMenuFunction_EditAvatar();

	//UFUNCTION()
	//void RightClickMenuFunction_EquipAvatar();
};
