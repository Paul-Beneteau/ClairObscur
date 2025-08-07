// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TurnCharacterInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTurnCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CLAIROBSCUR_API ITurnCharacterInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent)
	void TakeTurn();
	
	UFUNCTION(BlueprintNativeEvent)
	float GetSpeed() const;
};
