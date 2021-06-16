// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AISense_Sight.h"

#include "Zombie_AIController.generated.h"

/**
 * 
 */
UCLASS()
class JUNE_API AZombie_AIController : public AAIController
{
	GENERATED_BODY()

	private:

	AZombie_AIController();

	class UBlackboardData* Zombie_BB;

	class UBehaviorTree* Zombie_BT;

	virtual void OnPossess(APawn* InPawn) override;

	public:
	static const FName HomePosKey;

	static const FName PatrolPosKey;
	/************************ AI *************************/

	
};
