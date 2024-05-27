// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"

#include "Components/TextBlock.h"

void UPlayerHUD::SetAmmoText(const int Ammo)
{
	const FString FormattedString = FString::Format(
		TEXT("Ammo: {0}"),
		{FString::FromInt(Ammo)}
	);

	this->AmmoText->SetText(FText::FromString(FormattedString));
}

void UPlayerHUD::SetHealthText(float Health)
{
	int RoundedHealth = FMath::CeilToInt(Health);
	const FString FormattedString = FString::Format(
		TEXT("Health: {0}"),
		{FString::FromInt(RoundedHealth)}
	);
	
	this->HealthText->SetText(FText::FromString(FormattedString));
}
