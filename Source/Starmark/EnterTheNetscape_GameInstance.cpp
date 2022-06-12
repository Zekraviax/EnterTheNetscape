#include "EnterTheNetscape_GameInstance.h"

#include "Actor_GridTile.h"
#include "Kismet/GameplayStatics.h"
#include "SaveData_PlayerProfilesList.h"


// ------------------------- Player
void UEnterTheNetscape_GameInstance::LoadProfile(FString ProfileName)
{
	USaveData_PlayerProfilesList* SaveGameObject = Cast<USaveData_PlayerProfilesList>(UGameplayStatics::LoadGameFromSlot("PlayerProfilesList", 0));

	for (int i = 0; i < SaveGameObject->PlayerProfileNames.Num(); i++) {
		if (SaveGameObject->PlayerProfileNames[i] == ProfileName) {
			CurrentProfileReference = Cast<UPlayer_SaveData>(UGameplayStatics::LoadGameFromSlot(ProfileName, 0));

			PlayerName = CurrentProfileReference->Name;
			CurrentProfileName = ProfileName;

			// Set data in the players' controller
			//GetPlayer
			break;
		}
	}
}


AActor_GridTile* UEnterTheNetscape_GameInstance::FindTileByCoordinates(int x, int y) const
{
	TArray<AActor*> GridTilesArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor_GridTile::StaticClass(), GridTilesArray);
	AActor_GridTile* GridTile = nullptr;

	for (AActor* Tile : GridTilesArray) {
		GridTile = Cast<AActor_GridTile>(Tile);
		if (Tile->GetActorLocation().X / 200 == x && Tile->GetActorLocation().Y / 200 == y) {
			return GridTile;
		}
	}

	return nullptr;
}
