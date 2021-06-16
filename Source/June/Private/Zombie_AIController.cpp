// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

const FName AZombie_AIController::HomePosKey(TEXT("HomePos"));
const FName AZombie_AIController::PatrolPosKey(TEXT("PatrolPos"));

AZombie_AIController::AZombie_AIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData>
	BB(TEXT("BlackboardData'/Game/Zombies/AI/Zombie_BB.Zombie_BB'"));
	if(BB.Succeeded()) {Zombie_BB = BB.Object;}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree>
	BT(TEXT("BehaviorTree'/Game/Zombies/AI/Zombie_BT.Zombie_BT'"));
	if(BT.Succeeded()) {Zombie_BT = BT.Object;}
}

void AZombie_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	RunBehaviorTree(Zombie_BT);
	if(UseBlackboard(Zombie_BB,Blackboard))
	{
		Blackboard->SetValueAsVector(HomePosKey,InPawn->GetActorLocation());
	}
}
