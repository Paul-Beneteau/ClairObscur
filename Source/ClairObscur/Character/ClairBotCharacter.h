// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "ClairCharacter.h"
#include "ClairPlayerCharacter.h"
#include "ClairObscur/GameMode/TurnCharacterInterface.h"
#include "GameFramework/Character.h"
#include "ClairBotCharacter.generated.h"

class UGameplayEffect;
class UClairAttributeComp;
class UClairAttributeSet;
class UClairAbilitySystemComponent;

UCLASS()
class CLAIROBSCUR_API AClairBotCharacter : public AClairCharacter
{
	GENERATED_BODY()

public:
	virtual void TakeTurn_Implementation() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	EAbilityInputID PrimaryAttack { EAbilityInputID::PrimaryAttack };
};
