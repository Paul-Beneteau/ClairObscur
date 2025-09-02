// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "ClairObscur/Core/TurnCharacterInterface.h"
#include "GameFramework/Character.h"
#include "ClairCharacter.generated.h"

class UGameplayAbility;
class UClairAttributeComp;
class UClairAttributeSet;
class UClairAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTurnChanged);

UCLASS()
class CLAIROBSCUR_API AClairCharacter : public ACharacter, public IAbilitySystemInterface, public ITurnCharacterInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTagContainer GameplayTags;

	// Tag used to check player defenseless status. This should be moved in a dataset when character has multiple status
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag DefencelessTag { FGameplayTag::EmptyTag };
	
	// Character Head Icon used by UI
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UMaterial* Icon;

	UPROPERTY(BlueprintAssignable)
	FOnTurnChanged OnTurnStarted;	
	UPROPERTY(BlueprintAssignable)
	FOnTurnChanged OnTurnEnded;
	
	AClairCharacter();

	// Implements IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Implements ITurnCharacterInterface
	virtual float GetSpeed_Implementation() const override;	
	virtual void TakeTurn_Implementation() override;
	
protected:
	// GAS Components. Pointer is used instead of TObjectPtr to bind callback with player input
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UClairAbilitySystemComponent* ClairAbilitySystemComp;	
	UPROPERTY()
	TObjectPtr<UClairAttributeSet> ClairAttributeSet;	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UClairAttributeComp> ClairAttributeComp;

	FGameplayTagContainer PreviousTurnGameplayTags;
	
	virtual void BeginPlay() override;

	// Check if the turn must end. If so reset target and moves camera to his original position
	virtual void OnAbilityEndedHandler(UGameplayAbility* GameplayAbility);

	// End turn from turn manager subsystem
	virtual void EndTurn();
};
