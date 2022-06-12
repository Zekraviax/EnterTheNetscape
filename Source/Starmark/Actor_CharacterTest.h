#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Materials/Material.h"
#include "Components/StaticMeshComponent.h"
#include "Components/DecalComponent.h"

#include "Actor_CharacterTest.generated.h"

UCLASS()
class STARMARK_API AActor_CharacterTest : public ACharacter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActor_CharacterTest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

// Variables
// --------------------------------------------------

// ------------------------- Components
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterTest")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterTest")
	UMaterialInterface* StaticMaterial;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterTest")
	UMaterialInstanceDynamic* DynamicMaterial;
};
