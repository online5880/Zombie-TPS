// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GenericTeamAgentInterface.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Grenade.generated.h"

UCLASS()
class JUNE_API AGrenade : public AActor, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrenade();
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	class UStaticMeshComponent* Body_Mesh;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	class UProjectileMovementComponent* Projectile;

	FGenericTeamId TeamId;

	virtual FGenericTeamId GetGenericTeamId() const override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
