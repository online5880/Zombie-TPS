// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Perception/AISense_Hearing.h"

// Sets default values
AGrenade::AGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	Body_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body Mesh"));
	RootComponent = Body_Mesh;

	Projectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	Projectile->bShouldBounce = true;

	TeamId = FGenericTeamId(0);
	Tags.Add("Player");
}

FGenericTeamId AGrenade::GetGenericTeamId() const
{
	return TeamId;
}

void AGrenade::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
                     FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor != this)
	{
		//GEngine->AddOnScreenDebugMessage(-1,1.5f,FColor::Red, FString::Printf(TEXT("수류탄")));
		FTimerHandle Noise_Timer;
		GetWorld()->GetTimerManager().SetTimer(Noise_Timer,[this]()
		{
			UAISense_Hearing::ReportNoiseEvent(GetWorld(),GetActorLocation(),1.f,this,0.f,FName("Weapon Noise"));
		},0.9f,false);
	}
}

// Called when the game starts or when spawned
void AGrenade::BeginPlay()
{
	Super::BeginPlay();

	Body_Mesh->OnComponentHit.AddDynamic(this,&AGrenade::OnHit);
}
