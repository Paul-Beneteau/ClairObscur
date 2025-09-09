// Fill out your copyright notice in the Description page of Project Settings.


#include "ClairBotCharacter.h"

#include "ClairAbilitySystemComponent.h"
#include "ClairAttributeComp.h"
#include "ClairStanceCharacter.h"

// Activate primary attack on the player
void AClairBotCharacter::TakeTurn_Implementation()
{
	Super::TakeTurn_Implementation();
	
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	// Player which is the target must be valid. Burn is applied during the beginning of the turn so we have to check
	// that it didn't kill the character and that he can play his turn
	if (PlayerController && ClairAttributeComp->GetHealth())
	{
		ClairAbilitySystemComp->ActivateAbilityOnTarget(DefaultAttack, PlayerController->GetCharacter());
	}
}

// End turn when a gameplay ability ends
void AClairBotCharacter::OnAbilityEndedHandler(UGameplayAbility* GameplayAbility)
{
	EndTurn();
}
