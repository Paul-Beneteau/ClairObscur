// Fill out your copyright notice in the Description page of Project Settings.


#include "ClairCharacter.h"

#include "ClairAbilitySystemComponent.h"
#include "ClairAttributeComp.h"
#include "ClairAttributeSet.h"
#include "ClairObscur/Core/ClairGameStatics.h"
#include "ClairObscur/Core/TurnManagerSubsystem.h"

AClairCharacter::AClairCharacter()
{
	// Increase update frequency for GAS components
	SetNetUpdateFrequency(100.0f);
	ClairAbilitySystemComp = CreateDefaultSubobject<UClairAbilitySystemComponent>(TEXT("ClairAbilitySystemComp"));
	ClairAttributeSet = CreateDefaultSubobject<UClairAttributeSet>(TEXT("ClairAttributeSet"));
	ClairAttributeComp = CreateDefaultSubobject<UClairAttributeComp>(TEXT("AttributeComp"));
}

UAbilitySystemComponent* AClairCharacter::GetAbilitySystemComponent() const
{
	return ClairAbilitySystemComp;
}

float AClairCharacter::GetSpeed_Implementation() const
{
	return ClairAttributeComp->GetSpeed();
}

// Send start turn event used by child class and UI
void AClairCharacter::TakeTurn_Implementation()
{	
	OnTurnStarted.Broadcast();
}

// Initializes clair ability system component with his attribute set and grant initial ability set.
void AClairCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	ClairAbilitySystemComp->Initialize(this, this);
	ClairAttributeComp->Initialize(ClairAbilitySystemComp);
	ClairAbilitySystemComp->AbilityEndedCallbacks.AddUObject(this, &ThisClass::OnAbilityEndedHandler);
}

// Callback used by child class when an ability end
void AClairCharacter::OnAbilityEndedHandler(UGameplayAbility* GameplayAbility)
{ }

void AClairCharacter::EndTurn()
{
	// Check if tags must be removed
	if (DefencelessTag == FGameplayTag::EmptyTag)
	{
		UE_LOG(ClairLog, Warning, TEXT("AClairCharacter: Defenceless tag has not been set"));
	}
	else
	{		
		for (FGameplayTag Tag : PreviousTurnGameplayTags)
		{
			// If Defenceless lasted for 1 turn, remove it
			if (Tag == DefencelessTag)
			{
				GameplayTags.RemoveTag(DefencelessTag);
				ClairAttributeComp->OnDefencelessStatusChanged.Broadcast();
			}	
		}
	}
	
	PreviousTurnGameplayTags.Reset();
	PreviousTurnGameplayTags.AppendTags(GameplayTags);

	// End turn from TurnManagerSubsystem
	if (UTurnManagerSubsystem* TurnManagerSubsystem = GetGameInstance()->GetSubsystem<UTurnManagerSubsystem>())
	{
		TurnManagerSubsystem->EndTurn();
	}
	
	OnTurnEnded.Broadcast();
}
