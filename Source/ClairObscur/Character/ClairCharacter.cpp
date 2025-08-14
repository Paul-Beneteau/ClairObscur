// Fill out your copyright notice in the Description page of Project Settings.


#include "ClairCharacter.h"

#include "ClairAbilitySystemComponent.h"
#include "ClairAttributeComp.h"
#include "ClairAttributeSet.h"
#include "ClairObscur/ClairGameStatics.h"
#include "ClairObscur/GameMode/TurnDelegate.h"
#include "Kismet/GameplayStatics.h"

AClairCharacter::AClairCharacter()
{
	SetNetUpdateFrequency(100.0f);
	ClairAbilitySystemComp = CreateDefaultSubobject<UClairAbilitySystemComponent>(TEXT("ClairAbilitySystemComp"));
	ClairAttributeSet = CreateDefaultSubobject<UClairAttributeSet>(TEXT("ClairAttributeSet"));
	AttributeComp = CreateDefaultSubobject<UClairAttributeComp>(TEXT("AttributeComp"));
	OnTurnEnded = CreateDefaultSubobject<UTurnDelegate>(TEXT("OnTurnEnded"));
}

// Initializes clair ability system component with his attribute set and grant initial ability set.
void AClairCharacter::BeginPlay()
{
	Super::BeginPlay();

	ClairAbilitySystemComp->Initialize(this, this);
	AttributeComp->Initialize(ClairAbilitySystemComp);
	ClairAbilitySystemComp->AbilityEndedCallbacks.AddUObject(this, &ThisClass::OnGameplayAbilityEnded);

	TArray<AActor*> TurnCharacters;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UTurnCharacterInterface::StaticClass(), TurnCharacters);
	check(TurnCharacters.IsValidIndex(0));
	UE_LOG(ClairLog, Display, TEXT("Target - speed: %f"), ITurnCharacterInterface::Execute_GetSpeed(TurnCharacters[0]));
	ClairAbilitySystemComp->CurrentTarget = TurnCharacters[0];
}

UAbilitySystemComponent* AClairCharacter::GetAbilitySystemComponent() const
{
	return ClairAbilitySystemComp;
}

float AClairCharacter::GetSpeed_Implementation() const
{
	return AttributeComp->GetSpeed();
}

void AClairCharacter::PlayTurn_Implementation()
{
	UE_LOG(ClairLog, Display, TEXT("TakeTurn_Implementation - speed: %f"), GetSpeed_Implementation());
}

UTurnDelegate* AClairCharacter::GetOnTurnEndedDelegate_Implementation()
{	
	return OnTurnEnded;
}

// Ends turn when a gameplay ability has been finished
void AClairCharacter::OnGameplayAbilityEnded(UGameplayAbility* GameplayAbility)
{
	OnTurnEnded->Delegate.Broadcast();
}
