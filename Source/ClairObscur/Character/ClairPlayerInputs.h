#pragma once

#include "CoreMinimal.h"
#include "ClairAbilitySystemComponent.h"
#include "Engine/DataAsset.h"
#include "ClairPlayerInputs.generated.h"

class UInputAction;
class UInputMappingContext;

// Associate an input action with a player ability via an ability key
USTRUCT()
struct FClairAbilityInput
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> InputAction { nullptr };

	UPROPERTY(EditDefaultsOnly)
	EClairAbilityKey Key { EClairAbilityKey::None };
};

// Contains every player InputMappingContext, InputAction and abilities input
UCLASS()
class CLAIROBSCUR_API UClairPlayerInputs : public UDataAsset
{
	GENERATED_BODY()

public:
	// InputMappingContext corresponding of each player context
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputMappingContext> SelectActionContext;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputMappingContext> SelectConsumableContext;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputMappingContext> SelectAbilityContext;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputMappingContext> SelectTargetContext;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputMappingContext> WaitNextTurnContext;

	// Player actions
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> SelectConsumableMenu;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> SelectAbilityMenu;

	// Consumable menu 
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> ActivateFirstConsumable;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> ActivateSecondConsumable;

	// Possible input actions when an ability is selected
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> SelectNextTarget;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> SelectPreviousTarget;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> ActivateAbility;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Cancel;
	
	// Player abilities with their associated input
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TArray<FClairAbilityInput> Abilities;
};
