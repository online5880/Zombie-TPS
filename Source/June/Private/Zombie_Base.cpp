// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie_Base.h"
#include "Zombie_AnimInstance.h"
#include "Components/AudioComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

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

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	AudioComponent->SetActive(false);
	AudioComponent->SetupAttachment(RootComponent);

	Tags.Add("Zombie");
	/************************ 애니메이션 ***********************/
	/*static ConstructorHelpers::FObjectFinder<UAnimMontage>
	React(TEXT("AnimMontage'/Game/Zombies/Zombie_1/Anim_Zombie_hit_Montage.Anim_Zombie_hit_Montage'"));
	if(React.Succeeded()) {React_Montage = React.Object;}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
	Die(TEXT("AnimMontage'/Game/Zombies/Zombie_1/Anim_Zombie_deathspawn_Montage.Anim_Zombie_deathspawn_Montage'"));
	if(Die.Succeeded()) {Die_Montage = Die.Object;}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
	Impact_Die(TEXT("AnimMontage'/Game/Zombies/Zombie_1/Anim_Zombie_deathimpact_Montage.Anim_Zombie_deathimpact_Montage'"));
	if(Impact_Die.Succeeded()) {Impact_Die_Montage = Impact_Die.Object;}*/
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

	/*static ConstructorHelpers::FObjectFinder<USoundCue>
	S_Die(TEXT("AnimMontage'/Game/Zombies/Zombie_1/Anim_Zombie_deathspawn_Montage.Anim_Zombie_deathspawn_Montage'"));
	if(S_Die.Succeeded()) {React_Sound = S_Die.Object;}*/
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
		Die();
	}
	else
	{
		if(!bDie)
		{
			Health -= DamageAmount;
			AudioComponent->SetSound(React_Sound);
			AudioComponent->Play();
			if(!AnimInstance->IsAnyMontagePlaying())
			{
				AnimInstance->Montage_Play(React_Montage);
			}
		}
	}
	return DamageAmount;
}

void AZombie_Base::Die()
{
	GetCharacterMovement()->SetActive(false);
	if(bDamaged_Leg)
	{
		AnimInstance->Montage_Play(Ground_Die_Montage);
	}
	else
	{
		AnimInstance->Montage_Play(Die_Montage);
	}
	FTimerHandle Destroy_Timer;
	GetWorld()->GetTimerManager().SetTimer(Destroy_Timer,[this]()
	{
		Destroy();
	},3.2f,false);
}

