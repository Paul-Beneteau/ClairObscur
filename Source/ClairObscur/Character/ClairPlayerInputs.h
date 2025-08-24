// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ClairPlayerInputs.generated.h"

struct FClairAbilityInput;
class UInputAction;
class UInputMappingContext;

// Links a gameplay ability with an input ID used to create or activate the ability
UENUM(BlueprintType)
enum class EAbilityInputID : uint8
{
	None = 0 UMETA(Hidden),
	PrimaryAttack = 1,
	Dodge = 2,
	PrimaryFireAbility = 3,
};

// Bind an input action with a gameplay ability input ID
USTRUCT()
struct FClairAbilityInput
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	UInputAction* InputAction { nullptr };

	UPROPERTY(EditDefaultsOnly)
	EAbilityInputID InputID { EAbilityInputID::None };
};

UCLASS()
class CLAIROBSCUR_API UClairPlayerInputs : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputMappingContext> SelectAbilityContext;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputMappingContext> SelectTargetContext;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputMappingContext> DefenseAbilityContext;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Move;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Look;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> SelectNextTarget;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> SelectPreviousTarget;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> ActivateAbility;

	// Associates input action actions with an ability input ID
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TArray<FClairAbilityInput> Abilities;
};
