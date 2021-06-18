// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie_Base.h"
#include "Zombie_AnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "Zombie_AIController.h"

// Sets default values
AZombie_Base::AZombie_Base()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Max_Health = 200.f;
	Health = 100.f;
	Leg_Health = 100.f;
	bDamaged_Leg = false;
	bDie = false;
	bGrenade_Die = false;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	AudioComponent->SetActive(false);
	AudioComponent->SetupAttachment(RootComponent);

	AIControllerClass = AZombie_AIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	//Zombie_AIController = Cast<AZombie_AIController>(GetController());

	Tags.Add("Zombie");
	/************************ 애니메이션 ***********************/
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
	React(TEXT("AnimMontage'/Game/Zombies/Zombie_1/Anim/Anim_Zombie_hit_Montage.Anim_Zombie_hit_Montage'"));
	if(React.Succeeded()) {React_Montage = React.Object;}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
	Impact_Die(TEXT("AnimMontage'/Game/Zombies/Zombie_1/Anim/Anim_Zombie_deathimpact_Montage.Anim_Zombie_deathimpact_Montage'"));
	if(Impact_Die.Succeeded()) {Impact_Die_Montage = Impact_Die.Object;}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
	Die_1(TEXT("AnimMontage'/Game/Zombies/Zombie_1/Anim/Anim_Zombie_deathspawn_Montage.Anim_Zombie_deathspawn_Montage'"));
	if(Die_1.Succeeded()) {Die_Montage[1] = Die_1.Object;}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
	Die_2(TEXT("AnimMontage'/Game/Zombies/Zombie_1/Anim/ZombieDeath4UE4_Montage.ZombieDeath4UE4_Montage'"));
	if(Die_2.Succeeded()) {Die_Montage[2] = Die_2.Object;}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
	Die_3(TEXT("AnimMontage'/Game/Zombies/Zombie_1/Anim/ZombieDying4UE4_Montage.ZombieDying4UE4_Montage'"));
	if(Die_3.Succeeded()) {Die_Montage[3] = Die_3.Object;}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
	Ground_Die(TEXT("AnimMontage'/Game/Zombies/Zombie_1/Anim/Anim_Zombie_ground-death_Montage.Anim_Zombie_ground-death_Montage'"));
	if(Ground_Die.Succeeded()) {Ground_Die_Montage = Ground_Die.Object;}
	/************************ 사운드 ***********************/
	static ConstructorHelpers::FObjectFinder<USoundCue>
	S_React(TEXT("SoundCue'/Game/Zombies/Zombie_1/Sound/React.React'"));
	if(S_React.Succeeded()) {React_Sound = S_React.Object;}

	static ConstructorHelpers::FObjectFinder<USoundCue>
	S_Idle(TEXT("SoundCue'/Game/Zombies/Zombie_1/Sound/Idle.Idle'"));
	if(S_Idle.Succeeded()) {Idle_Sound = S_Idle.Object;}

	static ConstructorHelpers::FObjectFinder<USoundAttenuation>
	S_Att(TEXT("SoundAttenuation'/Game/Zombies/Zombie_1/Sound/Zombie_Ambi.Zombie_Ambi'"));
	if(S_Att.Succeeded()) {Zombie_Attenuation = S_Att.Object;}
}
// Called when the game starts or when spawned
void AZombie_Base::BeginPlay()
{
	Super::BeginPlay();
	AnimInstance = Cast<UZombie_AnimInstance>(GetMesh()->GetAnimInstance());
	AudioComponent->SetSound(Idle_Sound);
	AudioComponent->Play();
}

// Called every frame
void AZombie_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(Leg_Health==0.f)
	{
		bDamaged_Leg =true;
	}

}

// Called to bind functionality to input
void AZombie_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AZombie_Base::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if(Health - DamageAmount <= 0.f && !bDie)
	{
		Health -= DamageAmount;
		bDie = true;
		if(DamageAmount >= 200.f && !bDamaged_Leg)
		{
			bGrenade_Die = true;
			Turn_Target(DamageCauser->GetActorLocation());
		}
		Die();
	}
	else
	{
		if(!bDie)
		{
			/*class AZombie_AIController* Zombie_AIController;
			Zombie_AIController = Cast<AZombie_AIController>(GetController());*/
			
			Health -= DamageAmount;
			AudioComponent->SetSound(React_Sound);
			AudioComponent->Play();
			if(!AnimInstance->IsAnyMontagePlaying())
			{
				AnimInstance->Montage_Play(React_Montage);
			}
			GetCharacterMovement()->MaxWalkSpeed = 300.f;

			//Zombie_AIController->GetBlackboardComponent()->SetValueAsObject(AZombie_AIController::TargetKey,DamageCauser);
			FTimerHandle Reset_Timer;
			GetWorld()->GetTimerManager().SetTimer(Reset_Timer,[this]()
			{
				
			},10.f,false);
			
			FTimerHandle Speed_Timer;
			GetWorld()->GetTimerManager().SetTimer(Speed_Timer,[this]()
			{
				GetCharacterMovement()->MaxWalkSpeed = 30.f;
			},10.f,false);
		}
	}
	return DamageAmount;
}

void AZombie_Base::Die()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->SetActive(false);
	AudioComponent->DestroyComponent();
	if(bDamaged_Leg)
	{
		AnimInstance->Montage_Play(Ground_Die_Montage,0.7f);
	}
	else
	{
		if(bGrenade_Die)
		{
			AnimInstance->Montage_Play(Impact_Die_Montage);
			Ragdoll(0.25f);
		}
		else
		{
			int32 Rand = UKismetMathLibrary::RandomIntegerInRange(1,3);
			AnimInstance->Montage_Play(Die_Montage[Rand]);
			Ragdoll(0.9f);
		}
	}
	GetWorld()->GetTimerManager().SetTimer(Destroy_Timer,[this]()
	{
		Destroy();
		GetWorld()->GetTimerManager().ClearTimer(Destroy_Timer);
	},10.f,false);
}

void AZombie_Base::Ragdoll(float Time)
{
	GetWorld()->GetTimerManager().SetTimer(Ragdoll_Timer,[this]()
	{
		GetMesh()->SetCollisionResponseToChannel(ECC_Pawn,ECR_Block);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetCollisionObjectType(ECC_PhysicsBody);
		GetWorld()->GetTimerManager().ClearTimer(Ragdoll_Timer);
	},Time,false);
}

void AZombie_Base::Turn_Target(FVector Target)
{
	FRotator Rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),Target);
	SetActorRotation(Rot);
}

