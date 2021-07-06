// Fill out your copyright notice in the Description page of Project Settings.


#include "June/Public/Main.h"
#include "DrawDebugHelpers.h"
#include "Interact_Interface.h"
#include "Main_AnimInstance.h"
#include "Item_Base.h"
#include "Zombie_AIController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/AudioComponent.h"
#include "Net/UnrealNetwork.h"
#include "Perception/AISense_Hearing.h"
#include "Sound/SoundCue.h"

// Sets default values
AMain::AMain()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bEquip_Rifle = false;
	bAiming = false;
	bFocus = false;
	bWalking = false;
	bIsGrenade = false;
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 300.f;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent,USpringArmComponent::SocketName);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	AudioComponent->SetupAttachment(RootComponent);

	AIPerceptionStimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AIPerceptionStimuliSourceComponent"));
	AIPerceptionStimuliSourceComponent->bAutoRegister;

	Inventory_Capacity = 10;

	Health = 50.f;
	MaxHealth = 100.f;

	Tags.Add("Player");
	TeamId = FGenericTeamId(0);

	SetReplicates(true);
	bReplicates = true;
	
	/************************ 애니메이션 ************************/
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
	Equip_Rifle(TEXT("AnimMontage'/Game/Main/Anim/Rifle/IP/EquipRifle_Montage.EquipRifle_Montage'"));
	if(Equip_Rifle.Succeeded())	{	Equip_Rifle_Montage = Equip_Rifle.Object;	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
	UnEquip_Rifle(TEXT("AnimMontage'/Game/Main/Anim/Rifle/IP/HolsterRifle_Montage.HolsterRifle_Montage'"));
	if(UnEquip_Rifle.Succeeded())	{	UnEquip_Rifle_Montage = UnEquip_Rifle.Object;	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
	Rifle_Aiming(TEXT("AnimMontage'/Game/Main/Anim/Rifle/IP/Rifle_Aiming_Montage.Rifle_Aiming_Montage'"));
	if(Rifle_Aiming.Succeeded())	{	Rifle_Aiming_Montage = Rifle_Aiming.Object;	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
	Rifle_Fire(TEXT("AnimMontage'/Game/Main/Anim/Rifle/IP/Rifle_ShootLoop_Montage.Rifle_ShootLoop_Montage'"));
	if(Rifle_Fire.Succeeded())	{	Rifle_Firing_Montage = Rifle_Fire.Object;	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
	Rifle_Reload(TEXT("AnimMontage'/Game/Main/Anim/Rifle/IP/Rifle_Reload_Montage.Rifle_Reload_Montage'"));
	if(Rifle_Reload.Succeeded())	{	Rifle_Relaoding_Montage = Rifle_Reload.Object;	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
	Grenade_Ready(TEXT("AnimMontage'/Game/Main/Anim/Rifle/IP/Rifle_Grenade_Throw_Loop_Montage.Rifle_Grenade_Throw_Loop_Montage'"));
	if(Grenade_Ready.Succeeded())	{	Throw_Loop = Grenade_Ready.Object;	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
	Grenade_Far(TEXT("AnimMontage'/Game/Main/Anim/Rifle/IP/Rifle_Grenade_Throw_Far_Montage.Rifle_Grenade_Throw_Far_Montage'"));
	if(Grenade_Far.Succeeded())	{	Throw_Far = Grenade_Far.Object;	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
	Grenade_Close(TEXT("AnimMontage'/Game/Main/Anim/Rifle/IP/Rifle_Grenade_Throw_Close_Montage.Rifle_Grenade_Throw_Close_Montage'"));
	if(Grenade_Close.Succeeded())	{	Throw_Close = Grenade_Close.Object;	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
	React(TEXT("AnimMontage'/Game/Main/Anim/Rifle/IP/Rifle_React_Montage.Rifle_React_Montage'"));
	if(React.Succeeded()) { React_Montage = React.Object;}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
	Die(TEXT("AnimMontage'/Game/Main/Anim/Rifle/IP/Rifle_Death_Montage.Rifle_Death_Montage'"));
	if(Die.Succeeded()) { Die_Montage = Die.Object;}
}
FGenericTeamId AMain::GetGenericTeamId() const
{
	return TeamId;
}

// Called when the game starts or when spawned
void AMain::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance = Cast<UMain_AnimInstance>(GetMesh()->GetAnimInstance());

	Set_Weapon_State(EWeaponState::Normal);

	Zombie_Base = Cast<AZombie_Base>(UGameplayStatics::GetActorOfClass(GetWorld(),Zombies));
}

// Called every frame
void AMain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(!GetMovementComponent()->IsFalling())
	{
		bJump = true;
	}
	switch (Weapon_State)
	{
		case EWeaponState::Normal:
			bEquip_Rifle = false;
			break;
		case EWeaponState::Rifle:
			bEquip_Rifle = true;
	}
	LineTrace();
}

// Called to bind functionality to input
void AMain::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Interact",IE_Pressed,this,&AMain::Interact);
	PlayerInputComponent->BindAction("Fire",IE_Pressed,this,&AMain::Fire_Start);
	PlayerInputComponent->BindAction("Fire",IE_Released,this,&AMain::Fire_End);
	PlayerInputComponent->BindAction("Aiming",IE_Pressed,this,&AMain::Aiming);
	PlayerInputComponent->BindAction("Aiming",IE_Released,this,&AMain::Aiming_End);
	PlayerInputComponent->BindAction("Reload",IE_Pressed,this,&AMain::Reload);
	PlayerInputComponent->BindAction("Grenade",IE_Pressed,this,&AMain::Throw_Ready);
	PlayerInputComponent->BindAction("Grenade",IE_Released,this,&AMain::Throw);

	PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&AMain::Jump_Start);
	//PlayerInputComponent->BindAction("Jump",IE_Released,this,&AMain::Jump_End);
	PlayerInputComponent->BindAction("Walk",IE_Pressed,this,&AMain::Walk_Start);
	PlayerInputComponent->BindAction("Walk",IE_Released,this,&AMain::Walk_End);
	PlayerInputComponent->BindAction("Sprint",IE_Pressed,this,&AMain::Sprint_Start);
	PlayerInputComponent->BindAction("Sprint",IE_Released,this,&AMain::Sprint_End);
	PlayerInputComponent->BindAction("Rifle",IE_Released,this,&AMain::Equip_Rifle);
	PlayerInputComponent->BindAction("UnEquip",IE_Released,this,&AMain::UnEquip);

	PlayerInputComponent->BindAxis("MoveForward",this,&AMain::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",this,&AMain::MoveRight);
	PlayerInputComponent->BindAxis("Turn",this,&AMain::Turn);
	PlayerInputComponent->BindAxis("LookUp",this,&AMain::LookUp);
}
/******************************************************** 서버 ********************************************************/
bool AMain::ServerWalk_Start_Validate(){return true;}

void AMain::ServerWalk_Start_Implementation()
{
	bWalking = true;
	GetCharacterMovement()->MaxWalkSpeed = 200.f;
}

bool AMain::ServerWalk_End_Validate(){return true;}

void AMain::ServerWalk_End_Implementation()
{
	bWalking = false;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

bool AMain::ServerSprint_Start_Validate(){return true;}

void AMain::ServerSprint_Start_Implementation()
{
	if(GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::W))
	{
		GetCharacterMovement()->MaxWalkSpeed = 700.f;
	}
}

bool AMain::ServerSprint_End_Validate(){return true;}

void AMain::ServerSprint_End_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void AMain::ServerInteract_Implementation()
{
	MultiInteract();
}

bool AMain::ServerInteract_Validate()
{
	return true;
}

bool AMain::MultiInteract_Validate()
{
	return true;
}

void AMain::MultiInteract_Implementation()
{
	FVector Capsule_Location = GetCapsuleComponent()->GetComponentLocation();
	FVector Start = FVector(Capsule_Location.X,Capsule_Location.Y,Capsule_Location.Z+88.f);
	FVector Controller_Vector = UKismetMathLibrary::GetForwardVector(GetControlRotation());
	FVector End = (Start+(Controller_Vector * 250.f));

	if(AActor* HitActor = LineTraceSingle(Start,End).GetActor())
	{
		if(IInteract_Interface* Interact = Cast<IInteract_Interface>(HitActor))
		{
			if(Interact)
			{
				Interact->Interact(this);
			}
		}
	}
}

void AMain::ServerEquipRifle_Implementation()
{
	MultiEquipRifle();
}

bool AMain::ServerEquipRifle_Validate()
{
	return true;
}

bool AMain::MultiEquipRifle_Validate()
{
	return true;
}

void AMain::MultiEquipRifle_Implementation()
{
	TArray<AActor*> Attached_Actors;
	GetAttachedActors(Attached_Actors);
	FString Rifle = "Weapon_Base_BP";
	
	for(auto Actor : Attached_Actors)
	{
		if(Rifle == UKismetSystemLibrary::GetDisplayName(Actor))
		{
			Weapon_Base = Cast<AWeapon_Base>(Actor);
			break;
		}
	}
	
	if(AnimInstance)
	{
		if(!AnimInstance->IsAnyMontagePlaying() && Weapon_Base && !(Get_Weapon_State() == EWeaponState::Rifle))
		{
			AnimInstance->Montage_Play(Equip_Rifle_Montage);			
			GetWorld()->GetTimerManager().SetTimer(Rifle_Timer,[this]()
			{
				Weapon_Base->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetIncludingScale,FName("Weapon_Socket"));
				Set_Weapon_State(EWeaponState::Rifle);
			},0.5f,false);
		}
	}
}

bool AMain::ServerUnEquipRifle_Validate()
{
	return true;
}

void AMain::ServerUnEquipRifle_Implementation()
{
	MultiUnEquipRifle();
}

bool AMain::MultiUnEquipRifle_Validate()
{
	return true;
}

void AMain::MultiUnEquipRifle_Implementation()
{
	TArray<AActor*> Attached_Actors;
	GetAttachedActors(Attached_Actors);
	FString Rifle = "Weapon_Base_BP";
	
	for(auto Actor : Attached_Actors)
	{
		if(Rifle == UKismetSystemLibrary::GetDisplayName(Actor))
		{
			Weapon_Base = Cast<AWeapon_Base>(Actor);
			break;
		}
	}
	
	if(!AnimInstance->IsAnyMontagePlaying() && Weapon_Base && !(Get_Weapon_State() == EWeaponState::Normal))
	{
		switch (Weapon_State)
		{
		case EWeaponState::Rifle:
			AnimInstance->Montage_Play(UnEquip_Rifle_Montage);
			GetWorld()->GetTimerManager().SetTimer(Rifle_Timer,[this]()
			{
				Weapon_Base->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetIncludingScale,FName("Rifle_Socket"));
			},1.1f,false);
			Set_Weapon_State(EWeaponState::Normal);
			break;
		}
	}
}

bool AMain::ServerAiming_Validate(bool ServerAiming)
{
	return true;
}

void AMain::ServerAiming_Implementation(bool ServerAiming)
{
	MultiAiming(ServerAiming);
}

bool AMain::MultiAiming_Validate(bool MultiAiming)
{
	return true;
}

void AMain::MultiAiming_Implementation(bool MultiAiming)
{
	if(!bDie)
	{
		switch (Weapon_State)
		{
		case EWeaponState::Rifle:
			if(!AnimInstance->IsAnyMontagePlaying() && Weapon_Base && !bAiming && !bReloading && !bIsGrenade)
			{
				AnimInstance->Montage_Play(Rifle_Aiming_Montage);
				bAiming = true;
				break;
			}
		}
	}
}

bool AMain::ServerAiming_End_Validate(bool ServerAiming)
{
	return true;
}

void AMain::ServerAiming_End_Implementation(bool ServerAiming)
{
	MultiAiming_End(ServerAiming);
}

bool AMain::MultiAiming_End_Validate(bool MultiAiming)
{
	return true;
}

void AMain::MultiAiming_End_Implementation(bool MultiAiming)
{
	bAiming = false;
	AnimInstance->Montage_Stop(0.1f,Rifle_Aiming_Montage);
}

bool AMain::ServerFire_Validate(bool bServerFire)
{
	return true;
}

void AMain::ServerFire_Implementation(bool bServerFire)
{
	MultiFire(bServerFire);
}

bool AMain::MultiFire_Validate(bool bMultiFire)
{
	return true;
}

void AMain::MultiFire_Implementation(bool bMultiFire)
{
	if(bMultiFire)
	{
		Weapon_Base->Fire(this);
		UAISense_Hearing::ReportNoiseEvent(GetWorld(),GetActorLocation(),1.f,this,0.f,FName("Weapon Noise"));
		GetWorld()->GetTimerManager().SetTimer(Rifle_Fire_Timer,this,&AMain::Fire,0.1f,false);
	}
}

bool AMain::ServerFire_Start_Validate()
{
	return true;
}

void AMain::ServerFire_Start_Implementation()
{
	MultiFire_Start();
}

bool AMain::MultiFire_Start_Validate()
{
	return true;
}

void AMain::MultiFire_Start_Implementation()
{
	if(Get_Weapon_State() == EWeaponState::Rifle && bAiming && !bDie)
	{
		bFire = true;
		AnimInstance->Montage_Play(Rifle_Firing_Montage);
		Fire();
		SetState(EState::Fire);
	}
}

bool AMain::ServerFire_End_Validate()
{
	return true;
}

void AMain::ServerFire_End_Implementation()
{
	MultiFire_End();
}

bool AMain::MultiFire_End_Validate()
{
	return true;
}

void AMain::MultiFire_End_Implementation()
{
	bFire= false;
	AnimInstance->Montage_Stop(0.1f,Rifle_Firing_Montage);
	if(bAiming)
	{
		AnimInstance->Montage_Play(Rifle_Aiming_Montage,1.f,EMontagePlayReturnType::MontageLength,3.f);		
	}
}

void AMain::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMain,bAiming);
	DOREPLIFETIME(AMain,bFire);
}

