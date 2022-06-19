#include "WidgetComponent_CommandButton.h"


#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Widget_HUD_Battle.h"


void UWidgetComponent_CommandButton::AttackCommand()
{
	TArray<UUserWidget*> FoundBattleHudWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(this, FoundBattleHudWidgets, UWidget_HUD_Battle::StaticClass(), true);
	Cast<UWidget_HUD_Battle>(FoundBattleHudWidgets[0])->AttackCommand();
}