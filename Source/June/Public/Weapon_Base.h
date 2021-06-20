// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GetText_Interface.h"
#include "Interact_Interface.h"
#include "NiagaraSystem.h"
#include "Projectile_Base.h"
#include "Weapon_CameraShake_Base.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"
#include "Weapon_Base.generated.h"

UCLASS()
class JUNE_API AWeapon_Base : public AActor, public IInteract_Interface, public IGetText_Interface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon_Base();
	/************************ 몸 ***********************/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category ="Weapon")
	class USkeletalMeshComponent* Body_Mesh;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Weapon")
	TSubclassOf<AProjectile_Base> Bullet;
	/************************ 무기 정보 ***********************/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Weapon")
	FString Weapon_Name;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Weapon")
	int32 Ammo;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Weapon")
	int32 MaxAmmo;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Weapon")
	int32 HaveAmmo;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Weapon")
	float Rifle_Damage;
	/************************ 발사 ***********************/
	void Fire();
	
	void Fire_Start();

	void Fire_End();

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Weapon")
	TSubclassOf<UWeapon_CameraShake_Base> CameraShake;

	void Reload();
	
	void Reload_End();
	/************************ 사운드 ***********************/
	UPROPERTY(BlueprintReadOnly, Category = "Audio")
	class UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Audio")
	class USoundCue* Pickup_Sound_Cue;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Rifle")
	class USoundCue* Rifle_Shoot_Cue;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Rifle")
	class USoundCue* Rifle_Empty_Cue;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Rifle")
	class USoundCue* Rifle_Reload_Cue;
	/************************ 나이아가라 ***********************/
	class UNiagaraSystem* Rifle_Muzzle_Niagara;
	/************************ 피 ***********************/
	void Blood_Splatter_Decal(FVector Start, FVector End);

	class UMaterialInterface* Blood_Decal[12];
	/************************ 서버 ***********************/

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	virtual void Interact() override;

	virtual FString Get_Name() override;

	class AMain* Main;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AMain> TSubActor;

	class AActor* Target;

};
