// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie_Base.h"

// Sets default values
AZombie_Base::AZombie_Base()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add("Zombie");
}

// Called when the game starts or when spawned
void AZombie_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AZombie_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AZombie_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

