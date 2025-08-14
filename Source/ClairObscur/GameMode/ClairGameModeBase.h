// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ClairGameModeBase.generated.h"

UCLASS()
class CLAIROBSCUR_API AClairGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:	
	virtual void StartPlay() override;
};
