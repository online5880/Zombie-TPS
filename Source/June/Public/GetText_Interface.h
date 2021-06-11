// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GetText_Interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGetText_Interface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class JUNE_API IGetText_Interface
{
	GENERATED_BODY()
	
public:
	virtual FString Get_Name() = 0;

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
};
