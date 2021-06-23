// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_Noise.h"

#include "AIController.h"
#include "Zombie_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTaskNode_Noise::UBTTaskNode_Noise()
{
	NodeName = "Noise Node";
}

EBTNodeResult::Type UBTTaskNode_Noise::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Pawn = OwnerComp.GetAIOwner()->GetPawn();
	if(!Pawn) return EBTNodeResult::Failed;
	
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(AZombie_AIController::NoiseKey,false);
	return EBTNodeResult::Succeeded;
}
