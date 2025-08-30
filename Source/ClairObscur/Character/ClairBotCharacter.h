// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ClairCharacter.h"
#include "ClairPlayerCharacter.h"
#include "ClairBotCharacter.generated.h"

UCLASS()
class CLAIROBSCUR_API AClairBotCharacter : public AClairCharacter
{
	GENERATED_BODY()

public:
	virtual void TakeTurn_Implementation() override;

protected:
	// Attack used to hit player
	UPROPERTY(EditDefaultsOnly, Category="Attack")
	EAbilityInputID PrimaryAttack { EAbilityInputID::PrimaryAttack };

	virtual void OnAbilityEndedHandler(UGameplayAbility* GameplayAbility) override;
};
