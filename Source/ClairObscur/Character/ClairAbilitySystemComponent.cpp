// Fill out your copyright notice in the Description page of Project Settings.


#include "ClairAbilitySystemComponent.h"

#include "ClairAttributeSet.h"

void UClairAbilitySystemComponent::Initialize(AActor* InOwnerActor, AActor* InAvatarActor)
{
	InitAbilityActorInfo(InOwnerActor, InAvatarActor);
	
	// Grants initial abilities to ability system component
	for (const FClairAbility ClairAbility : ClairAbilitySet)
	{
		FGameplayAbilitySpec AbilitySpec { ClairAbility.GameplayAbility,0, static_cast<int32>(ClairAbility.InputID) };
		GiveAbility(AbilitySpec);		
	}
}