/******************************************************** 행동 ********************************************************/
void AMain::MoveForward(float Value)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	Direction.Z = 0.f;
	Direction.Normalize();
	AddMovementInput(Direction,Value);
}

void AMain::MoveRight(float Value)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	Direction.Z = 0.f;
	Direction.Normalize();
	AddMovementInput(Direction,Value);
}

void AMain::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void AMain::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void AMain::Jump_Start()
{
	bJump = true;
	if(bJump)
	{
		bPressedJump = true;
	}
}

void AMain::Jump_End()
{
	bPressedJump = false;
}

void AMain::Walk_Start() 
{
	bWalking = true;
	GetCharacterMovement()->MaxWalkSpeed = 200.f;
	if(!HasAuthority())
	{
		ServerWalk_Start();
	}
}

void AMain::Walk_End() 
{
	bWalking = false;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	if(!HasAuthority())
	{
		ServerWalk_End();
	}
}

void AMain::Sprint_Start() 
{
	if(GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::W))
	{
		GetCharacterMovement()->MaxWalkSpeed = 700.f;
	}
	if(!HasAuthority())
	{
		ServerSprint_Start();
	}
}

void AMain::Sprint_End() 
{
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	if(!HasAuthority())
	{
		ServerSprint_End();
	}
}

FHitResult AMain::LineTraceSingle(const FVector& StartLocation, const FVector& EndLocation)
{
	FHitResult OutHit;
	
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(OutHit,StartLocation,EndLocation,ECC_Visibility,CollisionParams);

	return OutHit;
}

