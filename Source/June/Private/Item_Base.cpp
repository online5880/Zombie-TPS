// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_Base.h"

#include "Base_GameInstacne.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AItem_Base::AItem_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Body_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body_Mesh"));
	RootComponent = Body_Mesh;

	Outline_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Outline Mesh"));
	Outline_Mesh->SetupAttachment(Body_Mesh);
	Outline_Mesh->SetVisibility(false);
	Outline_Mesh->SetStaticMesh(Body_Mesh->GetStaticMesh());
	Outline_Mesh->SetRelativeScale3D(FVector(1.01f,1.01f,1.01f));

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
	Outline(TEXT("Material'/Game/Blueprint/Outline.Outline'"));
	if(Outline.Succeeded()) { Outline_Material = Outline.Object; }

}

// Called when the game starts or when spawned
void AItem_Base::BeginPlay()
{
	Super::BeginPlay();
	
	AActor* Actor = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
	Main = Cast<AMain>(Actor);
	
	// Test
	// Test2
	
}

// Called every frame
void AItem_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItem_Base::Interact()
{
	Add_Item(Item_Data);
}

FString AItem_Base::Get_Name()
{
	return Item_Name;
}

void AItem_Base::Add_Item(FItem_Data_Struct Item)
{
	UBase_GameInstacne* GameInstacne  = Cast<UBase_GameInstacne>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	AItem_Base* Temp = NewObject<AItem_Base>();
	Temp->Item_Data = this->Item_Data;
	GameInstacne->Add_Inventory(Temp->Item_Data);
	Body_Mesh->DestroyComponent();
}

void AItem_Base::Remove_Item(FItem_Data_Struct Item)
{
	UBase_GameInstacne* GameInstacne  = Cast<UBase_GameInstacne>(UGameplayStatics::GetGameInstance(GetWorld()));
	GameInstacne->Remove_Inventory_Item(Item_Data);
}

void AItem_Base::Use(AMain* Character)
{
	switch (Item_Type)
	{
	case 0:
		GEngine->AddOnScreenDebugMessage(-1,1.5f,FColor::Red,FString::Printf(TEXT("아이템 1"),false));
		Character->Health += 30.f;
		break;
	case 1:
		GEngine->AddOnScreenDebugMessage(-1,1.5f,FColor::Red,FString::Printf(TEXT("아이템 2"),false));
		break;
	case 2:
		GEngine->AddOnScreenDebugMessage(-1,1.5f,FColor::Red,FString::Printf(TEXT("아이템 3"),false));
		break;
	case 3:
		GEngine->AddOnScreenDebugMessage(-1,1.5f,FColor::Red,FString::Printf(TEXT("아이템 4"),false));
		break;
	}
}

void AItem_Base::Outline()
{
	for(int i = 0; i<Body_Mesh->GetNumMaterials()-1;i++)
	{
		Outline_Mesh->SetMaterial(i,Outline_Material);
		GEngine->AddOnScreenDebugMessage(-1,1.5f,FColor::Red,FString::Printf(TEXT("아웃라인"),false));
	}
}

