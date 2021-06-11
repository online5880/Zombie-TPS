// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item_Data_Struct.generated.h"
/**
 * 
 */
class JUNE_API Item_Data_Struct
{
public:
	Item_Data_Struct();
	~Item_Data_Struct();
};

USTRUCT(Atomic,BlueprintType)
struct FItem_Data_Struct
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class AItem_Base* Item_Base;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString Item_Name;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString Item_Description;
 
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UTexture2D* Thumbnail;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bStack;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 Quantity;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bFilled;

	bool operator==(const FItem_Data_Struct& Item_Data_Struct) const
	{
		if(Item_Name == Item_Data_Struct.Item_Name)
			return true;
		return false;
	}
};
