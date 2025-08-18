// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TurnCharacterInterface.generated.h"

class UTurnDelegate;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTurnCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

// Interface that must be implemented by a character to be used by the TurnManagerSubsystem
class CLAIROBSCUR_API ITurnCharacterInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	float GetSpeed() const;

	UFUNCTION(BlueprintNativeEvent)
	void TakeTurn();
};
