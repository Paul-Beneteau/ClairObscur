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
	UPROPERTY()
	FOnAttributeChanged OnHealthChanged;
	
	UClairAttributeComp();
	
	int32 GetMaxHealth() const { return MaxHealth; }	
	int32 GetHealth() const { return Health; }

	// Change health with the delta and checks that the new health is in valid boundaries.
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthDelta(const int32 Delta);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	int32 MaxHealth;
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	int32 Health;	
};
