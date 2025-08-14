// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ClairAbilitySystemComponent.generated.h"

class UClairAttributeSet;

// Links a gameplay ability with an input ID used to create or activate the ability

UENUM(BlueprintType)
enum class EAbilityInputID : uint8
{
	None = 0 UMETA(Hidden),
	PrimaryAttack = 1,
}; 

USTRUCT()
struct FClairAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> GameplayAbility;

	UPROPERTY(EditDefaultsOnly)
	EAbilityInputID InputID { 0 };
};

UCLASS()
class CLAIROBSCUR_API UClairAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void Initialize(AActor* InOwnerActor, AActor* InAvatarActor);

	void StartAbility(const EAbilityInputID InputID);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* CurrentTarget { nullptr };
	
protected:

	// Contains gameplay abilities with their associated input key
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TArray<FClairAbility> ClairAbilitySet;

	TMap<EAbilityInputID, FGameplayAbilitySpecHandle> AbilitySpecHandles;
};
