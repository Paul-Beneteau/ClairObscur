// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "ClairBotCharacter.generated.h"

class UClairAttributeComp;
class UClairAttributeSet;
class UClairAbilitySystemComponent;

UCLASS()
class CLAIROBSCUR_API AClairBotCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AClairBotCharacter();

	virtual void PostInitializeComponents() override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UClairAbilitySystemComponent> ClairAbilitySystemComp;

	UPROPERTY()
	TObjectPtr<UClairAttributeSet> ClairAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UClairAttributeComp> AttributeComp;
};
