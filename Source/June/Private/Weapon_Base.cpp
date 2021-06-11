// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_Base.h"

#include "DrawDebugHelpers.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundBase.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
AWeapon_Base::AWeapon_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Body_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body Mesh"));
	RootComponent = Body_Mesh;

	Body_Mesh->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	AudioComponent->bAutoActivate = false;
	AudioComponent->SetupAttachment(RootComponent);
	/************************ 나이아가라 ***********************/
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>
	Rifle_Muzzle(TEXT("NiagaraSystem'/Game/Realistic_Starter_VFX_Pack_Niagara/Niagara/MuzzleFlash/NS_MuzzleFlash_01.NS_MuzzleFlash_01'"));
	if(Rifle_Muzzle.Succeeded())
	{
		Rifle_Muzzle_Niagara = Rifle_Muzzle.Object;
	}
}

// Called when the game starts or when spawned
void AWeapon_Base::BeginPlay()
{
	Super::BeginPlay();

	AActor* Actor = UGameplayStatics::GetPlayerPawn(GetWorld(),0);
	Main = Cast<AMain>(Actor);
}

// Called every frame
void AWeapon_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon_Base::Interact()
{
	AttachToComponent(Main->GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,FName("Rifle_Socket"));
	Weapon_Name = "";
	AudioComponent->SetSound(Pickup_Sound_Cue);
	AudioComponent->Play();
}

FString AWeapon_Base::Get_Name()
{
	return Weapon_Name;
}

void AWeapon_Base::Fire()
{
	if(Ammo != 0)
	{
		Ammo--;
		AudioComponent->SetSound(Rifle_Shoot_Cue);
		AudioComponent->Play();
		Fire_Start();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1,1.5f,FColor::Red,FString::Printf(TEXT("총알 없음"),false));
		AudioComponent->SetSound(Rifle_Empty_Cue);
		AudioComponent->Play();
	}
}

void AWeapon_Base::Fire_Start()
{
	FVector Camera_Location = Main->CameraComponent->GetComponentLocation();
	FVector Start_Vector = UKismetMathLibrary::GetForwardVector(Main->CameraComponent->GetComponentRotation());
	FVector StartLocation = (Camera_Location+(Start_Vector*250.f));
	
	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(Main->CameraComponent->GetComponentRotation());
	FVector EndLocation = (Camera_Location+(ForwardVector * 20000.f));
		
	FHitResult OutHit;
	
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	CollisionParams.AddIgnoredActor(Main);
		
	if(bool Line = GetWorld()->LineTraceSingleByChannel(OutHit,StartLocation,EndLocation,ECC_Visibility,CollisionParams))
	{
		FVector Socket_Location = Body_Mesh->GetSocketLocation(FName("Bullet_Socket"));
		Body_Mesh->GetSocketRotation(FName("Bullet_Socket"));
		FVector Select_Vector = UKismetMathLibrary::SelectVector(OutHit.ImpactPoint,OutHit.TraceEnd,Line);
		FRotator Find_Rot = UKismetMathLibrary::FindLookAtRotation(Socket_Location,Select_Vector);
		GetWorld()->SpawnActor<AProjectile_Base>(Bullet,Socket_Location,Find_Rot);

		UNiagaraFunctionLibrary::SpawnSystemAttached(Rifle_Muzzle_Niagara,Body_Mesh,FName("b_gun_muzzleflash"),FVector::ZeroVector,FRotator::ZeroRotator,EAttachLocation::KeepRelativeOffset,true,true);

		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartMatineeCameraShake(CameraShake,1.0f,ECameraShakePlaySpace::CameraLocal,FRotator::ZeroRotator);
		//DrawDebugLine(GetWorld(),StartLocation,EndLocation,FColor::Red,true,0.f);
	}		
}

void AWeapon_Base::Fire_End()
{
}

void AWeapon_Base::Reload()
{
	AudioComponent->SetSound(Rifle_Reload_Cue);
	AudioComponent->Play();
}

void AWeapon_Base::Reload_End()
{
	if((MaxAmmo - Ammo) > HaveAmmo)
	{
		Ammo = HaveAmmo + Ammo;
	}
	else
	{
		HaveAmmo = HaveAmmo - (MaxAmmo - Ammo);
		Ammo = (MaxAmmo - Ammo) + Ammo;
	}
}
