#include "ClairAttributeComp.h"

#include "ClairObscur/ClairGameStatics.h"
#include "ClairObscur/GameMode/TurnManagerSubsystem.h"
#include "GameFramework/Character.h"

void UClairAttributeComp::Initialize(UClairAbilitySystemComponent* InAbilitySystemComp)
{
	AActor* Owner = GetOwner();
	check(Owner);

	if (ClairAbilitySystemComp)
	{
		UE_LOG(ClairLog, Error, TEXT("ClairAttributeComp: cannot initialize because ClairAbilitySystemComp already exist"));
		return;
	}

	ClairAbilitySystemComp = InAbilitySystemComp;
	if (!ClairAbilitySystemComp)
	{
		UE_LOG(ClairLog, Error, TEXT("ClairAttributeComp: ClairAbilitySystemComp given for initialization is null"));
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
}

void UClairAttributeComp::HandleHealthChanged(AActor* Instigator, float OldValue, float NewValue)
{
	OnHealthChanged.Broadcast(this, Instigator, OldValue, NewValue);

	// If the character died
	if (NewValue == 0.0f && NewValue != OldValue)
	{			
		ACharacter* Character = CastChecked<ACharacter>(GetOwner());

		Character->SetLifeSpan(3.5f);
		
		if (DeathAnim)
		{			
			Character->PlayAnimMontage(DeathAnim);	
		}
		else
		{
			UE_LOG(ClairLog, Warning, TEXT("ClairAttributeComp: Death anim montage has not been set"));
		}
		
		if (APlayerController* PlayerController { Cast<APlayerController>(Character->GetInstigatorController()) })
		{
			Character->DisableInput(PlayerController);
		}

		UTurnManagerSubsystem* TurnManagerSubsystem = Character->GetGameInstance()->GetSubsystem<UTurnManagerSubsystem>();
		TurnManagerSubsystem->ResetTurnOrder();
	}
}

void UClairAttributeComp::HandleActionPointsChanged(AActor* Instigator, float OldValue, float NewValue)
{
	OnActionPointsChanged.Broadcast(this, Instigator, OldValue, NewValue);
}


