#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ClairAbilitySystemComponent.generated.h"

class UClairAttributeSet;

// Key representing an ability. It is associated with an ability in a TMap
UENUM(BlueprintType)
enum class EClairAbilityKey : uint8
{
	None = 0 UMETA(Hidden),
	PrimaryAttack = 1,
	Dodge = 2,
	PrimaryDefensiveAbility = 3,
	PrimaryVirtuoseAbility = 4,
	PrimaryOffensiveAbility = 5,
};

USTRUCT()
struct FClairAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> GameplayAbility;

	UPROPERTY(EditDefaultsOnly)
	EClairAbilityKey Key { EClairAbilityKey::None };
	
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
	void ActivateAbility(const EClairAbilityKey Key);
	
	void ActivateAbilityOnTarget(const EClairAbilityKey Key, AActor* Target);

	bool CanActivateAbility(const EClairAbilityKey Key);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<FClairAbility> InitialAbilities;
	
	// Ability handles used to activate abilities
	TMap<EClairAbilityKey, FGameplayAbilitySpecHandle> AbilityHandles;
};
