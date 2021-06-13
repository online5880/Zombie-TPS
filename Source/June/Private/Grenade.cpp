// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"

// Sets default values
AGrenade::AGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	Body_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body Mesh"));
	RootComponent = Body_Mesh;

	Projectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	Projectile->bShouldBounce = true;

}

// Called when the game starts or when spawned
void AGrenade::BeginPlay()
{
	Super::BeginPlay();
	
}
