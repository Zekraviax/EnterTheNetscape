#include "Widget_DevMenu.h"


#include "Materials/MaterialInstanceDynamic.h"
#include "Components/SkeletalMeshComponent.h"
#include "Actor_CharacterTest.h"
#include "Actor_MorphTargetTest.h"
#include "EnterTheNetscape_Variables.h"


// ------------------------- Widgets
void UWidget_DevMenu::OnWidgetOpened()
{
	for (auto Dye = DyeColorsMap.CreateConstIterator(); Dye; ++Dye) {
		ColourOneDropDown->AddOption(*Dye.Value());
		ColourTwoDropDown->AddOption(*Dye.Value());
		ColourThreeDropDown->AddOption(*Dye.Value());
	}
}


void UWidget_DevMenu::OnDevSubMenuDropdownChanged()
{
	if (DevSubMenuDropdown->GetSelectedOption() == "Dye-Changing") {

	} else if (DevSubMenuDropdown->GetSelectedOption() == "Type Calculator") {

	} else if (DevSubMenuDropdown->GetSelectedOption() == "Face Customizer") {

	} else if (DevSubMenuDropdown->GetSelectedOption() == "Grid Movement") {

	}
}


// ------------------------- Dye-Changing
void UWidget_DevMenu::OnColourDropdownChanged(E_DevMenu_ColourChangeDropdowns Dropdown)
{
	if (DyeChangeCharacter) {
		UMaterialInstanceDynamic* Material = DyeChangeCharacter->DynamicMaterial;

		for (auto Dye = DyeColorsMap.CreateConstIterator(); Dye; ++Dye) {
			if (*Dye.Value() == ColourOneDropDown->GetSelectedOption())
				Material->SetVectorParameterValue("Colour1", Dye.Key());

			if (*Dye.Value() == ColourTwoDropDown->GetSelectedOption())
				Material->SetVectorParameterValue("Colour2", Dye.Key());

			if (*Dye.Value() == ColourThreeDropDown->GetSelectedOption())
				Material->SetVectorParameterValue("Colour3", Dye.Key());
		}
	}
}

// ------------------------- Face Customizer
void UWidget_DevMenu::OnFaceChanged(AActor_MorphTargetTest* MorphTargetTestCharacter)
{
	if (MorphTargetTestCharacter) {
		if (Face_DropDown->GetSelectedOption() == "0") {
			Cast<USkeletalMeshComponent>(MorphTargetTestCharacter->GetComponentByClass(USkeletalMeshComponent::StaticClass()))->SetMorphTarget("Key 1", 0.f);
			Cast<USkeletalMeshComponent>(MorphTargetTestCharacter->GetComponentByClass(USkeletalMeshComponent::StaticClass()))->SetMorphTarget("Key 2", 0.f);
		}
		else if (Face_DropDown->GetSelectedOption() == "1") {
			Cast<USkeletalMeshComponent>(MorphTargetTestCharacter->GetComponentByClass(USkeletalMeshComponent::StaticClass()))->SetMorphTarget("Key 1", -1.f);
			Cast<USkeletalMeshComponent>(MorphTargetTestCharacter->GetComponentByClass(USkeletalMeshComponent::StaticClass()))->SetMorphTarget("Key 2", 1.f);
		}
		else if (Face_DropDown->GetSelectedOption() == "2") {
			Cast<USkeletalMeshComponent>(MorphTargetTestCharacter->GetComponentByClass(USkeletalMeshComponent::StaticClass()))->SetMorphTarget("Key 1", 1.f);
			Cast<USkeletalMeshComponent>(MorphTargetTestCharacter->GetComponentByClass(USkeletalMeshComponent::StaticClass()))->SetMorphTarget("Key 2", -1.f);
		}
	}
}


void UWidget_DevMenu::OnEyesChanged(AActor_MorphTargetTest* MorphTargetTestCharacter)
{
	if (MorphTargetTestCharacter) {
		USkeletalMeshComponent* SkeletalMeshComponent = Cast<USkeletalMeshComponent>(MorphTargetTestCharacter->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
		UMaterialInstanceDynamic* PizzaEyes_DynamicMaterial = UMaterialInstanceDynamic::Create(PizzaEyes_Material, MorphTargetTestCharacter);
		UMaterialInstanceDynamic* RoundEyes_DynamicMaterial = UMaterialInstanceDynamic::Create(RoundEyes_Material, MorphTargetTestCharacter);

		if (Eyes_DropDown->GetSelectedOption() == "Pizza" && PizzaEyes_DynamicMaterial)
			SkeletalMeshComponent->SetMaterial(0, PizzaEyes_DynamicMaterial);
		else if (Eyes_DropDown->GetSelectedOption() == "Round" && RoundEyes_DynamicMaterial)
			SkeletalMeshComponent->SetMaterial(0, RoundEyes_DynamicMaterial);
	}
}