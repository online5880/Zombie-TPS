// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_Noise.generated.h"

/**
 * 
 */
UCLASS()
class JUNE_API UBTTaskNode_Noise : public UBTTaskNode
{
	GENERATED_BODY()

	UBTTaskNode_Noise();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
