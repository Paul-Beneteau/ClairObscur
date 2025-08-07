// Fill out your copyright notice in the Description page of Project Settings.


#include "ClairBotCharacter.h"
#include "ClairAbilitySystemComponent.h"
#include "ClairAttributeComp.h"
#include "ClairObscur/ClairGameStatics.h"

AClairBotCharacter::AClairBotCharacter()
{
	ClairAbilitySystemComp = CreateDefaultSubobject<UClairAbilitySystemComponent>(TEXT("ClairBotAbilitySystemComp"));
	ClairAttributeSet = CreateDefaultSubobject<UClairAttributeSet>(TEXT("ClairBotAttributeSet"));
	AttributeComp = CreateDefaultSubobject<UClairAttributeComp>(TEXT("BotAttributeComp"));
}

void AClairBotCharacter::BeginPlay()
{
	Super::BeginPlay();

	ClairAbilitySystemComp->Initialize(this, this);
	AttributeComp->Initialize(ClairAbilitySystemComp);
}

UAbilitySystemComponent* AClairBotCharacter::GetAbilitySystemComponent() const
{
	return ClairAbilitySystemComp;
}

void AClairBotCharacter::TakeTurn_Implementation()
{
	UE_LOG(ClairLog, Display, TEXT("Bot: TakeTurn_Implementation - speed: %f"), GetSpeed_Implementation());
}

float AClairBotCharacter::GetSpeed_Implementation() const
{
	return AttributeComp->GetSpeed();
}

