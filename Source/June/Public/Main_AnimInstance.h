// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Main.h"
#include "Animation/AnimInstance.h"
#include "Main_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class JUNE_API UMain_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	public:

	UMain_AnimInstance();

	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category = AnimationProperties)
	void UpdateAnimationProperties();

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Movement")
	float MovementSpeed;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Movement")
	float Direction;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Movement")
	float Yaw;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Movement")
	float Pitch;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Movement")
	bool bIsAir;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Movement")
	float Controller_Yaw;

	class APawn* Pawn;

	UPROPERTY(BlueprintReadOnly)
	class AMain* Main;
	
};
