#pragma once

#include "CoreMinimal.h"
#include "ClairAttributeSet.h"
#include "Components/ActorComponent.h"
#include "ClairAbilitySystemComponent.h"

#include "ClairAttributeComp.generated.h"

class UClairAbilitySystemComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged, UClairAttributeComp*, ClairAttributeComp, AActor*, Instigator, float, OldValue, float, NewValue);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLAIROBSCUR_API UClairAttributeComp : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnHealthChanged;
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnActionPointsChanged;

	// Initialize the component using an ability system component.
	void Initialize(UClairAbilitySystemComponent* ClairAbilitySystemComp);
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	int32 GetMaxHealth() const { return (ClairAttributeSet ? ClairAttributeSet->GetMaxHealth() : 0.0f); }
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	int32 GetHealth() const { return (ClairAttributeSet ? ClairAttributeSet->GetHealth() : 0.0f); };
	// Callback when health is changed
	virtual void HandleHealthChanged(AActor* Instigator, float OldValue, float NewValue);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	int32 GetMaxActionPoints() const { return (ClairAttributeSet ? ClairAttributeSet->GetMaxActionPoints() : 0.0f); }
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	int32 GetActionPoints() const { return (ClairAttributeSet ? ClairAttributeSet->GetActionPoints() : 0.0f); }
	// Callback when action points count is changed
	virtual void HandleActionPointsChanged(AActor* Instigator, float OldValue, float NewValue);
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	int32 GetSpeed() const { return (ClairAttributeSet ? ClairAttributeSet->GetSpeed() : 0.0f); }
	
protected:
	UPROPERTY()
	TObjectPtr<UClairAbilitySystemComponent> ClairAbilitySystemComp;	
	UPROPERTY()
	TObjectPtr<const UClairAttributeSet> ClairAttributeSet;
	
	// Initialize attribute value with a gameplay effect
	UPROPERTY(EditAnywhere, Category = "Initialization")
	TSubclassOf<UGameplayEffect> InitialGameplayEffect;
};
