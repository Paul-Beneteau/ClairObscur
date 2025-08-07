// Fill out your copyright notice in the Description page of Project Settings.


#include "ClairGameModeBase.h"

#include "TurnManagerSubsystem.h"

void AClairGameModeBase::StartPlay()
{
	Super::StartPlay();

	UTurnManagerSubsystem* TurnManagerSubsystem = GetGameInstance()->GetSubsystem<UTurnManagerSubsystem>();
	TurnManagerSubsystem->Start();
}
