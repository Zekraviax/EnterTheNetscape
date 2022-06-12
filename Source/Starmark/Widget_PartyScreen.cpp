#include "Widget_PartyScreen.h"


// ------------------------- Widget
void UWidget_PartyScreen::OnWidgetOpened()
{

}


void UWidget_PartyScreen::OnPauseButtonPressed()
{
	this->RemoveFromParent();
}