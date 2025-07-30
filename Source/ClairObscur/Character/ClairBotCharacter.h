// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "ClairBotCharacter.generated.h"

class UGameplayEffect;
class UClairAttributeComp;
class UClairAttributeSet;
class UClairAbilitySystemComponent;

UCLASS()
class CLAIROBSCUR_API AClairBotCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AClairBotCharacter();

	virtual void BeginPlay() override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
protected:
	// GAS Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UClairAbilitySystemComponent> ClairAbilitySystemComp;
	UPROPERTY()
	TObjectPtr<UClairAttributeSet> ClairAttributeSet;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UClairAttributeComp> AttributeComp;
};
