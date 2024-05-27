// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseTank.h"
#include "PlayerTank.generated.h"

UCLASS()
class TOONTANKS_API APlayerTank : public ABaseTank
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	virtual void BuildInstanceWithAppliedSettings() const override;
	APlayerTank();

private:
	UPROPERTY(EditInstanceOnly)
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditInstanceOnly)
	class UCameraComponent* CameraComponent;
	
	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY(VisibleAnywhere)
	float SpawnPointLength = 100.f;
	
 
	UPROPERTY()
	float ArmLength = 600.f;

	// Shooting mechanics <CLASS FIELDS>
	UPROPERTY(EditInstanceOnly, Category="Fight")
	float FireRate = 0.8f;

	UPROPERTY(EditInstanceOnly, Category="Fight")
	float ReloadTime = 3.0f;

	UPROPERTY(VisibleAnywhere, Category="Fight")
	bool FireRateCanShoot = true;

	UPROPERTY(VisibleAnywhere, Category="Fight")
	bool IsOnReload = false;

	UPROPERTY(EditAnywhere, Category="Fight")
	int DefaultAmmoAmount = 5;

	UPROPERTY(VisibleAnywhere, Category="Fight")
	int CurrentAmmoAmount = 5;

	FTimerHandle ShootReloadTimerHandle;

	void ToggleFireRateCanShoot();
	void ToggleIsOnReload();
	// Setters for shooting
	void SetAmmoAmount(int NewAmount);
	void SetFireRateTimer();
	void SetReloadTimer();
	void SetIsOnReload(bool NewReloadValue);
	// Shooting itself
	void ShootProjectile();
	void SubtractAmmoAndReload();
	
	// Movement mechanics
	UPROPERTY(EditInstanceOnly, Category="Movement")
	float MoveSpeed = 400.f;
	UPROPERTY(EditInstanceOnly, Category="Movement")
	float RotationSpeed = 75.f;
	void Move(float Value);
	void Rotate(float Value);
	void SetupPlayerHUD();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	APlayerController* GetPlayerController();

	UPROPERTY(EditAnywhere, Category="HUD")
	TSubclassOf<class UPlayerHUD> PlayerHUDClass;

	UPROPERTY()
	class UPlayerHUD* PlayerHUDInstance;
	
};
