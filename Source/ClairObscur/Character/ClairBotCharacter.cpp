// Fill out your copyright notice in the Description page of Project Settings.


#include "ClairBotCharacter.h"

#include "ClairAbilitySystemComponent.h"
#include "ClairObscur/ClairGameStatics.h"
#include "ClairObscur/GameMode/TurnManagerSubsystem.h"

void AClairBotCharacter::TakeTurn_Implementation()
{
	// TODO: Target selection for multiple players
	Super::TakeTurn_Implementation();
	
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		ClairAbilitySystemComp->ActivateAbilityOnTarget(PrimaryAttack, PlayerController->GetCharacter());
	}

	// TODO remove comments used to debug
	/*if (UTurnManagerSubsystem* TurnManagerSubsystem = GetGameInstance()->GetSubsystem<UTurnManagerSubsystem>())
	{
		TurnManagerSubsystem->EndTurn();
	}*/
}
