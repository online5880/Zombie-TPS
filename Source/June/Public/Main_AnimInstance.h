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

	UFUNCTION(BlueprintCallable)
	void Footstep();

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Movement")
	float MovementSpeed;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Movement")
	float Direction;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Replicated,Category="Movement")
	float Yaw;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Replicated,Category="Movement")
	float Pitch;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Movement")
	bool bIsAir;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Replicated,Category="Movement")
	float Controller_Yaw;

	UPROPERTY(Replicated,BlueprintReadOnly)
	bool bAiming;

	UPROPERTY(EditAnywhere,Category="Step")
	class USoundCue* Foot_Sound[9];

	UPROPERTY(EditAnywhere,Category="Step")
	class UNiagaraSystem* Foot_Niagara[9];

	class APawn* Pawn;

	UPROPERTY(BlueprintReadOnly)
	class AMain* Main;
	/************************ 서버 ************************/
	UFUNCTION(Server,Reliable,WithValidation)
	void ServerController_Yaw(float fYaw);
	bool ServerController_Yaw_Validate(float fYaw);
	void ServerController_Yaw_Implementation(float fYaw);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
