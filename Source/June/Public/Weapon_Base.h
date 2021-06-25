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
	/************************ 서버 ************************/
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(Server,reliable,WithValidation)
	void ServerFire();
	bool ServerFire_Validate();
	void ServerFire_Implementation();

	UFUNCTION(NetMulticast,reliable,WithValidation)
	void MultiFire();
	bool MultiFire_Validate();
	void MultiFire_Implementation();

	UFUNCTION(Server,reliable,WithValidation)
	void ServerFire_Start(class AMain* Actor);
	bool ServerFire_Start_Validate(class AMain* Actor);
	void ServerFire_Start_Implementation(class AMain* Actor);

	UFUNCTION(NetMulticast,reliable,WithValidation)
	void MultiFire_Start(class AMain* Actor);
	bool MultiFire_Start_Validate(class AMain* Actor);
	void MultiFire_Start_Implementation(class AMain* Actor);

	UFUNCTION(Server,reliable,WithValidation)
	void ServerFire_End();
	bool ServerFire_End_Validate();
	void ServerFire_End_Implementation();

	UFUNCTION(Server,reliable,WithValidation)
	void ServerInteract(class AMain* Actor);
	bool ServerInteract_Validate(class AMain* Actor);
	void ServerInteract_Implementation(class AMain* Actor);

	UFUNCTION(NetMulticast,reliable,WithValidation)
	void MultiInteract(class AMain* Actor);
	bool MultiInteract_Validate(class AMain* Actor);
	void MultiInteract_Implementation(class AMain* Actor);
	/************************ 몸 ***********************/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category ="Weapon")
	class USkeletalMeshComponent* Body_Mesh;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Weapon")
	TSubclassOf<AProjectile_Base> Bullet;
	/************************ 무기 정보 ***********************/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Weapon")
	FString Weapon_Name;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Replicated,Category = "Weapon")
	int32 Ammo;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Replicated,Category = "Weapon")
	int32 MaxAmmo;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Replicated,Category = "Weapon")
	int32 HaveAmmo;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Weapon")
	float Rifle_Damage;
	/************************ 발사 ***********************/
	void Fire(class AMain* Actor);
	
	void Fire_Start(class AMain* Actor);

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

	virtual void Interact(AMain* Actor) override;

	virtual FString Get_Name() override;

	class AMain* Main;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AMain> TSubActor;

	class AActor* Target;

};
