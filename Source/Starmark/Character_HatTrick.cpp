#include "Character_HatTrick.h"


ACharacter_HatTrick::ACharacter_HatTrick()
{
	// Delete unnecessary components
	// Default Mesh
	GetMesh()->DestroyComponent();
	GetMesh()->SetActive(false);

	// Spring Arm
	UActorComponent* SpringArm = GetComponentByClass(USpringArmComponent::StaticClass());
	if (SpringArm != nullptr) {
		SpringArm->DestroyComponent();
		SpringArm->SetActive(false);
	}

	// Static Meshes
	TArray<UActorComponent*> StaticMeshComponentsArray = GetComponentsByClass(UStaticMeshComponent::StaticClass());
	for (int i = 0; i < StaticMeshComponentsArray.Num(); i++) {
		if (!StaticMeshComponentsArray[i]->GetName().Contains("Hat")) {
			StaticMeshComponentsArray[i]->DestroyComponent();
			StaticMeshComponentsArray[i]->SetActive(false);
			//UE_LOG(LogTemp, Warning, TEXT("ACharacter_HatTrick Constructor / Static Mesh Component Found: %s"), *StaticMeshComponentsArray[i]->GetName());
		}
	}
}