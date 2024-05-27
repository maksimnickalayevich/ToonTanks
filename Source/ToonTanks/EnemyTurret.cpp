// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyTurret.h"

#include "EnemyHPWidget.h"
#include "HealthComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

AEnemyTurret::AEnemyTurret()
{
	PrimaryActorTick.bCanEverTick = true;

	UStaticMesh* BaseTankMesh = GetBaseTankMesh(FString("/Script/Engine.StaticMesh'/Game/Assets/Meshes/SM_TowerBase.SM_TowerBase'"));
	UStaticMesh* TurretTankMesh = GetBaseTankMesh(FString("/Script/Engine.StaticMesh'/Game/Assets/Meshes/SM_TowerTurret.SM_TowerTurret'"));
	BaseMesh->SetStaticMesh(BaseTankMesh);
	TurretMesh->SetStaticMesh(TurretTankMesh);

	this->WidgetDisplaySurface = CreateDefaultSubobject<UWidgetComponent>(FName("Widget Display Surface"));
	this->WidgetDisplaySurface->bHiddenInGame = false;
	this->WidgetDisplaySurface->SetWidgetSpace(EWidgetSpace::World);
	this->WidgetDisplaySurface->SetDrawSize(FVector2D(100.f, 25.f));
	
	AEnemyTurret::BuildInstanceWithAppliedSettings();
}

void AEnemyTurret::BeginPlay()
{
	Super::BeginPlay();
	PlayerTank = Cast<APlayerTank>(UGameplayStatics::GetPlayerPawn(this->GetWorld(), 0));
	GetWorldTimerManager().SetTimer(
		TimerHandle,
		this,
		&AEnemyTurret::ShootInEnemy,
		FireRateTime,
		true
	);
	SetupHPHud();
}

void AEnemyTurret::SetupHPHud()
{
	this->WidgetDisplaySurface->SetWidgetClass(this->EnemyHPClass);
	
	this->EnemyHPInstance = CreateWidget<UEnemyHPWidget>(
		this->GetWorld(),
		this->EnemyHPClass
	);
	this->EnemyHPInstance->SetHPPercent(this->CalculateHealthPercent());

	// Find Camera attached to Player
	this->PlayerCameraComponent = UGameplayStatics::GetPlayerCameraManager(this->GetWorld(), 0);
	this->WidgetDisplaySurface->SetWidget(this->EnemyHPInstance);
}

void AEnemyTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (PlayerTank == nullptr) return;
	if (float DistanceToEnemy = FVector::Dist(this->GetActorLocation(), PlayerTank->GetActorLocation()); DistanceToEnemy <= this->TriggerMaxDistance)
	{
		this->RotateTurret(PlayerTank->GetActorLocation());
	}
	// Update Widget rotation to always face to camera
	UpdateWidgetRotationToFaceCamera();
}

void AEnemyTurret::BuildInstanceWithAppliedSettings() const
{
	this->CapsuleCollision->SetCapsuleRadius(30.f);	
	this->CapsuleCollision->SetCapsuleHalfHeight(62.f);
	this->CapsuleCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	this->BaseMesh->SetRelativeLocation(FVector(0, 0, -59.f));
	this->TurretMesh->SetRelativeLocation(FVector(0, 0, -7.f));
	this->ProjectileSpawnPoint->SetRelativeLocation(FVector(100.f, -1.f, 95.f));
}

void AEnemyTurret::ShootProjectile()
{
	this->SpawnBulletProjectile();
}

void AEnemyTurret::ShootInEnemy()
{
	if (this->PlayerTank == nullptr) return;
	
	// Find the Distance to a Player Tank
	if (FVector::Dist(this->GetActorLocation(), PlayerTank->GetActorLocation()) <= this->TriggerMaxDistance)
	{
		this->ShootProjectile();
	}
}

void AEnemyTurret::UpdateWidgetRotationToFaceCamera()
{
	if (this->PlayerCameraComponent == nullptr) return;
	
	// Find angle distance between camera and widget
	FVector ToTargetDistance = this->PlayerCameraComponent->GetCameraLocation() - this->WidgetDisplaySurface->GetComponentLocation();
	FRotator NewRotation = FRotator(0.f, ToTargetDistance.Rotation().Yaw, 0.f);
	this->WidgetDisplaySurface->SetWorldRotation(NewRotation);
}

float AEnemyTurret::CalculateHealthPercent()
{
	float DefaultHealth = this->GetHealthComponent()->GetDefaultHealth();
	return this->HealthComponent->GetHealth() / DefaultHealth;
}
