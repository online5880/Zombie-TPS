// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Grenade.generated.h"

UCLASS()
class JUNE_API AGrenade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrenade();
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	class UStaticMeshComponent* Body_Mesh;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	class UProjectileMovementComponent* Projectile;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
