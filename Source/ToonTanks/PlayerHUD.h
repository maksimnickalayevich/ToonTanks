// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class TOONTANKS_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	TObjectPtr<class UCanvasPanel> CanvasPanel;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	TObjectPtr<class UTextBlock> AmmoText;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	TObjectPtr<class UTextBlock> HealthText;

	// TODO: Add progress bar for reload?
	
	void SetAmmoText(int Ammo);
	void SetHealthText(float Health);
};
