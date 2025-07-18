#include "ClairPlayerCharacter.h"

#include "ClairAttributeComp.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ClairGameStatics.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/BlueprintTypeConversions.h"
#include "Kismet/KismetMathLibrary.h"
#include "ClairAbilitySystemComponent.h"
#include "ClairAttributeSet.h"

AClairPlayerCharacter::AClairPlayerCharacter()
{
	ClairAbilitySystemComp = CreateDefaultSubobject<UClairAbilitySystemComponent>(TEXT("ClairAbilitySystemComp"));
	ClairAttributeSet = CreateDefaultSubobject<UClairAttributeSet>(TEXT("ClairAttributeSet"));
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	// Player rotates the player controller when moving the mouse to look around which rotates the spring arm camera.
	SpringArmComp->bUsePawnControlRotation = true;
	// Disable side effect that rotate spring arm when capsule component is rotated (eg: with bOrientRotationToMovement)
	SpringArmComp->SetUsingAbsoluteRotation(true);
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	AttributeComp = CreateDefaultSubobject<UClairAttributeComp>(TEXT("AttributeComp"));
}

// Initializes clair ability system component with his attribute set.
void AClairPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(ClairAbilitySystemComp);
	ClairAbilitySystemComp->InitAbilityActorInfo(this, this);
	AttributeComp->InitializeWithAbilitySystem(ClairAbilitySystemComp);
}

UAbilitySystemComponent* AClairPlayerCharacter::GetAbilitySystemComponent() const
{
	return ClairAbilitySystemComp;
}

// Loads player input subsystem and mapping context then binds input actions to their corresponding functions.
void AClairPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);	

	// Loads input subsystem and context
	const APlayerController* PlayerController { GetController<APlayerController>() };
	check(PlayerController);	
	const ULocalPlayer* LocalPlayer { PlayerController->GetLocalPlayer() };
	check(LocalPlayer);	
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem { LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>() };
	check(InputSubsystem);
	
	InputSubsystem->ClearAllMappings();
	InputSubsystem->AddMappingContext(InputMappingContext, 0);

	// Binds input actions
	UEnhancedInputComponent* InputComp { CastChecked<UEnhancedInputComponent>(PlayerInputComponent) };
	
	InputComp->BindAction(Input_Move, ETriggerEvent::Triggered, this, &AClairPlayerCharacter::Move);
	InputComp->BindAction(Input_Look, ETriggerEvent::Triggered, this, &AClairPlayerCharacter::Look);
}

// Gets the forward and right axis player input movement and moves in the corresponding direction.
// (e.g. by default keyboard key W and S forms the forward axis. A and D forms the right axis)
void AClairPlayerCharacter::Move(const FInputActionInstance& MoveAxis2D)
{
	FRotator Direction { FRotator::ZeroRotator };
	Direction.Yaw = GetControlRotation().Yaw;
	
	const double ForwardAxis { MoveAxis2D.GetValue().Get<FVector2D>().Y };	
	AddMovementInput(Direction.Vector(), ForwardAxis);
	
	const double RightAxis { MoveAxis2D.GetValue().Get<FVector2D>().X };	
	AddMovementInput(Direction.RotateVector(FVector::RightVector), RightAxis);	
}

// Rotates yaw player controller according the up and down mouse axis input and rotates pitch according to the left and
// right mouse axis input.
void AClairPlayerCharacter::Look(const FInputActionValue& MouseAxis2D)
{
	AddControllerYawInput(MouseAxis2D.Get<FVector2D>().X);
	AddControllerPitchInput(-MouseAxis2D.Get<FVector2D>().Y);
}



