// Fill out your copyright notice in the Description page of Project Settings.


#include "ClairCharacter.h"

#include "ClairAbilitySystemComponent.h"
#include "ClairAttributeComp.h"
#include "ClairAttributeSet.h"
#include "ClairObscur/GameMode/TurnManagerSubsystem.h"

AClairCharacter::AClairCharacter()
{
	SetNetUpdateFrequency(100.0f);
	ClairAbilitySystemComp = CreateDefaultSubobject<UClairAbilitySystemComponent>(TEXT("ClairAbilitySystemComp"));
	ClairAttributeSet = CreateDefaultSubobject<UClairAttributeSet>(TEXT("ClairAttributeSet"));
	AttributeComp = CreateDefaultSubobject<UClairAttributeComp>(TEXT("AttributeComp"));
}

// Initializes clair ability system component with his attribute set and grant initial ability set.
void AClairCharacter::BeginPlay()
{
	Super::BeginPlay();

	ClairAbilitySystemComp->Initialize(this, this);
	AttributeComp->Initialize(ClairAbilitySystemComp);
	ClairAbilitySystemComp->AbilityEndedCallbacks.AddUObject(this, &ThisClass::AbilityEndedHandler);
}

UAbilitySystemComponent* AClairCharacter::GetAbilitySystemComponent() const
{
	return ClairAbilitySystemComp;
}

float AClairCharacter::GetSpeed_Implementation() const
{
	return AttributeComp->GetSpeed();
}

void AClairCharacter::TakeTurn_Implementation()
{
	//UE_LOG(ClairLog, Display, TEXT("TakeTurn_Implementation - speed: %f"), GetSpeed_Implementation());
}

// Ends turn when a gameplay ability has been finished
void AClairCharacter::AbilityEndedHandler(UGameplayAbility* GameplayAbility)
{
	UTurnManagerSubsystem* TurnManagerSubsystem = GetGameInstance()->GetSubsystem<UTurnManagerSubsystem>();
	TurnManagerSubsystem->EndTurn();
}
