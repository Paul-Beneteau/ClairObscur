// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ClairPlayerInputs.h"
#include "ClairAbilitySystemComponent.generated.h"

class UClairAttributeSet;

USTRUCT()
struct FClairAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> GameplayAbility;

	UPROPERTY(EditDefaultsOnly)
	EAbilityInputID InputID { EAbilityInputID::None };
	
	// Gameplay event tag that activate the ability by sending an event
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag GameplayEventTag { FGameplayTag::EmptyTag };
};

UCLASS()
class CLAIROBSCUR_API UClairAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void Initialize(AActor* InOwnerActor, AActor* InAvatarActor);

	UFUNCTION()
	void ActivateAbility(const EAbilityInputID InputID);
	
	void ActivateAbilityOnTarget(const EAbilityInputID InputID, AActor* Target);

	bool CanActivateAbility(const EAbilityInputID InputID);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<FClairAbility> InitialAbilities;
	
	// Ability handles used to activate abilities
	TMap<EAbilityInputID, FGameplayAbilitySpecHandle> AbilityHandles;
};
