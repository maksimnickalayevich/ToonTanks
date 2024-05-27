// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseTank.h"
#include "PlayerTank.h"
#include "EnemyTurret.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API AEnemyTurret : public ABaseTank
{
	GENERATED_BODY()

public:
	AEnemyTurret();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category="HUD")
	class UWidgetComponent* WidgetDisplaySurface;
	
	UPROPERTY(EditAnywhere, Category="HUD")
	TSubclassOf<class UEnemyHPWidget> EnemyHPClass;

	UPROPERTY()
	class UEnemyHPWidget* EnemyHPInstance;

	void SetupHPHud();
	
protected:
	virtual void BuildInstanceWithAppliedSettings() const override;
	virtual void ShootInEnemy();

private:
	UPROPERTY(EditAnywhere, Category="Enemies")
	APlayerTank* PlayerTank;

	UPROPERTY(EditAnywhere, Category="Enemies")
	float TriggerMaxDistance;
	UPROPERTY(EditInstanceOnly, Category="Fight")
	float FireRateTime;
	FTimerHandle TimerHandle;
	void ShootProjectile();
	void UpdateWidgetRotationToFaceCamera();

	UPROPERTY()
	class APlayerCameraManager* PlayerCameraComponent;

public:
	float CalculateHealthPercent();
};
