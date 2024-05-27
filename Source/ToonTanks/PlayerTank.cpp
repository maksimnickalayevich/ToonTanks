// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerTank.h"

#include "HealthComponent.h"
#include "PlayerHUD.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

APlayerTank::APlayerTank()
{
	// Find default meshes
	PrimaryActorTick.bCanEverTick = true;
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(FName("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	
	UStaticMesh* BaseTankAsset = this->GetBaseTankMesh(FString("/Script/Engine.StaticMesh'/Game/Assets/Meshes/SM_TankBase.SM_TankBase'"));
	UStaticMesh* BaseTurretAsset = this->GetBaseTurretMesh(FString("/Script/Engine.StaticMesh'/Game/Assets/Meshes/SM_TankTurret.SM_TankTurret'"));

	// Assign default meshes
	BaseMesh->SetStaticMesh(BaseTankAsset);
	TurretMesh->SetStaticMesh(BaseTurretAsset);
	
	// Build the Instance with default settings
	APlayerTank::BuildInstanceWithAppliedSettings();
	
	// Ensure The Instance Possess the player by default
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void APlayerTank::BuildInstanceWithAppliedSettings() const
{
	this->SpringArmComponent->SetRelativeRotation(FRotator(-40, 0, 0));
	SpringArmComponent->TargetArmLength = ArmLength;

	this->BaseMesh->SetRelativeLocation(FVector(0, 0, -69.0f));

	this->TurretMesh->SetRelativeScale3D(FVector(0.9f, 0.9f, 0.9f));
	this->TurretMesh->SetRelativeLocation(FVector(0, 0, 5.0f));

	this->CapsuleCollision->SetCapsuleRadius(60.f);
	this->CapsuleCollision->SetCapsuleHalfHeight(60.f);
	this->CapsuleCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	this->ProjectileSpawnPoint->SetRelativeLocation(FVector(SpawnPointLength, 0, 90.f));

}

void APlayerTank::BeginPlay()
{
	Super::BeginPlay();
	this->PlayerController = Cast<APlayerController>(this->GetController());
	this->PlayerController->SetShowMouseCursor(true);
	this->CurrentAmmoAmount = this->DefaultAmmoAmount;
	
	this->SetupPlayerHUD();
}

void APlayerTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FHitResult CursorHitResult;
	if (PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, CursorHitResult))
	{
		this->RotateTurret(CursorHitResult.ImpactPoint);
	}
}

// HUD
void APlayerTank::SetupPlayerHUD()
{
	this->PlayerHUDInstance = CreateWidget<UPlayerHUD>(
		this->GetPlayerController(),
		this->PlayerHUDClass
	);
	
	this->PlayerHUDInstance->SetAmmoText(this->CurrentAmmoAmount);
	this->PlayerHUDInstance->SetHealthText(this->HealthComponent->GetHealth());

	this->PlayerHUDInstance->AddToViewport();
	
}

void APlayerTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerTank::Move);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APlayerTank::Rotate);
	PlayerInputComponent->BindAction(FName("Fire"), EInputEvent::IE_Pressed, this, &APlayerTank::ShootProjectile);
}

APlayerController* APlayerTank::GetPlayerController()
{
	return this->PlayerController;
}

// MOVEMENT MECHANICS
void APlayerTank::Move(float Value)
{
	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	FVector Movement = FVector(Value * MoveSpeed * DeltaTime, 0, 0);
	this->AddActorLocalOffset(Movement, true);
}

void APlayerTank::Rotate(float Value)
{
	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	float RotationValue = Value * RotationSpeed * DeltaTime;
	FRotator Rotation = FRotator(0, RotationValue, 0);
	this->AddActorLocalRotation(Rotation, true);
}

// SHOOTING MECHANICS
void APlayerTank::ShootProjectile()
{
	if (!FireRateCanShoot || !CurrentAmmoAmount) return;
	this->SpawnBulletProjectile();	

	this->SetFireRateTimer();
	SubtractAmmoAndReload();
}

void APlayerTank::SubtractAmmoAndReload()
{
	// Reload mechanics
	int AmmoLeft = this->CurrentAmmoAmount - 1;
	this->SetAmmoAmount(AmmoLeft);
	
	if (!this->CurrentAmmoAmount)
	{
		this->SetIsOnReload(true);
		this->SetReloadTimer();
	}
}

void APlayerTank::SetAmmoAmount(int NewAmount)
{
	this->CurrentAmmoAmount = NewAmount;
	// Update HUD
	this->PlayerHUDInstance->SetAmmoText(NewAmount);
}

void APlayerTank::SetFireRateTimer()
{
	this->ToggleFireRateCanShoot();
	GetWorldTimerManager().SetTimer(
		this->ShootReloadTimerHandle,
		this,
		&APlayerTank::ToggleFireRateCanShoot,
		FireRate
	);
}

void APlayerTank::SetReloadTimer()
{
	GetWorldTimerManager().SetTimer(
		this->ShootReloadTimerHandle,
		this,
		&APlayerTank::ToggleIsOnReload,
		ReloadTime
	);
}

void APlayerTank::SetIsOnReload(bool NewReloadValue)
{
	this->IsOnReload = NewReloadValue;
}

void APlayerTank::ToggleFireRateCanShoot()
{
	// Toggle the reload after shoot
	this->FireRateCanShoot = !this->FireRateCanShoot;
}

void APlayerTank::ToggleIsOnReload()
{
	this->SetAmmoAmount(this->DefaultAmmoAmount);
	this->SetIsOnReload(false);
	if (!this->FireRateCanShoot) this->ToggleFireRateCanShoot();
}

