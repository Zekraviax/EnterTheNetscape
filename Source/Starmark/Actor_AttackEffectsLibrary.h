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

// ------------------------- Data Tables
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* StatusEffectsDataTable;

// ------------------------- Moves
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<AActor_GridTile*> HatTilesArray;

// ------------------------- Other
	//UPROPERTY()
	const FActorSpawnParameters SpawnInfo;

	UPROPERTY()
	FString AttackEffectsLibraryContextString;

// Functions
// --------------------------------------------------
	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void SwitchOnAttackEffect(EBattle_AttackEffects AttackEffect, ACharacter_Pathfinder* Attacker, AActor* Target);

// ------------------------- Jasper
	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void Jasper_Bonk(ACharacter_Pathfinder* Defender);

// ------------------------- Chirp
	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void Chirp_Scratch(ACharacter_Pathfinder* Attacker, ACharacter_Pathfinder* Defender);

// ------------------------- Spirit
	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void Spirit_Cut(ACharacter_Pathfinder* Attacker, ACharacter_Pathfinder* Defender);

// ------------------------- Sugar
	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void Sugar_Bash(ACharacter_Pathfinder* Attacker, ACharacter_Pathfinder* Defender);

	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void Sugar_Concuss(ACharacter_Pathfinder* Attacker, ACharacter_Pathfinder* Defender);
};
