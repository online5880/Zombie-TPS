// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"

const FName AZombie_AIController::HomePosKey(TEXT("HomePos"));
const FName AZombie_AIController::PatrolPosKey(TEXT("PatrolPos"));
const FName AZombie_AIController::TargetKey(TEXT("Target"));
const FName AZombie_AIController::TargetLocationKey(TEXT("TargetLocation"));

AZombie_AIController::AZombie_AIController()
{
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<UBlackboardData>
	BB(TEXT("BlackboardData'/Game/Zombies/AI/Zombie_BB.Zombie_BB'"));
	if(BB.Succeeded()) {Zombie_BB = BB.Object;}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree>
	BT(TEXT("BehaviorTree'/Game/Zombies/AI/Zombie_BT.Zombie_BT'"));
	if(BT.Succeeded()) {Zombie_BT = BT.Object;}

	SetGenericTeamId(FGenericTeamId(1));
}

void AZombie_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Zombie_Base = Cast<AZombie_Base>(InPawn);
	
	RunBehaviorTree(Zombie_BT);
	if(UseBlackboard(Zombie_BB,Blackboard))
	{
		Blackboard->SetValueAsVector(HomePosKey,InPawn->GetActorLocation());
	}
}

void AZombie_AIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AZombie_AIController::BeginPlay()
{
	Super::BeginPlay();

	Main = Cast<AMain>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
}
