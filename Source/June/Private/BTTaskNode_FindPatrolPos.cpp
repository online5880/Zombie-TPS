// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_FindPatrolPos.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "Zombie_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTaskNode_FindPatrolPos::UBTTaskNode_FindPatrolPos()
{
	NodeName = TEXT("Find Patrol Pos");
}

EBTNodeResult::Type UBTTaskNode_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Pawn = OwnerComp.GetAIOwner()->GetPawn();
	if(!Pawn) { return EBTNodeResult::Failed;}

	UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetNavigationSystem(Pawn->GetWorld());
	if(!NavigationSystem) {return EBTNodeResult::Failed;}

	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AZombie_AIController::HomePosKey);
	FNavLocation NextPos;

	if(NavigationSystem->GetRandomReachablePointInRadius(Origin,2000.f,NextPos))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AZombie_AIController::PatrolPosKey,NextPos.Location);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
