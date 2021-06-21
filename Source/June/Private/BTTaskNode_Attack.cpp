// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_Attack.h"

#include "AIController.h"
#include "Zombie_Base.h"

UBTTaskNode_Attack::UBTTaskNode_Attack()
{
	bNotifyTick = true;
	bIsAttacking = false;
	NodeName = "Attack";
}

EBTNodeResult::Type UBTTaskNode_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	Zombie_Base = Cast<AZombie_Base>(OwnerComp.GetAIOwner()->GetPawn());
	if(!Zombie_Base) return EBTNodeResult::Failed;

	if(!bIsAttacking)
	{
		Zombie_Base->Attack();
		bIsAttacking = true;
	}

	return EBTNodeResult::InProgress;
}

void UBTTaskNode_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	bIsAttacking = Zombie_Base->bIsAttacking;
	if(!bIsAttacking)
	{
		FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
	}
}
