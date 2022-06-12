#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Engine/DataTable.h"
#include "EnterTheNetscape_Variables.h"

#include "Actor_AttackEffectsLibrary.generated.h"


// Forward Declarations
class AActor_GridTile;
class ACharacter_HatTrick;
class AActor_StatusEffectsLibrary;
class ACharacter_NonAvatarEntity;
class ACharacter_Pathfinder;


UCLASS()
class STARMARK_API AActor_AttackEffectsLibrary : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActor_AttackEffectsLibrary();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

// Variables
// --------------------------------------------------

// ------------------------- References
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor_StatusEffectsLibrary> StatusEffectsLibrary_Class;

	UPROPERTY()
	AActor_StatusEffectsLibrary* StatusEffectsLibrary_Reference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ACharacter_NonAvatarEntity> RockWall_Class;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ACharacter_HatTrick> HatTrick_Class;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ACharacter_NonAvatarEntity> NonAvatarEntity_Class;

// ------------------------- Data Tables
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* StatusEffectsDataTable;

// ------------------------- Moves
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<AActor_GridTile*> HatTilesArray;

// ------------------------- Other
	const FActorSpawnParameters SpawnInfo;

// Functions
// --------------------------------------------------
	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void SwitchOnAttackEffect(EBattle_AttackEffects AttackEffect, ACharacter_Pathfinder* Attacker, AActor* Target);

	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void RepeatingSwitchOnAttackEffect(FAvatar_AttackStruct Attack, ACharacter_Pathfinder* Attacker, AActor* Target);

// ------------------------- Status Effects
	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void Attack_AddParalyze(ACharacter_Pathfinder* Attacker, ACharacter_Pathfinder* Defender);

	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void Attack_AddBurn(ACharacter_Pathfinder* Attacker, ACharacter_Pathfinder* Defender);

	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void Attack_AddDrowning(ACharacter_Pathfinder* Attacker, ACharacter_Pathfinder* Defender);

	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void Attack_AddStoneSkin(ACharacter_Pathfinder* Attacker, ACharacter_Pathfinder* Defender);

	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void Attack_AddBleed(ACharacter_Pathfinder* Attacker, ACharacter_Pathfinder* Defender);

	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void Attack_AddSoak(ACharacter_Pathfinder* Attacker, ACharacter_Pathfinder* Defender);

	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void Attack_AddSpellbound(ACharacter_Pathfinder* Attacker, ACharacter_Pathfinder* Defender);

// ------------------------- Other
	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void Attack_KnockbackTarget(ACharacter_Pathfinder* Attacker, ACharacter_Pathfinder* Defender);

	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void Attack_RefundMana(ACharacter_Pathfinder* Attacker);

	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void Attack_TransferMana(ACharacter_Pathfinder* Attacker, ACharacter_Pathfinder* Defender);

// ------------------------- Grid Tile Effects
	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void Spawn_RockWall(AActor_GridTile* TargetTile);

	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void Spawn_Hurricane(AActor_GridTile* TargetTile);

	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void Spawn_Hats(AActor_GridTile* TargetTile = nullptr);

	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void AddProperty_StoneRoad(AActor_GridTile* TargetTile);

	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void AddProperty_Shadow(AActor_GridTile* TargetTile);

	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void AddProperty_Fire(AActor_GridTile* TargetTile);
};
