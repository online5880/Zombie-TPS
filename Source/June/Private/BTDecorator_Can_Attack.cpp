// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_Can_Attack.h"

#include "AIController.h"
#include "Main.h"
#include "Zombie_AIController.h"
#include "Zombie_Base.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_Can_Attack::UBTDecorator_Can_Attack()
{
	NodeName = TEXT("Can Attack");
}

bool UBTDecorator_Can_Attack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto Pawn = Cast<AZombie_Base>(OwnerComp.GetAIOwner()->GetPawn());
	if(Pawn == nullptr) return false;

	auto Target = Cast<AMain>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AZombie_AIController::TargetKey));
	if(Target == nullptr) return false;

	bResult = (Target->GetDistanceTo(Pawn) <= 150.f);
	return bResult;
}
