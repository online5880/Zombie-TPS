// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GenericTeamAgentInterface.h"
#include "Grenade.h"
#include "Weapon_Base.h"
#include "Zombie_Base.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"

#include "Main.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	Normal UMETA(DisplayName = "Normal"),
	Rifle UMETA(DisplayName = "Rifle"),
	Pistol UMETA(DisplayName = "Pisol"),
};

UENUM(BlueprintType)
enum class EState : uint8
{
	Normal UMETA(DisplayName = "Normal"),
	Sprint UMETA(DisplayName = "Sprint"),
	Walk UMETA(DisplayName = "Walk"),
	Aiming UMETA(DisplayName = "Aiming"),
	Fire UMETA(DisplayName = "Fire"),
	React UMETA(DisplayName = "React"),
	Die UMETA(DisplayName = "Die"),
};

UCLASS()
class JUNE_API AMain : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	
	AMain();

	virtual FGenericTeamId GetGenericTeamId() const override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/************************ 서버 ************************/
	UFUNCTION(Server,Unreliable,WithValidation)
	void ServerWalk_Start();
	bool ServerWalk_Start_Validate();
	void ServerWalk_Start_Implementation();

	UFUNCTION(Server,Unreliable,WithValidation)
	void ServerWalk_End();
	bool ServerWalk_End_Validate();
	void ServerWalk_End_Implementation();

	UFUNCTION(Server,Unreliable,WithValidation)
	void ServerSprint_Start();
	bool ServerSprint_Start_Validate();
	void ServerSprint_Start_Implementation();

	UFUNCTION(Server,Unreliable,WithValidation)
	void ServerSprint_End();
	bool ServerSprint_End_Validate();
	void ServerSprint_End_Implementation();

	UFUNCTION(Server,Reliable,WithValidation)
	void ServerInteract();
	bool ServerInteract_Validate();
	void ServerInteract_Implementation();

	UFUNCTION(NetMulticast,Reliable,WithValidation)
	void MultiInteract();
	bool MultiInteract_Validate();
	void MultiInteract_Implementation();

	UFUNCTION(Server,Reliable,WithValidation)
	void ServerEquipRifle();
	bool ServerEquipRifle_Validate();
	void ServerEquipRifle_Implementation();

	UFUNCTION(NetMulticast,Reliable,WithValidation)
	void MultiEquipRifle();
	bool MultiEquipRifle_Validate();
	void MultiEquipRifle_Implementation();

	UFUNCTION(Server,Reliable,WithValidation)
	void ServerUnEquipRifle();
	bool ServerUnEquipRifle_Validate();
	void ServerUnEquipRifle_Implementation();

	UFUNCTION(NetMulticast,Reliable,WithValidation)
	void MultiUnEquipRifle();
	bool MultiUnEquipRifle_Validate();
	void MultiUnEquipRifle_Implementation();

	UFUNCTION(Server,Reliable,WithValidation)
	void ServerAiming(bool ServerAiming);
	bool ServerAiming_Validate(bool ServerAiming);
	void ServerAiming_Implementation(bool ServerAiming);

	UFUNCTION(NetMulticast,Reliable,WithValidation)
	void MultiAiming(bool MultiAiming);
	bool MultiAiming_Validate(bool MultiAiming);
	void MultiAiming_Implementation(bool MultiAiming);

	UFUNCTION(Server,Reliable,WithValidation)
	void ServerAiming_End(bool ServerAiming);
	bool ServerAiming_End_Validate(bool ServerAiming);
	void ServerAiming_End_Implementation(bool ServerAiming);

	UFUNCTION(NetMulticast,Reliable,WithValidation)
	void MultiAiming_End(bool MultiAiming);
	bool MultiAiming_End_Validate(bool MultiAiming);
	void MultiAiming_End_Implementation(bool MultiAiming);

	UFUNCTION(Server,Reliable,WithValidation)
	void ServerFire(bool bServerFire);
	bool ServerFire_Validate(bool bServerFire);
	void ServerFire_Implementation(bool bServerFire);

	UFUNCTION(NetMulticast,Reliable,WithValidation)
	void MultiFire(bool bMultiFire);
	bool MultiFire_Validate(bool bMultiFire);
	void MultiFire_Implementation(bool bMultiFire);

	UFUNCTION(Server,Reliable,WithValidation)
	void ServerFire_Start();
	bool ServerFire_Start_Validate();
	void ServerFire_Start_Implementation();

	UFUNCTION(NetMulticast,Reliable,WithValidation)
	void MultiFire_Start();
	bool MultiFire_Start_Validate();
	void MultiFire_Start_Implementation();

	UFUNCTION(Server,Reliable,WithValidation)
	void ServerFire_End();
	bool ServerFire_End_Validate();
	void ServerFire_End_Implementation();

	UFUNCTION(NetMulticast,Reliable,WithValidation)
	void MultiFire_End();
	bool MultiFire_End_Validate();
	void MultiFire_End_Implementation();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	/************************ 행동 ************************/
	UFUNCTION()
	void MoveForward(float Value); ////// 속도

	UFUNCTION()
	void MoveRight(float Value);  //// 방향

	void Turn(float Value); //// 회전

	void LookUp(float Value); //// 상하

	void Jump_Start(); //// 점프 시작

	void Jump_End(); //// 점프 끝

	UPROPERTY(VisibleAnywhere,Category="Movement")
	bool bJump;
	
	void Walk_Start(); //// 걷기
	
	void Walk_End();

	bool bWalking;

	void Sprint_Start(); //// 달리기

	void Sprint_End();

	FHitResult LineTraceSingle(const FVector& StartLocation,const FVector& EndLocation); /// 라인트레이스

	void LineTrace(); /// 라인트레이스

	bool bTextCleard;

	void Interact(); // 상호작용

	void Equip_Weapon(class AWeapon_Base* WeaponToEquip);

	class AWeapon_Base* SpawnDefaultWeapon();

	UFUNCTION()
	void Fire();

	void Fire_Start();

	UFUNCTION()
	void Fire_End();

	UFUNCTION()
	void Aiming();

	UFUNCTION()
	void Aiming_End();

	UFUNCTION()
	void Reload();

	UFUNCTION(BlueprintCallable)
	void Reload_End();

	void Throw_Ready();

	void Throw();

	UFUNCTION(BlueprintCallable)
	void Spawn_Grenade();

	void Equip_Rifle(); // 라이플 장착

	void UnEquip(); // 무기 넣기

	void Die();

