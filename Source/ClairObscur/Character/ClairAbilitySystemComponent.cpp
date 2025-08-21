// Fill out your copyright notice in the Description page of Project Settings.


#include "ClairAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "ClairAttributeSet.h"
#include "ClairObscur/ClairGameStatics.h"

// Grants abilities from ClairAbilitySet and saves their handle
void UClairAbilitySystemComponent::Initialize(AActor* InOwnerActor, AActor* InAvatarActor)
{
	InitAbilityActorInfo(InOwnerActor, InAvatarActor);
	
	// Grants initial abilities to ability system component	
	for (FClairAbility ClairAbility : InitialAbilities)
	{	
		if (ClairAbility.GameplayEventTag == FGameplayTag::EmptyTag)
		{
			UE_LOG(ClairLog, Warning, TEXT("UClairAbilitySystemComponent: an ability trigger tag has not been set in InitialAbilities"));
			return;
		}
		
		FGameplayAbilitySpec AbilitySpec { ClairAbility.GameplayAbility,0, static_cast<int32>(ClairAbility.InputID)};
		AbilitySpec.GameplayEventData = MakeShared<FGameplayEventData>();
		AbilitySpec.GameplayEventData->EventTag = ClairAbility.GameplayEventTag;

		AbilityHandles.Add(ClairAbility.InputID, GiveAbility(AbilitySpec));
	}
}

// Send en event with an event tag associated from an InputID in AbilityHandles. This event activates the associated
// ability with the target as a parameter.
void UClairAbilitySystemComponent::ActivateAbilityOnTarget(const EAbilityInputID InputID, AActor* Target)
{
	FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(*AbilityHandles.Find(InputID));
	check(AbilitySpec && AbilitySpec->GameplayEventData);

	AbilitySpec->GameplayEventData->Target = Target;
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwnerActor(), AbilitySpec->GameplayEventData->EventTag , *AbilitySpec->GameplayEventData);
}
