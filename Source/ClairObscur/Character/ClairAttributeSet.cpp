// Fill out your copyright notice in the Description page of Project Settings.


#include "ClairAttributeSet.h"
#include "GameplayEffectExtension.h"


// TODO: add data assets to initialize attributes.
UClairAttributeSet::UClairAttributeSet()
	: Health(100.0f)
	, MaxHealth(100.0f)
	, ActionPoints(2.0f)
	, MaxActionPoints(9.0f)
{
}

void UClairAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		// Health value must be between 0 and MaxHealth
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		// MaxHealth value can't be below 1.
		NewValue = FMath::Max(NewValue, 1.0f);
	}
	if (Attribute == GetActionPointsAttribute())
	{
		// ActionPoints value must be between 0 and MaxActionPoints
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, GetMaxActionPoints());
	}
	else if (Attribute == GetMaxActionPointsAttribute())
	{
		// MaxActionPoints value can't be below 1.
		NewValue = FMath::Max(NewValue, 1.0f);
	}
}

// Saves Attributes before attribute change are applied.
bool UClairAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}
	
	HealthBeforeChange = GetHealth();
	ActionPointsBeforeChange = GetActionPoints();

	return true;
}

// Clamps changed attributes with valid values, set new attributes values and call their corresponding event.
void UClairAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	const FGameplayEffectContextHandle& EffectContext = Data.EffectSpec.GetEffectContext();
	AActor* Instigator = EffectContext.GetOriginalInstigator();
	
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// Clamp new health between 0 and MaxHealth and set new health with damage attribute
		SetHealth(FMath::Clamp(GetHealth() - GetDamage(), 0.0f, GetMaxHealth()));
		SetDamage(0.0f);
	}	
	else if (Data.EvaluatedData.Attribute == GetHealAttribute())
	{
		// Clamp new health between 0 and MaxHealth and set new health with heal attribute
		SetHealth(FMath::Clamp(GetHealth() + GetHeal(), 0.0f, GetMaxHealth()));
		SetHeal(0.0f);
	}
	else if (Data.EvaluatedData.Attribute == GetActionPointsAttribute())
	{
		// Clamp new action points between 0 and MaxActionPoints and set new action points count
		SetActionPoints(FMath::Clamp(GetActionPoints(), 0.0f, GetMaxActionPoints()));
	}
	
	if (GetHealth() != HealthBeforeChange)
	{
		OnHealthChanged.Broadcast(Instigator, HealthBeforeChange, GetHealth());
	}
	else if (GetActionPoints() != ActionPointsBeforeChange)
	{
		OnActionPointsChanged.Broadcast(Instigator, ActionPointsBeforeChange, GetActionPoints());
	}
}