private:
	class AZombie_Base* Zombie_Base;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AZombie_Base> Zombies;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Combat",meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon_Base> DefaultWeaponClass;
	/************************ 카메라 ***********************/
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Camera", meta = (AllowPrivateAccess = true))
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Camera", meta = (AllowPrivateAccess = true))
	class UCameraComponent* CameraComponent;
	/**************************** 몸 ***********************/
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Body", meta = (AllowPrivateAccess = true))
	class USkeletalMeshComponent* Body_Mesh;

	class UAIPerceptionStimuliSourceComponent* AIPerceptionStimuliSourceComponent;
	/**************************** 사운드 ***********************/
	class UAudioComponent* AudioComponent;

	class USoundCue* React_Cue;
	/************************ 스탯 / 상태***********************/
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "State", meta = (AllowPrivateAccess = true))
	EWeaponState Weapon_State;

	EState State;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Stats", meta = (AllowPrivateAccess = true))
	float Health;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Stats", meta = (AllowPrivateAccess = true))
	float MaxHealth;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool bDie;

	FGenericTeamId TeamId;

	UPROPERTY(BlueprintReadOnly,meta = (AllowPrivateAccess = true))
	bool bFocus;
	/************************ 애니메이션 ************************/
	class UMain_AnimInstance* AnimInstance;
	
	UAnimMontage* Equip_Rifle_Montage; ///라이플 장착
 
	UAnimMontage* UnEquip_Rifle_Montage; /// 라이플 넣기

	UAnimMontage* Rifle_Aiming_Montage; /// 라이플 조준

	UAnimMontage* Rifle_Firing_Montage; /// 라이플 사격

	UAnimMontage* Rifle_Relaoding_Montage; /// 라이플 장전

	UAnimMontage* Throw_Loop; /// 수류탄 준비

	UAnimMontage* Throw_Far; /// 수류탄 멀리

	UAnimMontage* Throw_Close; /// 수류탄 가까이

	UAnimMontage* React_Montage; /// 피격 모션

	UAnimMontage* Die_Montage; /// 죽는 모션
	/************************ 인벤토리 ************************/
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Inventory",meta = (AllowPrivateAccess = true))
	int32 Inventory_Capacity;
	/************************ 무기 ************************/
	UPROPERTY(Replicated)
	bool bFire;

	UPROPERTY(BlueprintReadOnly,meta = (AllowPrivateAccess = true))
	bool bReloading;

	UPROPERTY(BlueprintReadOnly,Replicated,meta = (AllowPrivateAccess = true))
	bool bAiming;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta = (AllowPrivateAccess = true))
	class AWeapon_Base* EquippedWeapon;
	
	/************************ 라이플 ************************/
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Replicated,meta = (AllowPrivateAccess = true))
	bool bEquip_Rifle; // 라이플 장착 확인

	FTimerHandle Rifle_Timer;

	FTimerHandle Rifle_Fire_Timer;
	/************************ 수류탄 ************************/
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Weapon",meta = (AllowPrivateAccess = true))
	TSubclassOf<AGrenade> Grenade;

	UPROPERTY(BlueprintReadOnly,meta = (AllowPrivateAccess = true))
	bool bIsGrenade;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	USpringArmComponent* GetSpringArm() const {return SpringArmComponent;}

	UCameraComponent* GetCamera() const {return CameraComponent;}
	
	UFUNCTION(BlueprintImplementableEvent)
	void Get_Actor_Name(const FText& Text); /// 액터 이름 얻어오기
	
	EWeaponState Get_Weapon_State() const 	{ return Weapon_State; }

	void Set_Weapon_State(EWeaponState Set_State)	{ this->Weapon_State = Set_State; }

	EState Get_State() const { return State; }

	void SetState(EState Set_State) { this->State = Set_State;}

	bool GetAiming() const {return bAiming;}

	bool GetDie() const {return bDie;}

	float GetHealth() const {return Health;}
};
