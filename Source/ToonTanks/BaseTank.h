// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BaseTank.generated.h"

UCLASS()
class TOONTANKS_API ABaseTank : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseTank();

protected:
	UPROPERTY(EditInstanceOnly)
	class UCapsuleComponent* CapsuleCollision;

	UPROPERTY(EditInstanceOnly)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditInstanceOnly)
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(EditInstanceOnly)
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditInstanceOnly)
	class UHealthComponent* HealthComponent;

	static UStaticMesh* GetBaseTankMesh(FString Path);

	static UStaticMesh* GetBaseTurretMesh(FString Path);

	virtual void BuildInstanceWithAppliedSettings() const;

	void RotateTurret(const FVector& LookAtTarget) const;

	void SpawnBulletProjectile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UHealthComponent* GetHealthComponent();

};
