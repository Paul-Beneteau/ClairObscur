// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "ClairObscur/GameMode/TurnCharacterInterface.h"
#include "ClairObscur/GameMode/TurnDelegate.h"
#include "GameFramework/Character.h"
#include "ClairCharacter.generated.h"

class UGameplayAbility;
class UClairAttributeComp;
class UClairAttributeSet;
class UClairAbilitySystemComponent;

UCLASS()
class CLAIROBSCUR_API AClairCharacter : public ACharacter, public IAbilitySystemInterface, public ITurnCharacterInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	bool bIsMoving { false };

	UPROPERTY(BlueprintReadWrite)
	bool bIsWalkingBackward { false };
	
	AClairCharacter();

	// Implements IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Implements ITurnCharacterInterface
	virtual float GetSpeed_Implementation() const override;
	virtual void PlayTurn_Implementation() override;
	virtual UTurnDelegate* GetOnTurnEndedDelegate_Implementation() override;
	
protected:
	// GAS Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UClairAbilitySystemComponent> ClairAbilitySystemComp;
	UPROPERTY()
	TObjectPtr<UClairAttributeSet> ClairAttributeSet;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UClairAttributeComp> AttributeComp;

	// Callback when a turn is finished. Used by the TurnManagerSubsystem
	UPROPERTY()
	TObjectPtr<UTurnDelegate> OnTurnEnded;
	
	virtual void BeginPlay() override;

	// Callback when a gameplay ability ends
	void OnGameplayAbilityEnded(UGameplayAbility* GameplayAbility);

};
