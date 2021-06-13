// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Grenade.h"
#include "Weapon_Base.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Main.generated.h"

UENUM(BlueprintType)
enum class EState : uint8
{
	Normal UMETA(DisplayName = "Normal"),
	Rifle UMETA(DisplayName = "Rifle"),
	Pistol UMETA(DisplayName = "Pisol"),
};

UCLASS()
class JUNE_API AMain : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMain();

	/************************ 카메라 ***********************/
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Camera")
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Camera")
	class UCameraComponent* CameraComponent;
	/************************ 몸 ***********************/
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Body")
	class USkeletalMeshComponent* Body_Mesh;
	/************************ 스탯 / 상태***********************/
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "State")
	EState Weapon_State;

	EState Get_Weapon_State() const 	{ return Weapon_State; }

	void Set_Weapon_State(EState Set_State)	{ this->Weapon_State = Set_State; }

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Stats")
	float Health;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Stats")
	float MaxHealth;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
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

	FHitResult LineTraceSingle(const FVector& StartLocation,const FVector& EndLocation); // 라인트레이스

	void LineTrace(); // 라인트레이스

	UFUNCTION(BlueprintImplementableEvent)
	void Get_Actor_Name(const FText& Text); // 액터 이름 얻어오기

	bool bTextCleard;

	void Interact(); // 상호작용

	bool bFocus;
	/************************ 인벤토리 ************************/
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Inventory")
	int32 Inventory_Capacity;
	/************************ 무기 ************************/
	bool bFire;
	
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

	UPROPERTY(BlueprintReadOnly)
	bool bReloading;

	UPROPERTY(BlueprintReadOnly)
	bool bAiming;
	
	void UnEquip(); // 무기 넣기

	UPROPERTY(BlueprintReadOnly)
	class AWeapon_Base* Weapon_Base;
	/************************ 라이플 ************************/
	void Equip_Rifle(); // 라이플 장착

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	bool bEquip_Rifle; // 라이플 장착 확인

	FTimerHandle Rifle_Timer;

	FTimerHandle Rifle_Fire_Timer;
	/************************ 수류탄 ************************/
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Weapon")
	TSubclassOf<AGrenade> Grenade;

	void Throw_Ready();

	void Throw();

	UFUNCTION(BlueprintCallable)
	void Spawn_Grenade();
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
};
