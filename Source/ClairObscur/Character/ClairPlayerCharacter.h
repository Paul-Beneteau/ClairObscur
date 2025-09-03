#pragma once

#include "CoreMinimal.h"
#include "ClairAbilitySystemComponent.h"
#include "ClairCharacter.h"
#include "ClairPlayerInputs.h"
#include "ClairPlayerCharacter.generated.h"

enum class EClairConsumableKey : uint8;
class UClairConsumableComponent;
enum class EClairAbilityKey : uint8;
class UGameplayEffect;
class UCameraComponent;
class UInputMappingContext;

// Represents possible contexts that the player can be in from using inputs
UENUM(BlueprintType)
enum class EPlayerContext : uint8
{
	None = 0 UMETA(Hidden),
	// Player must select an action (Select object/Select ability/Attack)
	SelectAction = 1,
	SelectConsumable = 2,
	SelectAbility = 3,
	// Player must choose a target for the selected ability
	SelectTarget = 4,
	// Selected ability is being activated
	AbilityActivated = 5,
	// Player is waiting for bots to play their turns
	WaitNextTurn = 6,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnContextChanged, EPlayerContext, ContextEvent);

// Base class for character used by a player. Manages player inputs and player context. Context is composed of a camera
// view, a unique input mapping context that can be used by the player and the player HUD which is managed in blueprint
// by assigning the delegate OnContextChanged to be modified according to the player context.
UCLASS()
class CLAIROBSCUR_API AClairPlayerCharacter : public AClairCharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UCameraComponent> CameraComp;	

	// Delegate used by HUD to know when the current player context
	UPROPERTY(BlueprintAssignable)
	FOnContextChanged OnContextChanged;
	
	// Manage consumable used by player
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Consumable")
	TObjectPtr<UClairConsumableComponent> ClairConsumableComp;
	
	AClairPlayerCharacter();
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Start player turn
	virtual void TakeTurn_Implementation() override;

protected:	
	// Dataset containing player inputs
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UClairPlayerInputs> Inputs;

	// Saves the selected ability that can be activated later if the player choose a target
	EClairAbilityKey SelectedAbility { EClairAbilityKey::None };

	// Enemy Targets found. Target are sorted by their Y axis location
	UPROPERTY()
	TArray<AActor*> Targets;
	// Current target of the selected ability
	int32 CurrentTargetIndex { 0 };

	// Gameplay effect applied at the beginning of each turn. Gives 1 AP for now
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> BeginTurnGameplayEffect;

	// Camera rotation and location used for every player context
	FRotator PlayerCameraRotation { FRotator(-20.0, 0.0, 0.0).Quaternion() };
	FVector PlayerCameraLocationOffSet {  FVector(-165, 90, 70) };

	FRotator TargetCameraRotation { FRotator::ZeroRotator };
	FVector TargetCameraLocationOffSet {  FVector(-120, 0, 60) };

	FRotator AbilityCameraRotation { FRotator(-20.0, -20.0, 0.0).Quaternion() };
	FVector AbilityCameraLocationOffSet {  FVector(-300, 340, 170) };

	// Set a context depending on player input. It is composed of a camera transform that represents the scene view, an
	// input mapping context that represent every inputs that can be used in this context and a player context that is
	// sent via an event to change HUD accordingly
	void SetContext(const EPlayerContext PlayerContext, const FTransform& CameraTransform, const UInputMappingContext* InputContext = nullptr);

	// Set Context where player must select an action (Select consumable menu/Select ability menu/Attack)
	void SetSelectActionContext();

	// Set Context where player must select a consumable (Heal tint/Energy tint)
	void SetSelectConsumableContext();
	
	// Set Context where player must select an ability
	void SetSelectAbilityContext();
	
	// Set Context where player must select a target after having selected an ability
	void SetSelectTargetContext(const EClairAbilityKey Key);

	// Try to activate consumable and end turn if it is activated
	void TryActivateConsumable(const EClairConsumableKey Key);
	
	// Get player targets and sort them by their Y axis.
	void GetTargets();
	
	// Sets next target and moves camera to target location
	void SelectNextTarget();
	
	// Sets previous target and moves camera to target location
	void SelectPreviousTarget();
	
	// Moves camera to have a large view and activate ability
	void ActivateAbility();

	// Check if the turn must end. If so reset target and moves camera to his original position
	virtual void OnAbilityEndedHandler(UGameplayAbility* GameplayAbility) override;
	
	virtual void EndTurn() override;
};
