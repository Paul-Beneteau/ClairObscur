// Fill out your copyright notice in the Description page of Project Settings.


#include "ClairConsumableMagnitudeCalculation.h"

#include "ClairAttributeSet.h"

UClairConsumableMagnitudeCalculation::UClairConsumableMagnitudeCalculation()
{
	// Capture gameplay effect source instigator max health attribute
	SourceMaxHealthDef.AttributeToCapture = UClairAttributeSet::GetMaxHealthAttribute();
	SourceMaxHealthDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	SourceMaxHealthDef.bSnapshot = false;
	
	RelevantAttributesToCapture.Add(SourceMaxHealthDef);
}

// Return source instigator max health as base magnitude
float UClairConsumableMagnitudeCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// retrieve instigator max health
	float SourceMaxHealth { 0.0f };
	
	GetCapturedAttributeMagnitude(SourceMaxHealthDef, Spec, EvaluationParameters, SourceMaxHealth);

	return SourceMaxHealth;
}
