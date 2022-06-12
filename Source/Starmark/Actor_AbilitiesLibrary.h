#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Engine/DataTable.h"
#include "EnterTheNetscape_Variables.h"

#include "Actor_AbilitiesLibrary.generated.h"


// Forward Declarations
class ACharacter_Pathfinder;


UCLASS()
class STARMARK_API AActor_AbilitiesLibrary : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActor_AbilitiesLibrary();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

// Variables
// --------------------------------------------------
	
// ------------------------- Data Tables
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* AbilitiesDataTable;

// ------------------------- Special Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int RememberedIntegerOne = 0;
	
// Functions
// --------------------------------------------------
	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void SwitchOnAbilityEffect(E_Ability_Functions AbilityFunction, ACharacter_Pathfinder* Avatar, AActor* Target);
};
