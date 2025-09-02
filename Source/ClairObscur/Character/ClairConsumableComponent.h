// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ClairConsumableComponent.generated.h"

class UGameplayEffect;

// Key used by a TMap to save consumables
UENUM(BlueprintType)
enum class EClairConsumableKey : uint8
{
	None = 0 UMETA(Hidden),
	FirstConsumable = 1,
	SecondConsumable = 2,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnConsumableChanged, EClairConsumableKey, Key, int32, Count);

USTRUCT()
struct FClairConsumableItem
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Consumable")
	TSubclassOf<UGameplayEffect> Effect;

	// Consumable number of uses
	UPROPERTY(EditDefaultsOnly, Category = "Consumable")
	int32 Count { 0 };
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLAIROBSCUR_API UClairConsumableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnConsumableChanged OnConsumableChanged;

	UFUNCTION(BlueprintCallable)
	int32 GetConsumableCount(EClairConsumableKey Key) const;

	UFUNCTION(BlueprintCallable)
	void ActivateConsumable(EClairConsumableKey Key);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Consumable")
	TMap<EClairConsumableKey, FClairConsumableItem> Consumables;
};
