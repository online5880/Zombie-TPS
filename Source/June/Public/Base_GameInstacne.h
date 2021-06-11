// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Item_Data_Struct.h"
#include "Engine/GameInstance.h"
#include "Base_GameInstacne.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);
/**
 * 
 */
UCLASS()
class JUNE_API UBase_GameInstacne : public UGameInstance
{
	GENERATED_BODY()

	public:

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Inventory")
	TArray<FItem_Data_Struct> Inventory_Item;

	void Add_Inventory(FItem_Data_Struct Item);

	void Remove_Inventory_Item(FItem_Data_Struct Item);

	UPROPERTY(BlueprintAssignable,Category ="Inventory")
	FOnInventoryUpdated OnInventoryUpdated;
	
};
