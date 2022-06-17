#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/SkeletalMesh.h"
#include "EnterTheNetscape_Variables.h"
#include "Engine.h"
#include "Net/UnrealNetwork.h"

#include "Character_Pathfinder.generated.h"


// Forward Declarations
class AActor_GridTile;
class APlayerController_Battle;
class UWidget_HUD_Battle;
class UWidgetComponent_AvatarBattleData;


UCLASS()
class STARMARK_API ACharacter_Pathfinder : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacter_Pathfinder();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

// Variables
// --------------------------------------------------

// ------------------------- Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UDecalComponent* ActorSelected;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UMaterialInstanceDynamic* ActorSelected_DynamicMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ActorSelectedPlane;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	FLinearColor ActorSelected_DynamicMaterial_Colour;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UWidgetComponent* AvatarBattleData_Component;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TSubclassOf<UWidgetComponent_AvatarBattleData> AvatarBattleDataComponent_Class;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UWidgetComponent_AvatarBattleData* AvatarBattleDataComponent_Reference;

// ------------------------- Entity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Entity")
	FNetscapeExplorer_Struct AvatarData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity")
	bool RotateAvatarTowardsMouse = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity")
	FDataTableRowHandle EntityDataTableRow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity")
	bool FetchDataFromDataTable;

// ------------------------- Battle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
	UDataTable* UltimateTypeChartDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Battle")
	TArray<FAvatar_AttackStruct> CurrentKnownAttacks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Battle")
	FAvatar_AttackStruct CurrentSelectedAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
	TArray<AActor*> ValidAttackTargetsArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
	TArray<FAvatar_StatusEffect> CurrentStatusEffectsArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Battle")
	int IndexInPlayerParty;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
	int MultiplayerControllerUniqueID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
	UStaticMeshComponent* AttackTraceActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
	TArray<UStaticMesh*> AttackTraceStaticMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
	int AttackRotationSnapToDegrees = 90;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
	TArray<AActor_GridTile*> CurrentPathTiles;

// ------------------------- Other
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	APlayerController_Battle* PlayerControllerReference;

	UPROPERTY()
	FString CharacterContextString;

// Functions
// --------------------------------------------------

// ------------------------- Base
	UFUNCTION(BlueprintCallable, Client, Reliable)
	void BeginPlayWorkaroundFunction(UWidget_HUD_Battle* BattleHUDReference = nullptr);

// ------------------------- Cursor
	UFUNCTION(BlueprintCallable)
	void OnAvatarCursorOverBegin();

	UFUNCTION(BlueprintCallable)
	void OnAvatarCursorOverEnd();

	UFUNCTION(BlueprintCallable)
	void OnAvatarClicked();

	UFUNCTION()
	void SetAttackTraceActorLocationSnappedToGrid();

// ------------------------- Battle
	UFUNCTION(BlueprintCallable)
	void ShowAttackRange();

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void LaunchAttack(AActor* Target);

	UFUNCTION(BlueprintCallable)
	void SetTilesOccupiedBySize(bool ClearTiles);

	UFUNCTION(BlueprintCallable)
	void UpdateAvatarDataInPlayerParty();

	UFUNCTION(BlueprintNativeEvent)
	void ActorSelectedDynamicMaterialColourUpdate();

	UFUNCTION(BlueprintNativeEvent)
	void AvatarBeginTileOverlap();

	UFUNCTION(BlueprintCallable)
	void AvatarStopMoving(bool SnapToGrid);

// ------------------------- Multiplayer
	UFUNCTION(Client, Unreliable)
	void Client_GetAvatarData(FNetscapeExplorer_Struct NewAvatarData);

	UFUNCTION()
	void Local_GetAvatarData(FNetscapeExplorer_Struct NewAvatarData);
};