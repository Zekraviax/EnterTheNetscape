#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/HorizontalBox.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "EnterTheNetscape_Variables.h"
#include "WidgetComponent_StatusEffectIcon.h"

#include "WidgetComponent_AvatarBattleData.generated.h"


// Forward Declarations
class ACharacter_Pathfinder;


UCLASS()
class STARMARK_API UWidgetComponent_AvatarBattleData : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;
	
public:
// Variables
// --------------------------------------------------

// ------------------------- Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* ManaBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* TileMovesBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* AvatarText;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	//UTextBlock* TypesText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* HealthDisplayText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ManaDisplayText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TileMovesDisplayText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* StatusEffectIconsHorizontalBox;

// ------------------------- References
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UWidgetComponent_StatusEffectIcon> StatusEffectIcon_Class;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWidgetComponent_StatusEffectIcon* StatusEffectIcon_Reference;

// ------------------------- Avatar
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAvatar_Struct LinkedAvatar;

// Functions
// --------------------------------------------------

// ------------------------- Avatar
	UFUNCTION(BlueprintCallable)
	void UpdateAvatarData(FAvatar_Struct NewLinkedAvatar);

	UFUNCTION(BlueprintCallable)
	void GetAvatarStatusEffects(TArray<FAvatar_StatusEffect> StatusEffectsArray);
};
