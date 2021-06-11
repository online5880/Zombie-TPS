// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "NiagaraSystem.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Projectile_Base.generated.h"

UCLASS()
class JUNE_API AProjectile_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile_Base();

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Bullet")
	class UCapsuleComponent* CapsuleComponent;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Bullet")
	class UStaticMeshComponent* Bullet;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Bullet")
	class UProjectileMovementComponent* ProjectileMovementComponent;

	class UNiagaraSystem* Normal_Impact;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