void AMain::LineTrace()
{
	FVector Capsule_Location = GetCapsuleComponent()->GetComponentLocation();
	FVector Start = FVector(Capsule_Location.X,Capsule_Location.Y,Capsule_Location.Z+88.f);
	FVector Controller_Vector = UKismetMathLibrary::GetForwardVector(GetControlRotation());
	FVector End = (Start+(Controller_Vector * 250.f));

	if(AActor* HitActor = LineTraceSingle(Start,End).GetActor())
	{
		if(IGetText_Interface* Interface = Cast<IGetText_Interface>(HitActor))
		{
			FString Actor_Name = Interface->Get_Name();
			Get_Actor_Name(FText::FromString(Actor_Name));
			bFocus = true;
		}
		else { Get_Actor_Name(FText::FromString("")); bFocus = false; }
	}
	else { Get_Actor_Name(FText::FromString("")) ; bFocus = false; }
}

void AMain::Interact()
{
	if(HasAuthority())
	{
		MultiInteract();
	}
	else
	{
		ServerInteract();
	}
}

float AMain::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	//GEngine->AddOnScreenDebugMessage(-1,1.5f,FColor::Red, FString::Printf(TEXT("공격받음"),false));

	if(Health - DamageAmount <= 0.f && !bDie)
	{
		Health -= DamageAmount;
		bDie = true;
		Tags.Remove("Player");
		Die();
		if(Zombie_Base)
		{
			Zombie_Base->Zombie_AIController->Reset_Target(); /// Blackboard TargetKey -> nullptr
		}
		return DamageAmount;
	}
	else
	{
		if(!bDie)
		{
			int React_Rand = FMath::RandRange(0,3);

			Health -= DamageAmount;

			if(React_Rand == 0)
			{
				AnimInstance->Montage_Play(React_Montage);
				AnimInstance->Montage_JumpToSection(FName("React_1"),React_Montage);
			}
			if(React_Rand == 1)
			{
				AnimInstance->Montage_Play(React_Montage);
				AnimInstance->Montage_JumpToSection(FName("React_2"),React_Montage);
			}
			if(React_Rand == 2)
			{
				AnimInstance->Montage_Play(React_Montage);
				AnimInstance->Montage_JumpToSection(FName("React_3"),React_Montage);
			}
			if(React_Rand == 3)
			{
				AnimInstance->Montage_Play(React_Montage);
				AnimInstance->Montage_JumpToSection(FName("React_4"),React_Montage);
			}
			SetState(EState::React);

			return DamageAmount;
		}
	}
	return DamageAmount;
}

