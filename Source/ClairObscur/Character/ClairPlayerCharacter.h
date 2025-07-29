#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "ClairAbilitySystemComponent.h"
#include "ClairPlayerCharacter.generated.h"

enum class EAbilityInputKey : uint8;
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

// Bind an input action with a gameplay ability key
USTRUCT()
struct FClairAbilityInput
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	UInputAction* InputAction;

	UPROPERTY(EditDefaultsOnly)
	EAbilityInputKey Key;
};

UCLASS()
class CLAIROBSCUR_API AClairPlayerCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AClairPlayerCharacter();
	
	virtual void BeginPlay() override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Implements ability system interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
protected:
	// GAS components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UClairAbilitySystemComponent> ClairAbilitySystemComp;
	UPROPERTY()
	TObjectPtr<UClairAttributeSet> ClairAttributeSet;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UClairAttributeComp> AttributeComp;
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TObjectPtr<UClairAbilitySet> ClairAbilitySet;
	// Initialize attribute value with a gameplay effect
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> InitialGameplayEffect;

	// Camera components
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComp;	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComp;

	// Input
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> InputAction_Move;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> InputAction_Look;
	// Bind abilities with input
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TArray<FClairAbilityInput> ClairAbilityInputSet;
	
	void AbilityInputBindingPressedHandler(EAbilityInputKey Key);	
	void AbilityInputBindingReleasedHandler(EAbilityInputKey Key);
	
	// Moves player character according to (WASD by default) keyboard keys.
	void Move(const FInputActionInstance& MoveAxis2D);
	// Rotate the player controller according to the mouse inputs.
	void Look(const FInputActionValue& MouseAxis2D);
};
