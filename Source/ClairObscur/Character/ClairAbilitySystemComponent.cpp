// Fill out your copyright notice in the Description page of Project Settings.


#include "ClairAbilitySystemComponent.h"

#include "ClairAttributeSet.h"

void UClairAbilitySystemComponent::Initialize(AActor* InOwnerActor, AActor* InAvatarActor)
{
	InitAbilityActorInfo(InOwnerActor, InAvatarActor);

	AbilitySpecHandles.Reserve(ClairAbilitySet.Num());
	
	// Grants initial abilities to ability system component
	for (const FClairAbility ClairAbility : ClairAbilitySet)
	{
		FGameplayAbilitySpec AbilitySpec { ClairAbility.GameplayAbility,0, static_cast<int32>(ClairAbility.InputID) };
		AbilitySpecHandles.Add(ClairAbility.InputID, GiveAbility(AbilitySpec));
	}
}

void UClairAbilitySystemComponent::StartAbility(const EAbilityInputID InputID)
{
	TryActivateAbility(*AbilitySpecHandles.Find(InputID));
}