void AMain::Die()
{
	if(bDie)
	{
		AnimInstance->StopAllMontages(0.1f);
		SetState(EState::Die);
		//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		//GetCharacterMovement()->DisableMovement();
		int Die_Rand = FMath::RandRange(0,3);

		if(Die_Rand == 0)
		{
			AnimInstance->Montage_Play(Die_Montage);
			AnimInstance->Montage_JumpToSection(FName("Death_1"),Die_Montage);
		}
		if(Die_Rand == 1)
		{
			AnimInstance->Montage_Play(Die_Montage);
			AnimInstance->Montage_JumpToSection(FName("Death_2"),Die_Montage);
		}
		if(Die_Rand == 2)
		{
			AnimInstance->Montage_Play(Die_Montage);
			AnimInstance->Montage_JumpToSection(FName("Death_3"),Die_Montage);
		}
		if(Die_Rand == 3)
		{
			AnimInstance->Montage_Play(Die_Montage);
			AnimInstance->Montage_JumpToSection(FName("Death_4"),Die_Montage);
		}
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}
}

/******************************************************** 라이플 ********************************************************/
void AMain::Fire()
{
	if(HasAuthority())
	{
		MultiFire(bFire);
	}
	else
	{
		ServerFire(bFire);
	}
}

void AMain::Fire_Start()
{
	if(HasAuthority())
	{
		MultiFire_Start();
	}
	else
	{
		ServerFire_Start();
	}
}

