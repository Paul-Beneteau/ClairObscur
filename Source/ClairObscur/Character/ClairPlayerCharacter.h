#pragma once

#include "CoreMinimal.h"
#include "ClairAbilitySystemComponent.h"
#include "ClairCharacter.h"
#include "ClairPlayerInputs.h"
#include "ClairPlayerCharacter.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
enum class EAbilityInputID : uint8;
struct FGameplayAbilitySpecHandle;
class UClairAbilitySet;
class UGameplayEffect;
class UClairAttributeSet;
class UClairAbilitySystemComponent;
class UClairAttributeComp;
struct FInputActionValue;
struct FInputActionInstance;
class UCameraComponent;
class USpringArmComponent;
class UInputAction;
class UInputMappingContext;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FClairPlayerCharacterDelegate);

UCLASS()
class CLAIROBSCUR_API AClairPlayerCharacter : public AClairCharacter
{
	GENERATED_BODY()

public:
	// Camera components
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UCameraComponent> CameraComp;	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<USpringArmComponent> SpringArmComp;

	UPROPERTY(BlueprintAssignable)
	FClairPlayerCharacterDelegate OnTurnStarted;
	UPROPERTY(BlueprintAssignable)
	FClairPlayerCharacterDelegate OnAbilitySelected;
	UPROPERTY(BlueprintAssignable)
	FClairPlayerCharacterDelegate OnTargetSelected;
	
	AClairPlayerCharacter();
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Wait for the player to select an ability 
	virtual void TakeTurn_Implementation() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UClairPlayerInputs> Inputs;

	// Saves the selected ability to activate it later
	EAbilityInputID SelectedAbility { EAbilityInputID::None };

	// Saves targets to switch between them via inputs
	UPROPERTY()
	TArray<AActor*> Targets;	
	int32 CurrentTargetIndex { 0 };

	// Camera offset to view the scene when an ability is activated. Can be parametrised in BP
	UPROPERTY(EditDefaultsOnly, Category="Camera")
	FVector ActivatedAbilityCameraOffset { FVector(-300, 340, 170) };
	// Camera offset to view the selected target. Can be parametrised in BP
	UPROPERTY(EditDefaultsOnly, Category="Camera")
	FVector SelectedTargetCameraOffset { FVector(-120, 0, 60) };
	
	UPROPERTY()
	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> InputSubsystem;
	
	// Get targets and sort them by their Y axis. Switch input and HUD to select target.
	void SelectAbilityHandler(EAbilityInputID InputID);
	
	// Sets next target and moves camera to target location
	void SelectNextTargetHandler();
	
	// Sets previous target and moves camera to target location
	void SelectPreviousTargetHandler();
	
	void MoveCameraToActor(const AActor& Actor, const FVector& CameraOffset);

	// Moves camera to have a large view and activate ability
	void ActivateAbilityHandler();

	// Reset target and moves camera to his original position
	virtual void AbilityEndedHandler(UGameplayAbility* GameplayAbility) override;

	// Moves player character according to (WASD by default) keyboard keys.
	void Move(const FInputActionInstance& MoveAxis2D);
	// Rotate the player controller according to the mouse inputs.
	void Look(const FInputActionValue& MouseAxis2D);
};
