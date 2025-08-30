// Fill out your copyright notice in the Description page of Project Settings.


#include "ClairBotCharacter.h"

#include "ClairAbilitySystemComponent.h"
#include "ClairStanceCharacter.h"

// Activate primary attack on the player
void AClairBotCharacter::TakeTurn_Implementation()
{
	Super::TakeTurn_Implementation();
	
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		ClairAbilitySystemComp->ActivateAbilityOnTarget(PrimaryAttack, PlayerController->GetCharacter());
	}
}

// End turn when a gameplay ability ends
void AClairBotCharacter::OnAbilityEndedHandler(UGameplayAbility* GameplayAbility)
{
	EndTurn();
}
