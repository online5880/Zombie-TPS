// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Sound/SoundCue.h"

#include "Zombie_Base.generated.h"
UENUM(BlueprintType)
enum class EZobime_State : uint8
{
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
	/************************ 정보 *************************/
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Status")
	EZobime_State Zombie_State;

	EZobime_State Get_Zombie_State() const
	{
		return Zombie_State;
	}

	void SetZombie_State(EZobime_State State)
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

	void Die();

	FTimerHandle Destroy_Timer;

	void Ragdoll();

	FTimerHandle Ragdoll_Timer;
	/************************ 사운드 ***********************/
	UPROPERTY(EditAnywhere)
	class UAudioComponent* AudioComponent;

	class USoundAttenuation* Zombie_Attenuation;

	class USoundCue* Idle_Sound;

	class USoundCue* React_Sound;

	class USoundCue* Die_Sound;
	/************************ 애니메이션 ***********************/
	class UZombie_AnimInstance* AnimInstance;

	class UAnimMontage* React_Montage;

	class UAnimMontage* Die_Montage[2];

	class UAnimMontage* Impact_Die_Montage;

	class UAnimMontage* Ground_Die_Montage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
