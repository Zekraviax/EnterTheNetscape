#include "WidgetComponent_AvatarAttack.h"


#include "Actor_AttackEffectsLibrary.h"
#include "Actor_GridTile.h"
#include "Character_Pathfinder.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerController_Battle.h"
#include "EnterTheNetscape_GameInstance.h"


// ------------------------- Widget
void UWidgetComponent_AvatarAttack::OnButtonPressed()
{
	if (AvatarAttackIndex > -1 && PlayerControllerReference && PlayerControllerReference->CurrentSelectedAvatar->CurrentKnownAttacks.IsValidIndex(AvatarAttackIndex)) {
		PlayerControllerReference->PlayerClickMode = E_PlayerCharacter_ClickModes::E_SelectCharacterToAttack;
		PlayerControllerReference->TileHighlightMode = E_PlayerCharacter_HighlightModes::E_AttackPattern;

		// Set selected Attack
		PlayerControllerReference->CurrentSelectedAvatar->CurrentSelectedAttack = PlayerControllerReference->CurrentSelectedAvatar->CurrentKnownAttacks[AvatarAttackIndex];

		// Get new valid targets
		PlayerControllerReference->CurrentSelectedAvatar->ShowAttackRange();

		// Enable rotation towards mouse cursor
		PlayerControllerReference->CurrentSelectedAvatar->RotateAvatarTowardsMouse = true;
	}
}