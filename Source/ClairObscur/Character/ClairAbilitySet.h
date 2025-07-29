// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ClairAbilitySet.generated.h"

class UClairAbilitySystemComponent;
class UGameplayAbility;

// Links a gameplay ability with an input ID used to create or activate the ability
UENUM(BlueprintType)
enum class EAbilityInputKey : uint8
{
	None = 0 UMETA(Hidden),
	PrimaryAttack = 1,
};

USTRUCT()
struct FClairAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<UGameplayAbility> GameplayAbility;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	EAbilityInputKey Key;
};

// Store abilities for a player or a bot.
UCLASS()
class CLAIROBSCUR_API UClairAbilitySet : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<FClairAbility> ClairAbilities;
	
	void GrantAbilities(UClairAbilitySystemComponent* ClairAbilitySystemComp) const;
};
