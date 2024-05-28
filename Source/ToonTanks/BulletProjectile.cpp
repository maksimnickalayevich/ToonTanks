// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletProjectile.h"

#include "EnemyHPWidget.h"
#include "EnemyTurret.h"
#include "HealthComponent.h"
#include "PlayerHUD.h"
#include "PlayerTank.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ABulletProjectile::ABulletProjectile()
{
 	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CapsuleCollider = CreateDefaultSubobject<UCapsuleComponent>(FName("CapsuleCollision"));
	RootComponent = CapsuleCollider;
	this->EnableCapsuleCollider();
	
	// Create default mesh component to attach mesh to
	BulletMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(FName("Bullet Mesh"));
	
	// Get Asset and Mesh
	ConstructorHelpers::FObjectFinder<UStaticMesh> BulletAsset(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Meshes/SM_Projectile.SM_Projectile'"));
	UStaticMesh* BulletMesh = BulletAsset.Object;
	if (BulletMesh == nullptr) return;

	// Set Mesh and attachment
	BulletMeshComp->SetStaticMesh(BulletMesh);
	BulletMeshComp->SetupAttachment(CapsuleCollider);
	BulletMeshComp->SetGenerateOverlapEvents(false);

	// Init hit particles system
	ConstructorHelpers::FObjectFinder<UParticleSystem> HitParticlesAsset(TEXT("/Script/Engine.ParticleSystem'/Game/Assets/Effects/P_HitEffect.P_HitEffect'"));
	this->HitParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(FName("HitParticleComponentSystem"));
	this->HitParticleSystem->SetupAttachment(this->CapsuleCollider);
	if (HitParticlesAsset.Succeeded())
	{
		// Another Template that can be setup later when necessary
		this->HitParticleSystem->SetTemplate(HitParticlesAsset.Object);
	}
	this->HitParticleSystem->bHiddenInGame = true;
	this->HitParticleSystem->bAutoDestroy = false;

	ConstructorHelpers::FObjectFinder<UParticleSystem> TrailParticlesAsset(TEXT("/Script/Engine.ParticleSystem'/Game/Assets/Effects/P_ProjectileTrail.P_ProjectileTrail'"));
	this->TrailParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(FName("TrailParticleComponentSystem"));
	this->TrailParticleSystem->SetupAttachment(this->CapsuleCollider);
	if (TrailParticlesAsset.Succeeded())
	{
		this->TrailParticleSystem->SetTemplate(TrailParticlesAsset.Object);
	}
	this->TrailParticleSystem->bHiddenInGame = false;
}

void ABulletProjectile::EnableCapsuleCollider() const
{
	// Set the collision 
	CapsuleCollider->SetNotifyRigidBodyCollision(true);
	CapsuleCollider->SetCapsuleRadius(22.f);
	CapsuleCollider->SetCapsuleHalfHeight(22.f);
	CapsuleCollider->SetGenerateOverlapEvents(false);
	CapsuleCollider->SetCollisionObjectType(ECC_WorldDynamic);
	CapsuleCollider->SetCollisionResponseToAllChannels(ECR_Block);
}

// Called when the game starts or when spawned
void ABulletProjectile::BeginPlay()
{
	Super::BeginPlay();

	// Create Delegate to catch collision hits
	this->CapsuleCollider->OnComponentHit.AddDynamic(this, &ABulletProjectile::OnActorHitCallback);
}

void ABulletProjectile::OnActorHitCallback(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!this->IsActive) return;
	this->DealDamageTo(OtherActor);
	UGameplayStatics::SpawnEmitterAtLocation(
this,
		  this->HitParticleSystem->Template,
			this->GetActorLocation(),
			this->GetActorRotation()
		);
	// Disable collision to ensure no more collision will happen while moving Player Actor
	this->CapsuleCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	this->IsActive = false;
	this->Destroy();
}

void ABulletProjectile::DestroyBullet()
{
	this->Destroy();
}

void ABulletProjectile::DealDamageTo(AActor* OtherActor)
{
	if (OtherActor->IsA(APlayerTank::StaticClass()))
	{
		APlayerTank* CastedActor = Cast<APlayerTank>(OtherActor);
		UHealthComponent* HealthComponent = CastedActor->GetHealthComponent();
		float UpdatedHealth = HealthComponent->TakeDamage(this->DamageToPlayer);
		// Update PlayerHUD
		CastedActor->PlayerHUDInstance->SetHealthText(UpdatedHealth);
	}
	else if (OtherActor->IsA(AEnemyTurret::StaticClass()))
	{
		AEnemyTurret* EnemyTurret = Cast<AEnemyTurret>(OtherActor);
		UHealthComponent* HealthComponent = EnemyTurret->GetHealthComponent();
		HealthComponent->TakeDamage(this->Damage);

		float NewHealthPercent = EnemyTurret->CalculateHealthPercent();
		EnemyTurret->EnemyHPInstance->SetHPPercent(NewHealthPercent);
	}
}

void ABulletProjectile::Move(float DeltaTime)
{
	FVector CurrentLocation = this->GetActorLocation();
	FVector NewVectorDirection = CurrentLocation + this->GetActorForwardVector() * Speed * DeltaTime;
	this->SetActorLocation(NewVectorDirection, true);
}

// Called every frame
void ABulletProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	this->Move(DeltaTime);
}

