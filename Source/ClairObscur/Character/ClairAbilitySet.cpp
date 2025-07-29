// Fill out your copyright notice in the Description page of Project Settings.


#include "ClairAbilitySet.h"

#include "ClairAbilitySystemComponent.h"

void UClairAbilitySet::GrantAbilities(UClairAbilitySystemComponent* ClairAbilitySystemComp) const
{
	check(ClairAbilitySystemComp);
	
	for (const FClairAbility ClairAbility : ClairAbilities)
	{
		FGameplayAbilitySpec AbilitySpec { ClairAbility.GameplayAbility,0, static_cast<int32>(ClairAbility.Key) };
		ClairAbilitySystemComp->GiveAbility(AbilitySpec);		
	}
}
