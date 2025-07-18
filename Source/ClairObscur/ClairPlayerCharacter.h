#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "ClairPlayerCharacter.generated.h"

class UClairAttributeSet;
class UClairAbilitySystemComponent;
class UClairAttributeComp;
struct FInputActionValue;
struct FInputActionInstance;
class UCameraComponent;
class USpringArmComponent;
class UInputAction;
class UInputMappingContext;

UCLASS()
class CLAIROBSCUR_API AClairPlayerCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:	
	AClairPlayerCharacter();

	virtual void PostInitializeComponents() override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UClairAbilitySystemComponent> ClairAbilitySystemComp;

	UPROPERTY()
	TObjectPtr<UClairAttributeSet> ClairAttributeSet;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComp;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComp;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_Move;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_Look;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UClairAttributeComp> AttributeComp;

	// Moves player character according to (WASD by default) keyboard keys.
	void Move(const FInputActionInstance& MoveAxis2D);

	// Rotate the player controller according to the mouse inputs.
	void Look(const FInputActionValue& MouseAxis2D);
};
