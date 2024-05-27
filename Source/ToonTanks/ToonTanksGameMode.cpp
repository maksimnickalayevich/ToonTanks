// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"

#include "PlayerTank.h"
#include "ToonTanksPlayerController.h"
#include "Kismet/GameplayStatics.h"


void AToonTanksGameMode::BeginPlay()
{
	Super::BeginPlay();
	this->HandleGameStart();
}

void AToonTanksGameMode::HandleGameStart()
{
	this->PlayerTankRef = Cast<APlayerTank>(UGameplayStatics::GetPlayerPawn(this, 0));
	this->ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (this->ToonTanksPlayerController == nullptr) return;

	this->StartGame();
	
	this->ToonTanksPlayerController->SetPlayerEnabledState(false);

	FTimerHandle PlayerEnableTimerHandle;
	FTimerDelegate PlayerEnabledTimerDelegate = FTimerDelegate::CreateUObject(this->ToonTanksPlayerController, &AToonTanksPlayerController::SetPlayerEnabledState, true);
	GetWorldTimerManager().SetTimer(
		PlayerEnableTimerHandle,
		PlayerEnabledTimerDelegate,
		this->StartDelay,
		false
	);
}

void AToonTanksGameMode::SetEnemyTowersAmount(int32 NewAmount)
{
	this->EnemyTowersAmount = NewAmount;
	if (NewAmount == 0) this->GameOver(true);
}

int32 AToonTanksGameMode::GetEnemyTowersAmount()
{
	return this->EnemyTowersAmount;
}

void AToonTanksGameMode::SetIsPlayerAlive(bool NewIsAlive)
{
	this->IsPlayerAlive = NewIsAlive;
	if (!NewIsAlive) this->GameOver(false);
}
