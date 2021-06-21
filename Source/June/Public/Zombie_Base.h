// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GenericTeamAgentInterface.h"
#include "GameFramework/Character.h"
#include "Sound/SoundCue.h"

#include "Zombie_Base.generated.h"
DECLARE_DELEGATE(FDele_AttackEnd)

UENUM(BlueprintType)
enum class EZombie_State : uint8
{
	Patrol UMETA(DisplayName = "Patrol"),
	Chase UMETA(DisplayName = "Chase"),
	Normal UMETA(DisplayName = "Normal"),
	Ground UMETA(DisplayName = "Ground"),
	Dead UMETA(DisplayName = "Dead"),
};

UCLASS()
class JUNE_API AZombie_Base : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZombie_Base();

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Body Mesh")
	class USkeletalMeshComponent* Body_Mesh;
	/************************ AI *************************/
	/*UPROPERTY(EditAnywhere,BlueprintReadOnly,Category ="AI")
	class UAIPerceptionComponent* Zombie_AIPerception;*/

	class AZombie_AIController* Zombie_AIController;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="AI")
	bool bTarget;
	
	FDele_AttackEnd AttackEnd;
	
	void Attack();

	UFUNCTION(BlueprintCallable)
	void Attack_End();
	
	bool bIsAttacking;
	/************************ 정보 *************************/
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Status")
	EZombie_State Zombie_State;

	EZombie_State Get_Zombie_State() const
	{
		return Zombie_State;
	}

	void SetZombie_State(EZombie_State State)
	{
		this->Zombie_State = State;
	}

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Status")
	float Max_Health;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Status")
	float Health;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Status")
	float Leg_Health;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Status")
	bool bDamaged_Leg;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Status")
	bool bDie;

	bool bGrenade_Die;

	void Die();

	FTimerHandle Destroy_Timer;

	void Ragdoll(float Time);

	FTimerHandle Ragdoll_Timer;

	void Turn_Target(FVector Target);
	/************************ 사운드 ***********************/
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Audio")
	class UAudioComponent* AudioComponent;

	class USoundAttenuation* Zombie_Attenuation;

	class USoundCue* Idle_Sound;

	class USoundCue* React_Sound;

	class USoundCue* Die_Sound;

	class USoundCue* Attack_Sound;
	/************************ 애니메이션 ***********************/
	class UZombie_AnimInstance* AnimInstance;

	class UAnimMontage* React_Montage;
	
	class UAnimMontage* Impact_Die_Montage;

	class UAnimMontage* Ground_Die_Montage;

	class UAnimMontage* Attack_Montage;

	class UAnimMontage* Die_Montage[3];

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