void AMain::Fire_End()
{
	if(HasAuthority())
	{
		MultiFire_End();
	}
	else
	{
		ServerFire_End();
	}
}

void AMain::Aiming()
{
	if(HasAuthority())
	{
		MultiAiming(true);
	}
	else
	{
		ServerAiming(true);
	}
}

void AMain::Aiming_End()
{
	if(HasAuthority())
	{
		MultiAiming_End(false);
	}
	else
	{
		ServerAiming_End(false);
	}
}

void AMain::Reload()
{
	if(!bDie)
	{
		switch (Weapon_State)
		{
		case EWeaponState::Rifle:
			if(Weapon_Base->GetAmmo() != Weapon_Base->GetMaxAmmo() && Weapon_Base->GetHaveAmmo() != 0)
			{
				if(!bFire && !bAiming && !bReloading)
				{
					AnimInstance->Montage_Play(Rifle_Relaoding_Montage);
					Weapon_Base->Reload();
					bReloading = true;
				}
			}
		}
	}
}

void AMain::Reload_End()
{
	bReloading = false;
	AnimInstance->Montage_Stop(0.2f,Rifle_Relaoding_Montage);
	Weapon_Base->Reload_End();
}

void AMain::UnEquip()
{
	if(HasAuthority())
	{
		MultiUnEquipRifle();
	}
	else
	{
		ServerUnEquipRifle();
	}
}

void AMain::Equip_Rifle()
{
	if(HasAuthority())
	{
		MultiEquipRifle();
	}
	else
	{
		ServerEquipRifle();
	}
}
/******************************************************** 수류탄 ********************************************************/
void AMain::Throw_Ready()
{
	if(AnimInstance && !bIsGrenade)
	{
		if(!AnimInstance->IsAnyMontagePlaying() && Get_Weapon_State() == EWeaponState::Rifle)
		{
			AnimInstance->Montage_Play(Throw_Loop);
			bIsGrenade = true;
		}
	}
}

void AMain::Throw()
{
	if(AnimInstance)
	{
		if(AnimInstance->Montage_IsPlaying(Throw_Loop))
		{
			bIsGrenade = false;
			float Distance = GetControlRotation().Pitch;
			if(Distance < 180.f)
			{
				AnimInstance->Montage_Play(Throw_Far);
			}
			else
			{
				AnimInstance->Montage_Play(Throw_Close);
			}
		}
	}
}

void AMain::Spawn_Grenade()
{
	FVector Location = GetMesh()->GetSocketLocation(FName("Grenade_Socket"));
	GetWorld()->SpawnActor<AGrenade>(Grenade,Location,GetControlRotation());
	
}



