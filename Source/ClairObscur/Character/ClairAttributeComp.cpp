#include "ClairAttributeComp.h"

#include "ClairPlayerCharacter.h"
#include "ClairObscur/ClairGameStatics.h"
#include "ClairObscur/GameMode/TurnManagerSubsystem.h"
#include "GameFramework/Character.h"

void UClairAttributeComp::Initialize(UClairAbilitySystemComponent* InAbilitySystemComp)
{	
	ClairAbilitySystemComp = InAbilitySystemComp;
	if (!ClairAbilitySystemComp)
	{
		UE_LOG(ClairLog, Error, TEXT("ClairAttributeComp: Ability system component is null"));
		return;
	}
	
	ClairAttributeSet = ClairAbilitySystemComp->GetSet<UClairAttributeSet>();
	if (!ClairAttributeSet)
	{
		UE_LOG(ClairLog, Error, TEXT("ClairAttributeComp: ClairAttributeSet is null"));
		return;
	}

	// Register callbacks to listen for attribute changes.
	ClairAttributeSet->OnHealthChanged.AddUObject(this, &ThisClass::HandleHealthChanged);
	ClairAttributeSet->OnActionPointsChanged.AddUObject(this, &ThisClass::HandleActionPointsChanged);

	if (!InitialGameplayEffect)
	{
		UE_LOG(ClairLog, Error, TEXT("ClairAttributeComp: initial gameplay effect has not been set"));
		return;
	}
	
	// Initialize attribute values with an initial gameplay effect
	ClairAbilitySystemComp->ApplyGameplayEffectToSelf(InitialGameplayEffect->GetDefaultObject<UGameplayEffect>(),
		0.0f, ClairAbilitySystemComp->MakeEffectContext());

	// Check if burn damage must be applied at the start of at the character turn
	if (AClairCharacter* Character { Cast<AClairCharacter>(GetOwner()) })
	{
		Character->OnTurnStarted.AddUniqueDynamic(this, &UClairAttributeComp::HandleBurnStatus);
	}
}

void UClairAttributeComp::HandleHealthChanged(AActor* Instigator, float OldValue, float NewValue)
{
	OnHealthChanged.Broadcast(this, Instigator, OldValue, NewValue);

	// If the character died
	if (NewValue == 0.0f && NewValue != OldValue)
	{
		ACharacter* Character = CastChecked<ACharacter>(GetOwner());
		
		Character->SetLifeSpan(OnDeathDestroyActorDelay);
		
		if (DeathAnim)
		{			
			Character->PlayAnimMontage(DeathAnim);	
		}
		else
		{
			UE_LOG(ClairLog, Warning, TEXT("ClairAttributeComp: Death anim montage has not been set"));
		}

		// Remove character from TurnManagerSubsystem and pause it to wait for death animation or end of combat
		if (UTurnManagerSubsystem* TurnManagerSubsystem = Character->GetGameInstance()->GetSubsystem<UTurnManagerSubsystem>())
		{
			if (APlayerController* PlayerController { Cast<APlayerController>(Character->GetInstigatorController()) })
			{
				Character->DisableInput(PlayerController);				
				TurnManagerSubsystem->Pause();			
			}
			else
			{
				TurnManagerSubsystem->Pause(OnDeathDestroyActorDelay);	
			}
			
			TurnManagerSubsystem->RemoveCharacter(GetOwner());
		}
		
		OnDeath.Broadcast();
	}
}

void UClairAttributeComp::HandleActionPointsChanged(AActor* Instigator, float OldValue, float NewValue)
{
	OnActionPointsChanged.Broadcast(this, Instigator, OldValue, NewValue);
}

void UClairAttributeComp::HandleBurnStatus()
{	
	if (AClairCharacter* Character { Cast<AClairCharacter>(GetOwner()) })
	{
		float BurningDamage = { GetBurningDamage() };
		
		// Apply burning effect if character has burning stacks
		if (BurningDamage > 0 && BurningStacks.IsValidIndex(0))
		{
			// TODO: Find the right way to apply effect as we are taking one of the gameplay effect then modify it with
			// the new magnitude and set it back to original after.
			UGameplayEffect* BurningEffect = BurningStacks[0].Effect->GetDefaultObject<UGameplayEffect>();			
			
			// Save BaseMagnitude to set it back after being modified and applied
			FGameplayEffectModifierMagnitude BaseMagnitude = BurningEffect->Modifiers[0].ModifierMagnitude;
			
			BurningEffect->Modifiers[0].ModifierMagnitude = FGameplayEffectModifierMagnitude(BurningDamage);
				
			ClairAbilitySystemComp->ApplyGameplayEffectToSelf(BurningEffect,0.0f, ClairAbilitySystemComp->MakeEffectContext());
			
			BurningEffect->Modifiers[0].ModifierMagnitude = BaseMagnitude;

			// Remove 1 turn for the turn duration of burning stacks
			for (FBurningStack& BurningStack : BurningStacks)
			{
				BurningStack.TurnDuration--;
			}

			int32 BurningStacksCount = { BurningStacks.Num() };
			
			// Remove burning stack with null or negative turn duration
			BurningStacks.RemoveAll([&](const FBurningStack& BurningStack)
			{
				return BurningStack.TurnDuration <= 0;
			});

			// If one or more burning stack has been removed 
			if (BurningStacksCount > BurningStacks.Num())
			{
				OnBurnStatusChanged.Broadcast(BurningStacks.Num());
			}
		}
	}	
}

void UClairAttributeComp::AddBurningStacks(const TArray<FBurningStack> InBurningStacks)
{
	BurningStacks.Append(InBurningStacks);

	// Send event that the character is burning 
	if (BurningStacks.Num() > 0)
	{
		OnBurnStatusChanged.Broadcast(BurningStacks.Num());
	}
}

// Compute Burning damage by adding each burning stack damage to the total burning damage
float UClairAttributeComp::GetBurningDamage()
{
	float BurningDamage = { 0 };

	// Adds each stack damage to the total burning damage
	for (FBurningStack BurningStack : BurningStacks)
	{
		if (UGameplayEffect* BurningEffect = BurningStack.Effect->GetDefaultObject<UGameplayEffect>())
		{
			FGameplayEffectSpec GameplayEffectSpec (BurningEffect, ClairAbilitySystemComp->MakeEffectContext(),
				FGameplayEffectConstants::INSTANT_APPLICATION);

			float BurningStackDamage = { 0 };
					
			BurningEffect->Modifiers[0].ModifierMagnitude.AttemptCalculateMagnitude(GameplayEffectSpec, BurningStackDamage);

			BurningDamage += BurningStackDamage;
		}
		else
		{
			UE_LOG(ClairLog, Warning, TEXT("ClairAttributeComp: This stack doesn't have a burning gameplay effect"));
		}
	}

	return BurningDamage;
}