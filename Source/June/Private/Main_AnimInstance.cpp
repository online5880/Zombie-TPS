// Fill out your copyright notice in the Description page of Project Settings.


#include "Main_AnimInstance.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UMain_AnimInstance::UMain_AnimInstance()
{
	MovementSpeed = 0.f;
	Direction = 0.f;
	Yaw = 0.f;
	Pitch = 0.f;
	bIsAir = false;
}

void UMain_AnimInstance::NativeInitializeAnimation()
{
	if(!Pawn)
	{
		Pawn = TryGetPawnOwner();
		if(Pawn)
		{
			Main = Cast<AMain>(Pawn);
		}
	}
}

void UMain_AnimInstance::UpdateAnimationProperties()
{
	if(!Pawn)
	{
		Pawn = TryGetPawnOwner();
	}
	else
	{
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X,Speed.Y,0.f);
		FRotator Offset_Rot =  UKismetMathLibrary::NormalizedDeltaRotator(Pawn->GetControlRotation(),Pawn->GetActorRotation());
		Yaw = Offset_Rot.Yaw;
		Pitch = Offset_Rot.Pitch;
		MovementSpeed = LateralSpeed.Size();
		Direction = CalculateDirection(Speed,Pawn->GetActorRotation());
		if(Main->bAiming)
		{
			Controller_Yaw = Pawn->GetControlRotation().Pitch;
		}
		else {Controller_Yaw = 0.f;}
		

		bIsAir = Pawn->GetMovementComponent()->IsFalling();
	}
}
