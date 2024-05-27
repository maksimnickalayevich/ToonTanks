// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ToonTanksGameMode.generated.h"


UCLASS()
class TOONTANKS_API AToonTanksGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void StartGame();

	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool bIsGameOver);
	
	UPROPERTY(EditAnywhere, Category="Links")
	class APlayerTank* PlayerTankRef;
	
	float StartDelay = 3.f;

private:
	int32 EnemyTowersAmount;
	bool IsPlayerAlive = true;

public:
	virtual void BeginPlay() override;

	void HandleGameStart();
	
	UPROPERTY()
	class AToonTanksPlayerController* ToonTanksPlayerController;

	void SetEnemyTowersAmount(int32 NewAmount);
	int32 GetEnemyTowersAmount();
	void SetIsPlayerAlive(bool NewIsAlive);
	bool GetIsPlayerAlive() {return this->IsPlayerAlive;}
};
