#include "ClairPlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "../ClairGameStatics.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "ClairAbilitySystemComponent.h"
#include "ClairBotCharacter.h"
#include "Kismet/GameplayStatics.h"

AClairPlayerCharacter::AClairPlayerCharacter()
{	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	// Player rotates the player controller when moving the mouse to look around which rotates the spring arm camera.
	SpringArmComp->bUsePawnControlRotation = true;
	// Disable side effect that rotate spring arm when capsule component is rotated (eg: with bOrientRotationToMovement)
	SpringArmComp->SetUsingAbsoluteRotation(true);
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
}

// Loads player input subsystem and mapping context then binds input actions to their corresponding functions.
void AClairPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);	

	if (Inputs == nullptr)
	{
		UE_LOG(ClairLog, Warning, TEXT("AClairPlayerCharacter: Player inputs have not been set"));
		return;
	}
	
	// Loads input subsystem
	const APlayerController* PlayerController { GetController<APlayerController>() };
	check(PlayerController);	
	const ULocalPlayer* LocalPlayer { PlayerController->GetLocalPlayer() };
	check(LocalPlayer);	
	InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(InputSubsystem);
	
	InputSubsystem->ClearAllMappings();
	InputSubsystem->AddMappingContext(Inputs->DefenseAbilityContext, 0);

	UEnhancedInputComponent* InputComp { CastChecked<UEnhancedInputComponent>(PlayerInputComponent) };
	
	// Binds input actions
	InputComp->BindAction(Inputs->Move, ETriggerEvent::Started, this, &AClairPlayerCharacter::Move);
	InputComp->BindAction(Inputs->Look, ETriggerEvent::Started, this, &AClairPlayerCharacter::Look);
	InputComp->BindAction(Inputs->SelectNextTarget, ETriggerEvent::Started, this, &AClairPlayerCharacter::SelectNextTargetHandler);
	InputComp->BindAction(Inputs->SelectPreviousTarget, ETriggerEvent::Started, this, &AClairPlayerCharacter::SelectPreviousTargetHandler);
	InputComp->BindAction(Inputs->ActivateAbility, ETriggerEvent::Started, this, &AClairPlayerCharacter::ActivateAbilityHandler);
	
	for (FClairAbilityInput Ability : Inputs->Abilities)
	{
		if (Ability.InputID == EAbilityInputID::Dodge)
		{
			InputComp->BindAction(Ability.InputAction, ETriggerEvent::Started, this,
				&AClairPlayerCharacter::Dodge, Ability.InputID);
		}
		else
		{
			InputComp->BindAction(Ability.InputAction, ETriggerEvent::Started, this,
				&AClairPlayerCharacter::SelectAbilityHandler, Ability.InputID);
		}
	}
}

// Loads input context to select an ability and wait for the player input 
void AClairPlayerCharacter::TakeTurn_Implementation()
{
	InputSubsystem->RemoveMappingContext(Inputs->DefenseAbilityContext);
	InputSubsystem->AddMappingContext(Inputs->SelectAbilityContext, 0);

	// Send Event to add menu that select ability in HUD
	OnTurnStarted.Broadcast();
}

void AClairPlayerCharacter::Dodge(EAbilityInputID InputID)
{
	CameraComp->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	ClairAbilitySystemComp->ActivateAbilityOnTarget(InputID, this);
}

// Saves the selected ability and switch input context and HUD to select target. Loads targets and move camera to the
// first target
void AClairPlayerCharacter::SelectAbilityHandler(EAbilityInputID InputID)
{
	SelectedAbility = InputID;
		
	// Switch Input context to select a target
	InputSubsystem->RemoveMappingContext(Inputs->SelectAbilityContext);
	InputSubsystem->AddMappingContext(Inputs->SelectTargetContext, 0);

	// Send Event to remove menu that select ability in HUD
	OnAbilitySelected.Broadcast();
	
	// Get Targets
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AClairBotCharacter::StaticClass(),Targets);
	check(Targets.IsValidIndex(0));
	// Sort targets with their Y axis location
	Targets.Sort([](const AActor& FirstActor, const AActor& SecondActor)
				 { return FirstActor.GetActorLocation().Y < SecondActor.GetActorLocation().Y; });

	// Moves camera to the first target
	MoveCameraToActor(*Targets[CurrentTargetIndex], SelectedTargetCameraOffset);
}

// Switch to next target et moves camera
void AClairPlayerCharacter::SelectNextTargetHandler()
{
	if (CurrentTargetIndex < Targets.Num() - 1)
	{
		CurrentTargetIndex++;
		MoveCameraToActor(*Targets[CurrentTargetIndex], SelectedTargetCameraOffset);
	}
}

// Switch to previous target et moves camera
void AClairPlayerCharacter::SelectPreviousTargetHandler()
{	
	if (CurrentTargetIndex > 0)
	{
		CurrentTargetIndex--;
		MoveCameraToActor(*Targets[CurrentTargetIndex], SelectedTargetCameraOffset);
	}	
}

void AClairPlayerCharacter::MoveCameraToActor(const AActor& Actor, const FVector& CameraOffset)
{	
	FTransform CameraTransform;
	CameraTransform.SetLocation(Actor.GetActorLocation() + CameraOffset);
	CameraTransform.SetRotation(UE::Math::TQuat<double>(0.0, 0.0, 0.0, 0.0));
	
	CameraComp->SetWorldTransform(CameraTransform);
}

void AClairPlayerCharacter::ActivateAbilityHandler()
{
	InputSubsystem->RemoveMappingContext(Inputs->SelectTargetContext);

	// Send Event to remove menu that select target in HUD
	OnTargetSelected.Broadcast();
	
	// Moves camera to have a large view of the activated ability
	CameraComp->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));	
	CameraComp->SetWorldLocation(GetActorLocation() + ActivatedAbilityCameraOffset);
	CameraComp->SetWorldRotation(FRotator(-20.0, -20.0, 0.0).Quaternion());
	
	ClairAbilitySystemComp->ActivateAbilityOnTarget(SelectedAbility, Targets[CurrentTargetIndex]);
}

void AClairPlayerCharacter::AbilityEndedHandler(UGameplayAbility* GameplayAbility)
{	
	InputSubsystem->AddMappingContext(Inputs->DefenseAbilityContext, 0);
	
	// Moves camera to his original position
	CameraComp->AttachToComponent(SpringArmComp, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false));
	CameraComp->SetRelativeRotation(FRotator(-20.0, 0.0, 0.0).Quaternion());

	// Reset target
	CurrentTargetIndex = 0;

	OnTurnEnded.Broadcast();
	
	Super::AbilityEndedHandler(GameplayAbility);
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


