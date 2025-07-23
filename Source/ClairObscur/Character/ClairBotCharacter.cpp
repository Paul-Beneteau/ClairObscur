// Fill out your copyright notice in the Description page of Project Settings.


#include "ClairBotCharacter.h"
#include "ClairAbilitySystemComponent.h"
#include "ClairAttributeComp.h"

AClairBotCharacter::AClairBotCharacter()
{
	ClairAbilitySystemComp = CreateDefaultSubobject<UClairAbilitySystemComponent>(TEXT("ClairBotAbilitySystemComp"));
	ClairAttributeSet = CreateDefaultSubobject<UClairAttributeSet>(TEXT("ClairBotAttributeSet"));
	AttributeComp = CreateDefaultSubobject<UClairAttributeComp>(TEXT("BotAttributeComp"));
}

void AClairBotCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(ClairAbilitySystemComp);
	ClairAbilitySystemComp->InitAbilityActorInfo(this, this);
	AttributeComp->InitializeWithAbilitySystem(ClairAbilitySystemComp);
}

UAbilitySystemComponent* AClairBotCharacter::GetAbilitySystemComponent() const
{
	return ClairAbilitySystemComp;
}

