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

// Represents a consumable, it is composed of a gameplay effect applied to the owner and a count representing the
// number os uses
USTRUCT()
struct FClairConsumableItem
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Consumable")
	TSubclassOf<UGameplayEffect> Effect;

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
	int32 GetConsumableCount(const EClairConsumableKey Key) const;

	UFUNCTION(BlueprintCallable)
	void ActivateConsumable(const EClairConsumableKey Key);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Consumable")
	TMap<EClairConsumableKey, FClairConsumableItem> Consumables;
};
