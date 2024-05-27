// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksPlayerController.h"

void AToonTanksPlayerController::SetPlayerEnabledState(bool bPlayerEnabled)
{
	if (bPlayerEnabled)
	{
		this->GetPawn()->EnableInput(this);
	}
	else
	{
		this->GetPawn()->DisableInput(this);

	}
}
