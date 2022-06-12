#include "Widget_CharacterCreator.h"

#include "Misc/DateTime.h"
#include "Misc/DefaultValueHelper.h"
#include "Kismet/GameplayStatics.h"
#include "EnterTheNetscape_GameInstance.h"


// ------------------------- Widget
void UWidget_CharacterCreator::OnWidgetOpened()
{
	FDateTime* Date = new FDateTime;
	int CurrentYear = Date->FDateTime::Now().GetYear();

	if (Birthday_Day_DropDown->IsValidLowLevel()) {
		for (int d = 1; d <= 31; d++) {
			Birthday_Day_DropDown->AddOption(FString::FromInt(d));
		}

		Birthday_Day_DropDown->SetSelectedOption("1");
	}

	if (Birthday_Month_DropDown->IsValidLowLevel()) {
		for (int month = 1; month <= 12; month++) {
			Birthday_Month_DropDown->AddOption(FString::FromInt(month));
		}

		Birthday_Month_DropDown->SetSelectedOption("1");
	}

	if (Birthday_Year_DropDown->IsValidLowLevel()) {
		for (int y = CurrentYear; y >= 1900; y--) {
			Birthday_Year_DropDown->AddOption(FString::FromInt(y));
		}
	}
}


// ------------------------- Player
void UWidget_CharacterCreator::CalculateHoroscope()
{
	BirthDayAsNumber = FCString::Atoi(*Birthday_Day_DropDown->GetSelectedOption());
	BirthMonthAsNumber = FCString::Atoi(*Birthday_Month_DropDown->GetSelectedOption());
	BirthYearAsNumber = FCString::Atoi(*Birthday_Year_DropDown->GetSelectedOption());

	if (!GameInstanceReference) {
		GameInstanceReference = Cast<UEnterTheNetscape_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	}

	switch (BirthMonthAsNumber)
	{
		case(1):
			if (BirthDayAsNumber <= 19) {
				GameInstanceReference->PlayerData.Horoscope = EPlayer_Horoscopes::E_Capricorn;
				HoroscopeText->SetText(FText::FromString("Capricorn"));
				break;
			} else {
				GameInstanceReference->PlayerData.Horoscope = EPlayer_Horoscopes::E_Aquarius;
				HoroscopeText->SetText(FText::FromString("Aquarius"));
				break;
			}
			break;
		case(2):
			if (BirthDayAsNumber <= 18) {
				GameInstanceReference->PlayerData.Horoscope = EPlayer_Horoscopes::E_Aquarius;
				HoroscopeText->SetText(FText::FromString("Aquarius"));
				break;
			} else {
				GameInstanceReference->PlayerData.Horoscope = EPlayer_Horoscopes::E_Pisces;
				HoroscopeText->SetText(FText::FromString("Pisces"));
				break;
			}
		case(3):
			if (BirthDayAsNumber <= 20) {
				GameInstanceReference->PlayerData.Horoscope = EPlayer_Horoscopes::E_Pisces;
				HoroscopeText->SetText(FText::FromString("Pisces"));
				break;
			} else {
				GameInstanceReference->PlayerData.Horoscope = EPlayer_Horoscopes::E_Aries;
				HoroscopeText->SetText(FText::FromString("Aries"));
				break;
			}
		case(4):
			if (BirthDayAsNumber <= 19) {
				GameInstanceReference->PlayerData.Horoscope = EPlayer_Horoscopes::E_Aries;
				HoroscopeText->SetText(FText::FromString("Aries"));
				break;
			} else {
				GameInstanceReference->PlayerData.Horoscope = EPlayer_Horoscopes::E_Taurus;
				HoroscopeText->SetText(FText::FromString("Taurus"));
				break;
			}
		case(5):
			if (BirthDayAsNumber <= 20) {
				GameInstanceReference->PlayerData.Horoscope = EPlayer_Horoscopes::E_Taurus;
				HoroscopeText->SetText(FText::FromString("Taurus"));
				break;
			} else {
				GameInstanceReference->PlayerData.Horoscope = EPlayer_Horoscopes::E_Gemini;
				HoroscopeText->SetText(FText::FromString("Gemini"));
				break;
			}
		case(6):
			if (BirthDayAsNumber <= 20) {
				GameInstanceReference->PlayerData.Horoscope = EPlayer_Horoscopes::E_Gemini;
				HoroscopeText->SetText(FText::FromString("Gemini"));
				break;
			} else {
				GameInstanceReference->PlayerData.Horoscope = EPlayer_Horoscopes::E_Cancer;
				HoroscopeText->SetText(FText::FromString("Cancer"));
				break;
			}
		case(7):
			if (BirthDayAsNumber <= 22) {
				GameInstanceReference->PlayerData.Horoscope = EPlayer_Horoscopes::E_Cancer;
				HoroscopeText->SetText(FText::FromString("Cancer"));
				break;
			} else {
				GameInstanceReference->PlayerData.Horoscope = EPlayer_Horoscopes::E_Leo;
				HoroscopeText->SetText(FText::FromString("Leo"));
				break;
			}
		case(8):
			if (BirthDayAsNumber <= 22) {
				GameInstanceReference->PlayerData.Horoscope = EPlayer_Horoscopes::E_Leo;
				HoroscopeText->SetText(FText::FromString("Leo"));
				break;
			} else {
				GameInstanceReference->PlayerData.Horoscope = EPlayer_Horoscopes::E_Virgo;
				HoroscopeText->SetText(FText::FromString("Virgo"));
				break;
			}
		case(9):
			if (BirthDayAsNumber <= 22) {
				GameInstanceReference->PlayerData.Horoscope = EPlayer_Horoscopes::E_Virgo;
				HoroscopeText->SetText(FText::FromString("Virgo"));
				break;
			} else {
				GameInstanceReference->PlayerData.Horoscope = EPlayer_Horoscopes::E_Libra;
				HoroscopeText->SetText(FText::FromString("Libra"));
				break;
			}
		case(10):
			if (BirthDayAsNumber <= 22) {
				GameInstanceReference->PlayerData.Horoscope = EPlayer_Horoscopes::E_Libra;
				HoroscopeText->SetText(FText::FromString("Libra"));
				break;
			} else {
				GameInstanceReference->PlayerData.Horoscope = EPlayer_Horoscopes::E_Scorpio;
				HoroscopeText->SetText(FText::FromString("Scorpio"));
				break;
			}
		case(11):
			if (BirthDayAsNumber <= 22) {
				GameInstanceReference->PlayerData.Horoscope = EPlayer_Horoscopes::E_Scorpio;
				HoroscopeText->SetText(FText::FromString("Scorpio"));
				break;
			} else {
				GameInstanceReference->PlayerData.Horoscope = EPlayer_Horoscopes::E_Sagittarius;
				HoroscopeText->SetText(FText::FromString("Sagittarius"));
				break;
			}
		case(12):
			if (BirthDayAsNumber <= 21) {
				GameInstanceReference->PlayerData.Horoscope = EPlayer_Horoscopes::E_Sagittarius;
				HoroscopeText->SetText(FText::FromString("Sagittarius"));
				break;
			} else {
				GameInstanceReference->PlayerData.Horoscope = EPlayer_Horoscopes::E_Capricorn;
				HoroscopeText->SetText(FText::FromString("Capricorn"));
				break;
			}
		default:
			// The default is already set to Capricorn
			break;
	}

	CalculateMark();
}


