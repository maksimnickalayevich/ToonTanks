// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHPWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class TOONTANKS_API UEnemyHPWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="Health")
	float CurrentHealth;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	TObjectPtr<class UProgressBar> HPBar;
	
	void SetHPPercent(float NewHealthPercent);
};
