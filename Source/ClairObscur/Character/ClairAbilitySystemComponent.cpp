#include "ClairAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "ClairAttributeSet.h"
#include "ClairObscur/Core/ClairGameStatics.h"

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
		
		FGameplayAbilitySpec AbilitySpec { ClairAbility.GameplayAbility,0, static_cast<int32>(ClairAbility.Key)};
		AbilitySpec.GameplayEventData = MakeShared<FGameplayEventData>();
		AbilitySpec.GameplayEventData->EventTag = ClairAbility.GameplayEventTag;

		AbilityHandles.Add(ClairAbility.Key, GiveAbility(AbilitySpec));
	}
}

void UClairAbilitySystemComponent::ActivateAbility(const EClairAbilityKey Key)
{
	FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(*AbilityHandles.Find(Key));
	
	if (AbilitySpec == nullptr)
	{
		UE_LOG(ClairLog, Warning, TEXT("UClairAbilitySystemComponent: ability to activate is not found"));
		return;
	}

	// Activate ability by sending an event via a gameplay event tag
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwnerActor(), AbilitySpec->GameplayEventData->EventTag , *AbilitySpec->GameplayEventData);
}

void UClairAbilitySystemComponent::ActivateAbilityOnTarget(const EClairAbilityKey Key, AActor* Target)
{	
	FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(*AbilityHandles.Find(Key));
	check(AbilitySpec && AbilitySpec->GameplayEventData);

	// Saves the target that can be read by the blueprint gameplay ability
	AbilitySpec->GameplayEventData->Target = Target;
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwnerActor(), AbilitySpec->GameplayEventData->EventTag , *AbilitySpec->GameplayEventData);
}

bool UClairAbilitySystemComponent::CanActivateAbility(const EClairAbilityKey Key)
{
	FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(*AbilityHandles.Find(Key));
	check(AbilitySpec && AbilitySpec->Ability);
	
	return AbilitySpec->Ability->CanActivateAbility(*AbilityHandles.Find(Key), AbilityActorInfo.Get());
}
