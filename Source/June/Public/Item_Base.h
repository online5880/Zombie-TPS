// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GetText_Interface.h"
#include "Item_Data_Struct.h"
#include "Interact_Interface.h"
#include "Main.h"
#include "GameFramework/Actor.h"
#include "Item_Base.generated.h"
UENUM()
enum class EItem_Type : uint8
{
	Health UMETA(DisplayName = "Health"),
	Test_1 UMETA(DisplayName = "Test_1"),
	Test_2 UMETA(DisplayName = "Test_2"),
	Test_3 UMETA(DisplayName = "Test_3"),
};
UCLASS()
class JUNE_API AItem_Base : public AActor,public IInteract_Interface, public IGetText_Interface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem_Base();

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category ="Item")
	class UStaticMeshComponent* Body_Mesh;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category ="Item")
	class UStaticMeshComponent* Outline_Mesh;
	/************************ 아이템 정보 ***********************/
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Item")
	FString Item_Name;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category ="Item")
	FItem_Data_Struct Item_Data;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Item")
	EItem_Type Item_Type;
	/************************ 머터리얼 ***********************/
	class UMaterialInterface* Outline_Material;
	/************************ 서버 ***********************/

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact() override;

	virtual FString Get_Name() override;

	void Add_Item(FItem_Data_Struct Item);

	UFUNCTION(BlueprintCallable)
	void Remove_Item(FItem_Data_Struct Item);

	UFUNCTION(BlueprintCallable, Category = "Item")
	void Use(AMain* Character);

	void Outline();

	UPROPERTY(BlueprintReadOnly,Category="Item")
	class AMain* Main;
};
