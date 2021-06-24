// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile_Base.h"

#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AProjectile_Base::AProjectile_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	//SetReplicates(true);

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	CapsuleComponent->SetCollisionObjectType(ECC_PhysicsBody);
	//CapsuleComponent->SetRelativeRotation(FRotator(0.f,-90.f,0.f));
	
	CapsuleComponent->SetCapsuleHalfHeight(3.f);
	CapsuleComponent->SetCapsuleRadius(0.5f);
	RootComponent = CapsuleComponent;

	Bullet = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bullet"));
	Bullet->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//Bullet->SetRelativeRotation(FRotator(0.f,-90.f,0.f));

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));

	SetReplicates(true);

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>
	Normal(TEXT("NiagaraSystem'/Game/Realistic_Starter_VFX_Pack_Niagara/Niagara/Impact/NS_Impact_Rock.NS_Impact_Rock'"));
	if(Normal.Succeeded())
	{
		Normal_Impact = Normal.Object;
	}
}

void AProjectile_Base::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor != this)
	{
		//GEngine->AddOnScreenDebugMessage(-1,1.5f,FColor::Red, FString::Printf(TEXT("Hit Actor : %s"),*Hit.GetActor()->GetName()),false);
	}
}

// Called when the game starts or when spawned
void AProjectile_Base::BeginPlay()
{
	Super::BeginPlay();

	CapsuleComponent->OnComponentHit.AddDynamic(this,&AProjectile_Base::OnHit);
}