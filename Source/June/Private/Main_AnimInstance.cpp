// Fill out your copyright notice in the Description page of Project Settings.


#include "Main_AnimInstance.h"

#include "DrawDebugHelpers.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
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
		if(Main->bAiming)
		{
			Controller_Yaw = Pawn->GetControlRotation().Pitch;
		}
		else {Controller_Yaw = 0.f;}
		

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

	if(bool bResult = GetWorld()->LineTraceSingleByChannel(OutHit,Start,End,ECC_Visibility,CollisionParams))
	{
		TEnumAsByte<EPhysicalSurface> Surface = OutHit.GetComponent()->GetMaterial(0)->GetPhysicalMaterial()->SurfaceType.GetValue();
		UGameplayStatics::PlaySoundAtLocation(GetWorld(),Foot_Sound[Surface.GetValue()],OutHit.Location,FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),Foot_Niagara[Surface.GetValue()],OutHit.Location);
		/*switch (Surface)
		{
			case 0:
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Default Surface"));
				UGameplayStatics::PlaySoundAtLocation(GetWorld(),Foot_Sound[0],OutHit.Location,FRotator::ZeroRotator);
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),Foot_Niagara[0],OutHit.Location);
				break;
			case 1:
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Water Surface"));
				UGameplayStatics::PlaySoundAtLocation(GetWorld(),Foot_Sound[1],OutHit.Location,FRotator::ZeroRotator);
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),Foot_Niagara[1],OutHit.Location);
				break;
			case 2:
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Wood Surface"));
				UGameplayStatics::PlaySoundAtLocation(GetWorld(),Foot_Sound[2],OutHit.Location,FRotator::ZeroRotator);
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),Foot_Niagara[2],OutHit.Location);
				break;
			case 3:
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Blood Surface"));
				UGameplayStatics::PlaySoundAtLocation(GetWorld(),Foot_Sound[3],OutHit.Location,FRotator::ZeroRotator);
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),Foot_Niagara[3],OutHit.Location);
				break;
			case 4:
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Metal Surface"));
				UGameplayStatics::PlaySoundAtLocation(GetWorld(),Foot_Sound[4],OutHit.Location,FRotator::ZeroRotator);
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),Foot_Niagara[4],OutHit.Location);
				break;
			case 5:
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Grass Surface"));
				UGameplayStatics::PlaySoundAtLocation(GetWorld(),Foot_Sound[5],OutHit.Location,FRotator::ZeroRotator);
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),Foot_Niagara[5],OutHit.Location);
				break;
			case 6:
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Electric Surface"));
				UGameplayStatics::PlaySoundAtLocation(GetWorld(),Foot_Sound[6],OutHit.Location,FRotator::ZeroRotator);
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),Foot_Niagara[6],OutHit.Location);
				break;
			case 7:
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Sand Surface"));
				UGameplayStatics::PlaySoundAtLocation(GetWorld(),Foot_Sound[7],OutHit.Location,FRotator::ZeroRotator);
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),Foot_Niagara[7],OutHit.Location);
				break;
			case 8:
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Glass Surface"));
				UGameplayStatics::PlaySoundAtLocation(GetWorld(),Foot_Sound[8],OutHit.Location,FRotator::ZeroRotator);
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),Foot_Niagara[8],OutHit.Location);
				break;
		}
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("%i"),Surface);
		UE_LOG(LogTemp,Warning,TEXT("Footstep : %d"),Surface.GetValue());*/
	}
	//DrawDebugLine(GetWorld(),Start,End,FColor::Red,false,3.f);
}
