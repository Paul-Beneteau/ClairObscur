#include "ClairConsumableComponent.h"

#include "ClairPlayerCharacter.h"
#include "ClairObscur/Core/ClairGameStatics.h"

int32 UClairConsumableComponent::GetConsumableCount(const EClairConsumableKey Key) const
{
	const FClairConsumableItem* Consumable = Consumables.Find(Key);
	
	if (Consumable == nullptr)
	{
		UE_LOG(ClairLog, Warning, TEXT("UClairConsumableComponent: Consumable is not found"));
		return 0;
	}
	
	return Consumable->Count;
}

// If the consumable is found, apply his effect and reduce his count by 1.
void UClairConsumableComponent::ActivateConsumable(const EClairConsumableKey Key)
{
	FClairConsumableItem* Consumable = Consumables.Find(Key);
	
	if (Consumable == nullptr)
	{
		UE_LOG(ClairLog, Warning, TEXT("UClairConsumableComponent: Consumable is not found"));
		return;
	}

	if (Consumable->Count <= 0)
	{
		UE_LOG(ClairLog, Display, TEXT("UClairConsumableComponent: There is no more consumable"));
		return;
	}
	
	if (AClairPlayerCharacter* PlayerCharacter = Cast<AClairPlayerCharacter>(GetOwner()))
	{
		if (UAbilitySystemComponent* AbilitySystemComp = PlayerCharacter->GetAbilitySystemComponent())
		{
			AbilitySystemComp->ApplyGameplayEffectToSelf(Consumable->Effect->GetDefaultObject<UGameplayEffect>(),
													0.0f, AbilitySystemComp->MakeEffectContext());
			Consumable->Count--;

			OnConsumableChanged.Broadcast(Key, Consumable->Count);
		}
		else
		{
			UE_LOG(ClairLog, Warning, TEXT("UClairConsumableComponent: Consumable owner ability system component is null"));
		}
	}
	else
	{
		UE_LOG(ClairLog, Warning, TEXT("UClairConsumableComponent: Consumable owner is not a player character"));
	}
}