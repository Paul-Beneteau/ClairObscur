#include "ClairAttributeComp.h"

void UClairAttributeComp::Initialize(UClairAbilitySystemComponent* InAbilitySystemComp)
{
	AActor* Owner = GetOwner();
	check(Owner);

	if (ClairAbilitySystemComp)
	{
		UE_LOG(LogTemp, Error, TEXT("ClairAttributeComp: attribute component for owner [%s] has already been"
			   " initialized with an ability system."), *GetNameSafe(Owner));
		return;
	}

	ClairAbilitySystemComp = InAbilitySystemComp;
	if (!ClairAbilitySystemComp)
	{
		UE_LOG(LogTemp, Error, TEXT("ClairAttributeComp: Cannot initialize attribute component for owner [%s]"
			   " with NULL ability system."), *GetNameSafe(Owner));
		return;
	}
	
	ClairAttributeSet = ClairAbilitySystemComp->GetSet<UClairAttributeSet>();
	if (!ClairAttributeSet)
	{
		UE_LOG(LogTemp, Error, TEXT("ClairAttributeComp: Cannot initialize Attribute component for owner [%s]"
			   " with NULL attribute set on the ability system."), *GetNameSafe(Owner));
		return;
	}

	// Register callbacks to listen for attribute changes.
	ClairAttributeSet->OnHealthChanged.AddUObject(this, &ThisClass::HandleHealthChanged);
	ClairAttributeSet->OnActionPointsChanged.AddUObject(this, &ThisClass::HandleActionPointsChanged);

	if (!InitialGameplayEffect)
	{
		UE_LOG(LogTemp, Error, TEXT("ClairAttributeComp: Cannot initialize Attribute component for owner [%s]"
			   " with NULL initial gameplay effect"), *GetNameSafe(Owner));
		return;
	}
	
	// Initialize attribute values with an initial gameplay effect
	ClairAbilitySystemComp->ApplyGameplayEffectToSelf(InitialGameplayEffect->GetDefaultObject<UGameplayEffect>(),
		0.0f, ClairAbilitySystemComp->MakeEffectContext());
}

void UClairAttributeComp::HandleHealthChanged(AActor* Instigator, float OldValue, float NewValue)
{
	OnHealthChanged.Broadcast(this, Instigator, OldValue, NewValue);
}

void UClairAttributeComp::HandleActionPointsChanged(AActor* Instigator, float OldValue, float NewValue)
{
	OnActionPointsChanged.Broadcast(this, Instigator, OldValue, NewValue);
}


