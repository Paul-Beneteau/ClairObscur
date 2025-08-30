#pragma once

#include "CoreMinimal.h"
#include "ClairAttributeSet.h"
#include "Components/ActorComponent.h"
#include "ClairAbilitySystemComponent.h"

#include "ClairAttributeComp.generated.h"

class UClairAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged, UClairAttributeComp*, ClairAttributeComp, AActor*, Instigator, float, OldValue, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBurnStatusChanged, int32, BurnStacks);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStatusChanged);

// Represent a burning stack that can be applied by abilities. It damages his owner each turn
USTRUCT(BlueprintType)
struct FBurningStack
{
	GENERATED_BODY()
	
	// Gameplay effect that set burning stack damage
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> Effect;

	// Number of turn the stack deals damage
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 TurnDuration { 3 };
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLAIROBSCUR_API UClairAttributeComp : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnHealthChanged;
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnActionPointsChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnStatusChanged OnDeath;

	// Send event when character starts and stop to burn
	UPROPERTY(BlueprintAssignable)
	FOnBurnStatusChanged OnBurnStatusChanged;
	
	// Initialize the component using an ability system component.
	void Initialize(UClairAbilitySystemComponent* ClairAbilitySystemComp);
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	int32 GetMaxHealth() const { return (ClairAttributeSet ? ClairAttributeSet->GetMaxHealth() : 0.0f); }
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	int32 GetHealth() const { return (ClairAttributeSet ? ClairAttributeSet->GetHealth() : 0.0f); };
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	int32 GetMaxActionPoints() const { return (ClairAttributeSet ? ClairAttributeSet->GetMaxActionPoints() : 0.0f); }
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	int32 GetActionPoints() const { return (ClairAttributeSet ? ClairAttributeSet->GetActionPoints() : 0.0f); }
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	int32 GetSpeed() const { return (ClairAttributeSet ? ClairAttributeSet->GetSpeed() : 0.0f); }
	
	UFUNCTION(BlueprintCallable, Category = "Status")
	int32 GetBurningStacksCount() const { return BurningStacks.Num(); };
	
	UFUNCTION(BlueprintCallable, Category = "Status")	
	void AddBurningStacks(const TArray<FBurningStack> InBurningStacks);
	
	UFUNCTION(BlueprintCallable, Category = "Status")	
	float GetBurningDamage();
	
protected:
	UPROPERTY()
	TObjectPtr<UClairAbilitySystemComponent> ClairAbilitySystemComp;	
	UPROPERTY()
	TObjectPtr<const UClairAttributeSet> ClairAttributeSet;
	
	// Initialize attribute value with a gameplay effect
	UPROPERTY(EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> InitialGameplayEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> DeathAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	float OnDeathDestroyActorDelay { 2.0f };
	
	// Number of turns the burning effect is applied
	TArray<FBurningStack> BurningStacks;

	// Callback when health is changed
	void HandleHealthChanged(AActor* Instigator, float OldValue, float NewValue);
	// Callback when action points count is changed
	void HandleActionPointsChanged(AActor* Instigator, float OldValue, float NewValue);

	UFUNCTION()
	void HandleBurnStatus();
};
