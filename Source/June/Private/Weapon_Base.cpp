// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_Base.h"
#include "Main.h"
#include "DrawDebugHelpers.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundBase.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Zombie_Base.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AWeapon_Base::AWeapon_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Body_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body Mesh"));
	Body_Mesh->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
	RootComponent = Body_Mesh;
	
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
	/************************ 데칼 - 피 ************************/
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
	Blood_1(TEXT("MaterialInstanceConstant'/Game/BloodPack/4096x4096/Dots/MI_Dots_Decal_1.MI_Dots_Decal_1'"));
	if(Blood_1.Succeeded())
	{
		Blood_Decal[0] = Blood_1.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
    Blood_2(TEXT("MaterialInstanceConstant'/Game/BloodPack/4096x4096/Dots/MI_Dots_Decal_23.MI_Dots_Decal_23'"));
	if(Blood_2.Succeeded())
	{
		Blood_Decal[1] = Blood_2.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
    Blood_3(TEXT("MaterialInstanceConstant'/Game/BloodPack/4096x4096/Dots/MI_Dots_Decal_24.MI_Dots_Decal_24'"));
	if(Blood_3.Succeeded())
	{
		Blood_Decal[2] = Blood_3.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
    Blood_4(TEXT("MaterialInstanceConstant'/Game/BloodPack/4096x4096/Dots/MI_Dots_Decal_25.MI_Dots_Decal_25'"));
	
	if(Blood_4.Succeeded())
	{
		Blood_Decal[3] = Blood_4.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
    Blood_5(TEXT("MaterialInstanceConstant'/Game/BloodPack/4096x4096/Dots/MI_Dots_Decal_5.MI_Dots_Decal_5'"));
	if(Blood_5.Succeeded())
	{
		Blood_Decal[4] = Blood_5.Object;
	}
	//////////////////////////////// 땅
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
    Blood_6(TEXT("MaterialInstanceConstant'/Game/BloodPack/4096x4096/Dots/MI_Dots_Decal_6.MI_Dots_Decal_6'"));
	if(Blood_6.Succeeded())
	{
		Blood_Decal[5] = Blood_6.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
    Blood_7(TEXT("MaterialInstanceConstant'/Game/BloodPack/4096x4096/Dots/MI_Dots_Decal_8.MI_Dots_Decal_8'"));
	if(Blood_7.Succeeded())
	{
		Blood_Decal[6] = Blood_7.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
    Blood_8(TEXT("MaterialInstanceConstant'/Game/BloodPack/4096x4096/Dots/MI_Dots_Decal_22.MI_Dots_Decal_22'"));
	if(Blood_8.Succeeded())
	{
		Blood_Decal[7] = Blood_8.Object;
	}
}
// Called when the game starts or when spawned
void AWeapon_Base::BeginPlay()
{
	Super::BeginPlay();

	AActor* Actor = UGameplayStatics::GetActorOfClass(GetWorld(),TSubActor);
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
		AudioComponent->SetSound(Rifle_Empty_Cue);
		AudioComponent->Play();
	}
}

void AWeapon_Base::Blood_Splatter_Decal(FVector Start, FVector End)
{
	int32 Wall_Random = FMath::RandRange(0,4);
	FVector Wall_Blood_Size = (FVector(64.f,128.f,128.f));
	FRotator Wall_Blood_Rotate = FRotator(0.f,0.f,UKismetMathLibrary::RandomFloatInRange(0.f,360.f));
	
	FHitResult OutHit;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	CollisionParams.AddIgnoredActor(Main);

	bool IsHit = GetWorld()->LineTraceSingleByChannel(OutHit,Start,End,ECC_Visibility,CollisionParams);
	if(IsHit)
	{
		UGameplayStatics::SpawnDecalAtLocation(this,Blood_Decal[Wall_Random],Wall_Blood_Size,OutHit.ImpactPoint,Wall_Blood_Rotate,5.f);
		//UE_LOG(LogTemp,Warning,TEXT("2차 벽 튀김"))
	}
}

void AWeapon_Base::Fire_Start()
{
	FVector Camera_Location = Main->CameraComponent->GetComponentLocation();
	FVector Start_Vector = UKismetMathLibrary::GetForwardVector(Main->CameraComponent->GetComponentRotation());
	FVector StartLocation = (Camera_Location+(Start_Vector*250.f));
	
	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(Main->CameraComponent->GetComponentRotation());
	FVector EndLocation = (Camera_Location+(ForwardVector * 20000.f));

	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartMatineeCameraShake(CameraShake,1.0f,ECameraShakePlaySpace::CameraLocal,FRotator::ZeroRotator);
		
	FHitResult OutHit;
	
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	CollisionParams.AddIgnoredActor(Main);

	int32 Ground_Random = FMath::RandRange(5,7);
	FVector Ground_Blood_Size = (FVector(172.f,172.f,172.f));
	FRotator Ground_Blood_Rotate = FRotator(UKismetMathLibrary::RandomFloatInRange(-45.f,-90.f),0.f,0.f);
		
	if(bool Line = GetWorld()->LineTraceSingleByChannel(OutHit,StartLocation,EndLocation,ECC_Visibility,CollisionParams))
	{
		FVector End_Blood = (OutHit.ImpactPoint+(Start_Vector*200.f));

		FVector Z_Location = OutHit.GetActor()->GetActorLocation();
		FVector Literal_Location = FVector(Z_Location.X,Z_Location.Y,0.f);
		
		GetWorld()->SpawnActor<AProjectile_Base>(Bullet,StartLocation,Main->GetControlRotation());
		if(OutHit.GetActor()->ActorHasTag("Zombie"))
		{
			class AZombie_Base* Zombie_Base = Cast<AZombie_Base>(OutHit.GetActor());
			if(Zombie_Base)
			{
				UGameplayStatics::SpawnDecalAtLocation(this,Blood_Decal[Ground_Random],Ground_Blood_Size,Literal_Location,Ground_Blood_Rotate,12.f);
				Blood_Splatter_Decal(OutHit.ImpactPoint,End_Blood);
				
				UGameplayStatics::ApplyDamage(Zombie_Base,Rifle_Damage,nullptr,Main,nullptr);
				if(OutHit.BoneName.ToString()=="Head")
				{
					UGameplayStatics::ApplyPointDamage(OutHit.GetActor(),Rifle_Damage*1.5f,OutHit.GetActor()->GetActorLocation(),OutHit,nullptr,this,nullptr);
				}
				if(OutHit.BoneName.ToString()=="thigh_l" || OutHit.BoneName.ToString()=="thigh_r")
				{
					Zombie_Base->Leg_Health-=50.f;
				}
			}
		}
		UNiagaraFunctionLibrary::SpawnSystemAttached(Rifle_Muzzle_Niagara,Body_Mesh,FName("b_gun_muzzleflash"),FVector::ZeroVector,FRotator::ZeroRotator,EAttachLocation::KeepRelativeOffset,true,true);
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
