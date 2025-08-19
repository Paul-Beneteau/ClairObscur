// Fill out your copyright notice in the Description page of Project Settings.


#include "ClairBotCharacter.h"

#include "ClairAbilitySystemComponent.h"
#include "ClairObscur/ClairGameStatics.h"
#include "ClairObscur/GameMode/TurnManagerSubsystem.h"

void AClairBotCharacter::TakeTurn_Implementation()
{
	UE_LOG(ClairLog, Display, TEXT("Bot: TakeTurn_Implementation - speed: %f"), GetSpeed_Implementation());

	// TODO: Target selection for multiple players
	AActor* Player = GetWorld()->GetFirstPlayerController()->GetCharacter();
	
	ClairAbilitySystemComp->ActivateAbilityOnTarget(PrimaryAttack, Player);
	
	//UTurnManagerSubsystem* TurnManagerSubsystem = GetGameInstance()->GetSubsystem<UTurnManagerSubsystem>();
	//TurnManagerSubsystem->EndTurn();
}
