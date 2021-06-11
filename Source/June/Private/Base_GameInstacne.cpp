// Fill out your copyright notice in the Description page of Project Settings.


#include "Base_GameInstacne.h"


void UBase_GameInstacne::Add_Inventory(FItem_Data_Struct Item)
{
	GEngine->AddOnScreenDebugMessage(-1,1.5f,FColor::Red,FString::Printf(TEXT("GameInstance Add Item"),false));
	for(int i = 0 ; i< Inventory_Item.Num();i++)
	{
		FItem_Data_Struct Item_Temp = Inventory_Item[i];
		{
			if(Item == Item_Temp)
			{
				GEngine->AddOnScreenDebugMessage(-1,1.5f,FColor::Red,FString::Printf(TEXT("GameInstance Item 중복"),false));
				Item.bStack = true;
				Item.Quantity++;
				Item.bFilled = false;
				Inventory_Item[i].Quantity++;
				break;
			}
			else
			{
				Item.bStack = false;
			}
		}
	}
	//FItem_Data_Struct Test = Inventory_Item.Find(Item);
	Inventory_Item.Add(Item);
	OnInventoryUpdated.Broadcast();
}

void UBase_GameInstacne::Remove_Inventory_Item(FItem_Data_Struct Item)
{
	for(int i = 0 ; i< Inventory_Item.Num();i++)
	{
		FItem_Data_Struct& Item_Temp = Inventory_Item[i];
		{
			if(Item == Item_Temp)
			{
				if(Inventory_Item[i].Quantity > 0)
				{
					Inventory_Item[i].Quantity--;
				}
				else if(Inventory_Item[i].Quantity == 0)
				{
					GEngine->AddOnScreenDebugMessage(-1,1.5f,FColor::Red,FString::Printf(TEXT("GameInstance Item 삭제"),false));
					Inventory_Item.Remove(Item);
				}
				break;
			}
		}
	}
}

