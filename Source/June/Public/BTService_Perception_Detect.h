// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"


#include "BTService_Perception_Detect.generated.h"

/**
 * 
 */
UCLASS()
class JUNE_API UBTService_Perception_Detect : public UBTService
{
	GENERATED_BODY()

	public:

	UBTService_Perception_Detect();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
