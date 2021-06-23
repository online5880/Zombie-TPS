// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
DECLARE_DELEGATE(FDelegate_Reset);
#include "CoreMinimal.h"
#include "AIController.h"
#include "Main.h"
#include "Zombie_Base.h"
#include "Perception/AISense_Sight.h"

#include "Zombie_AIController.generated.h"
/**
 * 
 */
UCLASS()
class JUNE_API AZombie_AIController : public AAIController
{
	GENERATED_BODY()

	public:

	UPROPERTY(BlueprintReadOnly)
	class AZombie_Base* Zombie_Base;

	UFUNCTION()
	void Reset_Target();

	FDelegate_Reset Delegate_Reset;

	private:

	AZombie_AIController();

	class UBlackboardData* Zombie_BB;

	class UBehaviorTree* Zombie_BT;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void Tick(float DeltaSeconds) override;
	
	virtual void BeginPlay() override;

	public:
	static const FName HomePosKey;

	static const FName PatrolPosKey;

	static const FName TargetKey;

	static const FName TargetLocationKey;

	static const FName NoiseKey;
	/************************ AI *************************/
	

};
