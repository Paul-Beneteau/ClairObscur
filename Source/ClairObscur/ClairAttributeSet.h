// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "ClairAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_MULTICAST_DELEGATE_ThreeParams(FClairAttributeEvent, AActor* /*EffectInstigator*/, float /*OldValue*/, float /*NewValue*/);

UCLASS()
class CLAIROBSCUR_API UClairAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	ATTRIBUTE_ACCESSORS(UClairAttributeSet, Health)
	ATTRIBUTE_ACCESSORS(UClairAttributeSet, MaxHealth)	
	ATTRIBUTE_ACCESSORS(UClairAttributeSet, Damage)
	ATTRIBUTE_ACCESSORS(UClairAttributeSet, Heal)
	ATTRIBUTE_ACCESSORS(UClairAttributeSet, ActionPoints)
	ATTRIBUTE_ACCESSORS(UClairAttributeSet, MaxActionPoints)

	mutable FClairAttributeEvent OnHealthChanged;
	mutable FClairAttributeEvent OnActionPointsChanged;
	
	UClairAttributeSet();
	
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData &Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", meta = (HideFromModifiers))
	FGameplayAttributeData Health;
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxHealth;
	float HealthBeforeChange { 0.0f };

	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Damage;	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Heal;
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData ActionPoints;
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxActionPoints;
	float ActionPointsBeforeChange { 0.0f };
	
};