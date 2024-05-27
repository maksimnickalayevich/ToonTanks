// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletProjectile.generated.h"

UCLASS()
class TOONTANKS_API ABulletProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulletProjectile();

protected:
	UPROPERTY(EditInstanceOnly)
	UStaticMeshComponent* BulletMeshComp;

	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* CapsuleCollider;
	
	UPROPERTY(EditAnywhere)
	float Speed = 1000.f;

	UPROPERTY(EditAnywhere, Category="Destroy")
	float DestroyAfterSeconds = 1.f;

	UPROPERTY(EditAnywhere, Category="Damage")
	float Damage = 30.f;

	UPROPERTY(VisibleAnywhere, Category="Damage")
	bool IsActive = true;

	UPROPERTY(EditAnywhere, Category="Damage")
	float DamageToPlayer = 35.f;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnActorHitCallback(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit );

	void Move(float DeltaTime);
	void EnableCapsuleCollider() const;
	void DestroyBullet();
	void DealDamageTo(AActor* OtherActor);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	TObjectPtr<class UParticleSystemComponent> HitParticleSystem;

	// UPROPERTY()
	// TObjectPtr<class UParticleSystemComponent> TrailParticleSystem;
};
