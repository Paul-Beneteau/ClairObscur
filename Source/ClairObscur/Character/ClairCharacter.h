// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "ClairObscur/GameMode/TurnCharacterInterface.h"
#include "GameFramework/Character.h"
#include "ClairCharacter.generated.h"

class UGameplayAbility;
class UClairAttributeComp;
class UClairAttributeSet;
class UClairAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCharacterDelegate);

UCLASS()
class CLAIROBSCUR_API AClairCharacter : public ACharacter, public IAbilitySystemInterface, public ITurnCharacterInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTagContainer GameplayTags;

	// Character Head Icon used by UI
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UMaterial* Icon;

	UPROPERTY(BlueprintAssignable)
	FCharacterDelegate OnTurnStarted;
	UPROPERTY(BlueprintAssignable)
	FCharacterDelegate OnTurnEnded;
	
	AClairCharacter();

	// Implements IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Implements ITurnCharacterInterface
	virtual float GetSpeed_Implementation() const override;
	virtual void TakeTurn_Implementation() override;
	
protected:
	// GAS Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UClairAbilitySystemComponent> ClairAbilitySystemComp;
	UPROPERTY()
	TObjectPtr<UClairAttributeSet> ClairAttributeSet;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UClairAttributeComp> AttributeComp;
	
	virtual void BeginPlay() override;

	// Callback when a gameplay ability ends
	virtual void AbilityEndedHandler(UGameplayAbility* GameplayAbility);

};
