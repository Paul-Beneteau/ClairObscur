// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TurnDelegate.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTurnDelegate);

// Delegate wrapper used by TurnCharacterInterface because unreal interfaces can't use a delegate getter directly.
UCLASS()
class CLAIROBSCUR_API UTurnDelegate : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FTurnDelegate Delegate;
};
