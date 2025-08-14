// Fill out your copyright notice in the Description page of Project Settings.


#include "ClairBotCharacter.h"
#include "ClairAbilitySystemComponent.h"
#include "ClairAttributeComp.h"
#include "ClairObscur/ClairGameStatics.h"

void AClairBotCharacter::PlayTurn_Implementation()
{
	UE_LOG(ClairLog, Display, TEXT("Bot TakeTurn_Implementation - speed: %f"), GetSpeed_Implementation());
	// TODO: Parametrize the ability used
	ClairAbilitySystemComp->StartAbility(EAbilityInputID::PrimaryAttack);
}
