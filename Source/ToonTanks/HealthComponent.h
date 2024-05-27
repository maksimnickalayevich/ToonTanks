// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API UHealthComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	float TakeDamage(float Damage);
	void SetHealth(float NewHealth);
	void DestroyOwnerActor();
	float GetHealth();

	UPROPERTY()
	class AToonTanksGameMode* TTGameMode;
	
	UPROPERTY(EditAnywhere, Category="Health")
	float Health = 100.f;
	
	UPROPERTY(EditAnywhere, Category="Health")
	float DefaultHealth = 120.f;
	
	float GetDefaultHealth() const {return this->DefaultHealth;}
};
