// Fill out your copyright notice in the Description page of Project Settings.


#include "ClairConsumableComponent.h"

#include "ClairPlayerCharacter.h"
#include "ClairObscur/ClairGameStatics.h"

int32 UClairConsumableComponent::GetConsumableCount(EClairConsumableKey Key) const
{
	const FClairConsumableItem* Consumable = Consumables.Find(Key);
	
	if (Consumable == nullptr)
	{
		UE_LOG(ClairLog, Warning, TEXT("UClairConsumableComponent: Consumable is not found"));
		return 0;
	}
	
	return Consumable->Count;
}

void UClairConsumableComponent::ActivateConsumable(EClairConsumableKey Key)
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