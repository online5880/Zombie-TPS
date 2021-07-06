// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie_AnimInstance.h"

#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UZombie_AnimInstance::UZombie_AnimInstance()
{
	MovementSpeed = 0.f;
	bIsAir = false;
	
	Random_Ground = UKismetMathLibrary::RandomIntegerInRange(0,1);
}

void UZombie_AnimInstance::NativeInitializeAnimation()
{
	if(!Pawn)
	{
		Pawn = TryGetPawnOwner();
		if(Pawn)
		{
			Zombie = Cast<AZombie_Base>(Pawn);
		}
	}
}

void UZombie_AnimInstance::UpdateAnimationProperties()
{
	if(!Pawn)
	{
		Pawn = TryGetPawnOwner();
	}
	else
	{
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X,Speed.Y,0.f);
		MovementSpeed = LateralSpeed.Size();
		bDamaged_Leg = Zombie->bDamaged_Leg;

		bIsAir = Pawn->GetMovementComponent()->IsFalling();
	}
}
