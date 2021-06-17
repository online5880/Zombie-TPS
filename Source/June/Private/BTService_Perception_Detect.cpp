// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Perception_Detect.h"

#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "Main.h"
#include "Zombie_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_Perception_Detect::UBTService_Perception_Detect()
{
	NodeName = "Detect Node";
	Interval = 1.f;
}

void UBTService_Perception_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	/*auto Pawn = OwnerComp.GetAIOwner()->GetPawn();
	if(!Pawn) return;
	
	if(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AZombie_AIController::TargetKey))
	{
		auto Target = Cast<AMain>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AZombie_AIController::TargetKey));
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AZombie_AIController::TargetLocationKey,Target->GetActorLocation());
	}*/
	
	auto Control_Pawn = OwnerComp.GetAIOwner()->GetPawn();
	if(Control_Pawn == nullptr) return;

	auto Zombie_AI = Cast<AZombie_AIController>(OwnerComp.GetAIOwner());
	if(Zombie_AI == nullptr) return;;

	UWorld* World = Control_Pawn->GetWorld();
	FVector Center = Control_Pawn->GetActorLocation();
	float Detect_Radius = 800.f; //Zombie_AI->Detect_Radius;

	if(World == nullptr) return;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, Control_Pawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(Detect_Radius),
		CollisionQueryParam
	);

	if(bResult)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AZombie_AIController::TargetKey,nullptr);
		
		for(auto OverlapResult : OverlapResults)
		{
			AMain* Main = Cast<AMain>(OverlapResult.GetActor());
			if(Main && Main->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AZombie_AIController::TargetKey,Main);
				return;
			}
		}
	}
	//DrawDebugSphere(World,Center,Detect_Radius,16,FColor::Purple,false,3.f);
}
