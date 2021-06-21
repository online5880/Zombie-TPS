// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_TurnToTarget.h"

#include "AIController.h"
#include "Main.h"
#include "Zombie_AIController.h"
#include "Zombie_Base.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTaskNode_TurnToTarget::UBTTaskNode_TurnToTarget()
{
	NodeName = "Turn To Target";
}

EBTNodeResult::Type UBTTaskNode_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Zombie = Cast<AZombie_Base>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Zombie)
		return EBTNodeResult::Failed;

	auto Target = Cast<AMain>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AZombie_AIController::TargetKey));
	if (nullptr == Target)
		return EBTNodeResult::Failed;

	FVector LookVector = Target->GetActorLocation() - Zombie->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	Zombie->SetActorRotation(FMath::RInterpTo(Zombie->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.f));

	return EBTNodeResult::Succeeded;
}
