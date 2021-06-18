// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_Can_Attack.generated.h"

/**
 * 
 */
UCLASS()
class JUNE_API UBTDecorator_Can_Attack : public UBTDecorator
{
	GENERATED_BODY()

	public:

	UBTDecorator_Can_Attack();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
