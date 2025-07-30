#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "ClairAbilitySystemComponent.h"
#include "ClairPlayerCharacter.generated.h"

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

// Bind an input action with a gameplay ability input ID
USTRUCT()
struct FClairAbilityInput
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	UInputAction* InputAction { nullptr };

	UPROPERTY(EditDefaultsOnly)
	EAbilityInputID InputID;
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UClairAbilitySystemComponent> ClairAbilitySystemComp;
	UPROPERTY()
	TObjectPtr<UClairAttributeSet> ClairAttributeSet;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UClairAttributeComp> AttributeComp;

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
	
	void AbilityPressedHandler(EAbilityInputID InputID);	
	void AbilityReleasedHandler(EAbilityInputID InputID);
	
	// Moves player character according to (WASD by default) keyboard keys.
	void Move(const FInputActionInstance& MoveAxis2D);
	// Rotate the player controller according to the mouse inputs.
	void Look(const FInputActionValue& MouseAxis2D);
};
