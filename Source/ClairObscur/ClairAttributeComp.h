#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ClairAttributeComp.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeChanged, float, NewAttributeValue, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLAIROBSCUR_API UClairAttributeComp : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnHealthChanged;
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnActionPointsChanged;
	
	UClairAttributeComp();
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	int32 GetMaxHealth() const { return MaxHealth; }
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	int32 GetHealth() const { return Health; }
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ChangeHealth(const int32 Delta);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	int32 GetActionPoints() const { return ActionPoints; }
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	int32 GetMaxActionPoints() const { return MaxActionPoints; }
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ChangeActionPoints(const int32 Delta);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	int32 Health;
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	int32 MaxHealth;
	
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	int32 ActionPoints;
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	int32 MaxActionPoints;
};
