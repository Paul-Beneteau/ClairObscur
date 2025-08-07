// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "ClairObscur/GameMode/TurnCharacterInterface.h"
#include "GameFramework/Character.h"
#include "ClairBotCharacter.generated.h"

class UGameplayEffect;
class UClairAttributeComp;
class UClairAttributeSet;
class UClairAbilitySystemComponent;

UCLASS()
class CLAIROBSCUR_API AClairBotCharacter : public ACharacter, public IAbilitySystemInterface, public ITurnCharacterInterface
{
	GENERATED_BODY()

public:
	AClairBotCharacter();

	virtual void BeginPlay() override;
	
	// Implements IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Implements ITurnCharacterInterface
	virtual void TakeTurn_Implementation() override;
	virtual float GetSpeed_Implementation() const override;
	
protected:
	// GAS Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UClairAbilitySystemComponent> ClairAbilitySystemComp;
	UPROPERTY()
	TObjectPtr<UClairAttributeSet> ClairAttributeSet;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UClairAttributeComp> AttributeComp;
};
