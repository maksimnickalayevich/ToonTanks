// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

#include "EnemyTurret.h"
#include "PlayerHUD.h"
#include "ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent(): TTGameMode(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	TTGameMode = Cast<AToonTanksGameMode>(UGameplayStatics::GetGameMode(this->GetWorld()));
	// Find all Enemy Towers
	TArray<AActor*> AllEnemyTowers;
	UGameplayStatics::GetAllActorsOfClass(TTGameMode, AEnemyTurret::StaticClass(), AllEnemyTowers);
	if (AllEnemyTowers.Num() == 0) return;

	TTGameMode->SetEnemyTowersAmount(AllEnemyTowers.Num());
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
//
float UHealthComponent::TakeDamage(float Damage)
{
	float UpdatedHealth = this->GetHealth() - Damage;
	this->SetHealth(UpdatedHealth);

	if (UpdatedHealth <= 0.0)
	{
		this->DestroyOwnerActor();
		return 0.f;
	}
	
	return UpdatedHealth;
}

void UHealthComponent::SetHealth(float NewHealth)
{
	this->Health = NewHealth;
}

float UHealthComponent::GetHealth()
{
	return this->Health;
}

void UHealthComponent::DestroyOwnerActor()
{
	AActor* Owner = this->GetOwner();
	if (Owner == nullptr) return;

	if (Owner->IsA(AEnemyTurret::StaticClass()))
	{
		AEnemyTurret* Enemy = Cast<AEnemyTurret>(Owner);
		UGameplayStatics::SpawnEmitterAtLocation(
			Enemy,
			Enemy->DeathParticleSystem->Template,
			Enemy->GetActorLocation(),
			Enemy->GetActorRotation()
		);
		this->TTGameMode->SetEnemyTowersAmount(this->TTGameMode->GetEnemyTowersAmount() - 1);
	}
	else if (Owner->IsA(APlayerTank::StaticClass()))
	{
		this->TTGameMode->SetIsPlayerAlive(false);
		APlayerTank* Player = Cast<APlayerTank>(Owner);

		UGameplayStatics::SpawnEmitterAtLocation(
			Player,
			Player->DeathParticleSystem->Template,
			Player->GetActorLocation(),
			Player->GetActorRotation()
		);
		
		Player->PlayerHUDInstance->RemoveFromParent();
	}
	
	Owner->Destroy();
}
