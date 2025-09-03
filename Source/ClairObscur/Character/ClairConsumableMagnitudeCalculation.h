#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "ClairConsumableMagnitudeCalculation.generated.h"

// Gameplay effect magnitude calculation for clair consumable
UCLASS()
class CLAIROBSCUR_API UClairConsumableMagnitudeCalculation : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:
	UClairConsumableMagnitudeCalculation();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
	
protected:
	// Capture MaxHealth source attribute
	FGameplayEffectAttributeCaptureDefinition SourceMaxHealthDef;
};
