// Fill out your copyright notice in the Description page of Project Settings.


#include "Main_AnimInstance.h"

#include "DrawDebugHelpers.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

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
		bAiming = Main->bAiming;
		Controller_Yaw = Pawn->GetControlRotation().Pitch;
		if(bAiming)
		{
			MultiController_Yaw(Controller_Yaw);
		}
		else
		{
			Controller_Yaw = 0.f;
			MultiController_Yaw(0.f);
		}
		bIsAir = Pawn->GetMovementComponent()->IsFalling();
	}
}

void UMain_AnimInstance::Footstep()
{
	FVector Start = Pawn->GetActorLocation();
	FVector End = FVector(Start.X,Start.Y,Start.Z-150.f);

	FHitResult OutHit;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(Pawn);

	if(GetWorld()->LineTraceSingleByChannel(OutHit,Start,End,ECC_Visibility,CollisionParams))
	{
		TEnumAsByte<EPhysicalSurface> Surface = OutHit.GetComponent()->GetMaterial(0)->GetPhysicalMaterial()->SurfaceType.GetValue();
		UGameplayStatics::PlaySoundAtLocation(GetWorld(),Foot_Sound[Surface.GetValue()],OutHit.Location,FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),Foot_Niagara[Surface.GetValue()],OutHit.Location);
	}
}

bool UMain_AnimInstance::MultiController_Yaw_Validate(float fYaw)
{
	return true;
}

void UMain_AnimInstance::MultiController_Yaw_Implementation(float fYaw)
{
	Controller_Yaw = Yaw;
}

void UMain_AnimInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UMain_AnimInstance,bAiming);
	DOREPLIFETIME(UMain_AnimInstance,Controller_Yaw);
	DOREPLIFETIME(UMain_AnimInstance,Yaw);
	DOREPLIFETIME(UMain_AnimInstance,Pitch);
}