void UWidget_CharacterCreator::CalculateMark()
{
	// Clear previous variables
	MarkNumberAsInt = 0;
	MarkNumberAsString = "";
	MarkNumberAsCharArray.Empty();
	MarkNumberAsIntArray.Empty();

	BirthYearAsCharArray.Empty();
	BirthMonthAsCharArray.Empty();
	BirthDayAsCharArray.Empty();
	BirthYearAsString = "";
	BirthYearAsString2 = "";

	if (BirthDayAsNumber > 9) {
		BirthDayAsCharArray.Add(Birthday_Day_DropDown->GetSelectedOption().LeftChop(1));
		BirthDayAsCharArray.Add(Birthday_Day_DropDown->GetSelectedOption().RightChop(1));
	} else {
		BirthDayAsCharArray.Add(Birthday_Day_DropDown->GetSelectedOption());
	}

	if (BirthMonthAsNumber > 9) {
		BirthMonthAsCharArray.Add(Birthday_Month_DropDown->GetSelectedOption().LeftChop(1));
		BirthMonthAsCharArray.Add(Birthday_Month_DropDown->GetSelectedOption().RightChop(1));
	} else {
		BirthMonthAsCharArray.Add(Birthday_Month_DropDown->GetSelectedOption());
	}

	BirthYearAsString = Birthday_Year_DropDown->GetSelectedOption().LeftChop(2);
	BirthYearAsString2 = Birthday_Year_DropDown->GetSelectedOption().RightChop(2);

	BirthYearAsCharArray.Add(BirthYearAsString.LeftChop(1));
	BirthYearAsCharArray.Add(BirthYearAsString.RightChop(1));
	BirthYearAsCharArray.Add(BirthYearAsString2.LeftChop(1));
	BirthYearAsCharArray.Add(BirthYearAsString2.RightChop(1));


	// Convert all individual characters into ints
	for (int x = 0; x < BirthDayAsCharArray.Num(); x++) {
		MarkNumberAsIntArray.Add(FCString::Atoi(*BirthDayAsCharArray[x]));
	}
	for (int y = 0; y < BirthMonthAsCharArray.Num(); y++) {
		MarkNumberAsIntArray.Add(FCString::Atoi(*BirthMonthAsCharArray[y]));
	}
	for (int z = 0; z < BirthYearAsCharArray.Num(); z++) {
		MarkNumberAsIntArray.Add(FCString::Atoi(*BirthYearAsCharArray[z]));
	}

	// Add all individual numbers together
	for (int i = 0; i < MarkNumberAsIntArray.Num(); i++) {
		MarkNumberAsInt += MarkNumberAsIntArray[i];
	}

	MarkText->SetText(FText::FromString(FString::FromInt(MarkNumberAsInt)));

	// If the MarkNumber is greater than 9, split the number into individual characters and add them together
	// Repeat as necessary
	while (MarkNumberAsInt > 9) {
		MarkNumberAsString = FString::FromInt(MarkNumberAsInt);
		MarkNumberAsIntArray.Empty();
		MarkNumberAsCharArray.Empty();

		MarkNumberAsCharArray.Add(MarkNumberAsString.LeftChop(1));
		MarkNumberAsCharArray.Add(MarkNumberAsString.RightChop(1));

		for (int i = 0; i < MarkNumberAsCharArray.Num(); i++) {
			MarkNumberAsIntArray.Add(FCString::Atoi(*MarkNumberAsCharArray[i]));
		}

		MarkNumberAsInt = 0;
		for (int i = 0; i < MarkNumberAsIntArray.Num(); i++) {
			MarkNumberAsInt += MarkNumberAsIntArray[i];
		}
	}

	if (!GameInstanceReference) {
		GameInstanceReference = Cast<UEnterTheNetscape_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	}
}