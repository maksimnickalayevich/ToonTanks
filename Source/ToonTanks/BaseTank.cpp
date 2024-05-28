// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseTank.h"

#include "BulletProjectile.h"
#include "HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Helpers/Macroses.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
ABaseTank::ABaseTank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Instantiate member components
	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(FName("CapsuleCollision"));
	RootComponent = CapsuleCollision;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("BaseMesh"));
	BaseMesh->SetupAttachment(CapsuleCollision);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("TurretMesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(FName("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(FName("Actor Health"));

	ConstructorHelpers::FObjectFinder<UParticleSystem> DeathParticleAsset(TEXT("/Script/Engine.ParticleSystem'/Game/Assets/Effects/P_DeathEffect.P_DeathEffect'"));
	this->DeathParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(FName("DeathParticleSystemComponent"));
	if (DeathParticleAsset.Succeeded())
	{
		this->DeathParticleSystem->SetTemplate(DeathParticleAsset.Object);
	}
	this->DeathParticleSystem->bHiddenInGame = true;

	// Init death sound
	ConstructorHelpers::FObjectFinder<USoundBase> DeathSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Assets/Audio/Explode_Audio.Explode_Audio'"));
	if (DeathSoundAsset.Succeeded()) this->DeathSound = DeathSoundAsset.Object;
}

UStaticMesh* ABaseTank::GetBaseTankMesh(FString Path)
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> BaseTankMeshAsset(*Path);
	UStaticMesh* BaseTankAsset = BaseTankMeshAsset.Object;
	if (BaseTankAsset == nullptr)
	{
		ASSET_NOT_FOUND("Asset for Base Tank not found!");
	}

	return BaseTankAsset;
}

UStaticMesh* ABaseTank::GetBaseTurretMesh(FString Path)
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> BaseTurretMeshAsset(*Path);
	UStaticMesh* BaseTurretAsset = BaseTurretMeshAsset.Object;
	if (BaseTurretAsset == nullptr)
	{
		ASSET_NOT_FOUND("Asset for Base Turret not found!");
	}
	return BaseTurretAsset;
}

void ABaseTank::BuildInstanceWithAppliedSettings() const
{
}

// Called when the game starts or when spawned
void ABaseTank::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABaseTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (float CurrentHealth = this->HealthComponent->GetHealth(); CurrentHealth <= 0.0f) this->Destroy();
}

void ABaseTank::RotateTurret(const FVector& LookAtTarget) const
{
	FVector ToTarget = LookAtTarget - this->TurretMesh->GetComponentLocation();
	FRotator NewRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);

	this->TurretMesh->SetWorldRotation(
		FMath::RInterpTo(
			this->TurretMesh->GetComponentRotation(),
			NewRotation,
			UGameplayStatics::GetWorldDeltaSeconds(this),
			10.f
		)
	);
}

UHealthComponent* ABaseTank::GetHealthComponent()
{
	if (this->HealthComponent != nullptr)
	{
		return this->HealthComponent;
	}
	
	return nullptr;
}

void ABaseTank::SpawnBulletProjectile()
{
	const FActorSpawnParameters BulletActorSpawnParams = {};
	GetWorld()->SpawnActor<ABulletProjectile>(
		this->ProjectileSpawnPoint->GetComponentLocation(),
		this->ProjectileSpawnPoint->GetComponentRotation(),
		BulletActorSpawnParams
	);
}


