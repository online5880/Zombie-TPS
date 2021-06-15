// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Zombie_Base.h"
#include "Animation/AnimInstance.h"
#include "Zombie_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class JUNE_API UZombie_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	public:
	
	UZombie_AnimInstance();

	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category = AnimationProperties)
	void UpdateAnimationProperties();

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Movement")
	float MovementSpeed;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Movement")
	bool bIsAir;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Movement")
	bool bDamaged_Leg;

	class APawn* Pawn;

	class AZombie_Base* Zombie;
	
};
