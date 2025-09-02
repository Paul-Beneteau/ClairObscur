#include "ClairPlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "../ClairGameStatics.h"
#include "Camera/CameraComponent.h"
#include "ClairAbilitySystemComponent.h"
#include "ClairBotCharacter.h"
#include "ClairConsumableComponent.h"
#include "Kismet/GameplayStatics.h"

AClairPlayerCharacter::AClairPlayerCharacter()
{	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	ClairConsumableComp = CreateDefaultSubobject<UClairConsumableComponent>(TEXT("ConsumableComp"));
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
	
	const APlayerController* PlayerController { GetController<APlayerController>() };
	check(PlayerController);	
	const ULocalPlayer* LocalPlayer { PlayerController->GetLocalPlayer() };
	check(LocalPlayer);	
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(InputSubsystem);
	
	InputSubsystem->ClearAllMappings();
	// Gives the player the ability to dodge at the start of the combat before his turn
	InputSubsystem->AddMappingContext(Inputs->WaitNextTurnContext, 0);

	UEnhancedInputComponent* InputComp { CastChecked<UEnhancedInputComponent>(PlayerInputComponent) };
	
	// Binds input actions
	InputComp->BindAction(Inputs->SelectConsumableMenu, ETriggerEvent::Started, this, &AClairPlayerCharacter::SetSelectItemContext);
	InputComp->BindAction(Inputs->ActivateFirstConsumable, ETriggerEvent::Started, this, &AClairPlayerCharacter::TryActivateConsumable, EClairConsumableKey::FirstConsumable);
	InputComp->BindAction(Inputs->ActivateSecondConsumable, ETriggerEvent::Started, this, &AClairPlayerCharacter::TryActivateConsumable, EClairConsumableKey::SecondConsumable);
	InputComp->BindAction(Inputs->SelectAbility, ETriggerEvent::Started, this, &AClairPlayerCharacter::SetSelectAbilityContext);
	InputComp->BindAction(Inputs->SelectNextTarget, ETriggerEvent::Started, this, &AClairPlayerCharacter::SelectNextTarget);
	InputComp->BindAction(Inputs->SelectPreviousTarget, ETriggerEvent::Started, this, &AClairPlayerCharacter::SelectPreviousTarget);
	InputComp->BindAction(Inputs->ActivateAbility, ETriggerEvent::Started, this, &AClairPlayerCharacter::ActivateAbility);
	InputComp->BindAction(Inputs->Cancel, ETriggerEvent::Started, this, &AClairPlayerCharacter::SetSelectActionContext);
	
	for (FClairAbilityInput Ability : Inputs->Abilities)
	{
		// Dodge doesn't need a target like rest of abilities and doesn't end turn
		if (Ability.InputID == EAbilityInputID::Dodge)
		{
			InputComp->BindAction(Ability.InputAction, ETriggerEvent::Started, ClairAbilitySystemComp,
				                  &UClairAbilitySystemComponent::ActivateAbility, Ability.InputID);
			continue;
		}

		InputComp->BindAction(Ability.InputAction, ETriggerEvent::Started, this,
				&AClairPlayerCharacter::SetSelectTargetContext, Ability.InputID);
	}
}

// Load select action context that wait for the player to choose an action. Apply the beginning turn gameplay effect
void AClairPlayerCharacter::TakeTurn_Implementation()
{
	Super::TakeTurn_Implementation();
	
	SetSelectActionContext();

	// Apply a gameplay effect at the start of the turn. Adds 1 action points each turn
	ClairAbilitySystemComp->ApplyGameplayEffectToSelf(BeginTurnGameplayEffect->GetDefaultObject<UGameplayEffect>(),
		0.0f, ClairAbilitySystemComp->MakeEffectContext());
}

// Removes current input mapping context and set a new one if parameter is provided. Moves camera to the new location
// and send corresponding context changed event
void AClairPlayerCharacter::SetContext(const EPlayerContext PlayerContext, const FTransform& CameraTransform, 
									   const UInputMappingContext* InputContext /* = nullptr */)
{
	
	const APlayerController* PlayerController { GetController<APlayerController>() };

	if (!PlayerController)
	{
		UE_LOG(ClairLog, Warning, TEXT("AClairPlayerCharacter: Player controller is null"));
		return;
	}
	
	const ULocalPlayer* LocalPlayer { PlayerController->GetLocalPlayer() };

	if (!LocalPlayer)
	{
		UE_LOG(ClairLog, Warning, TEXT("AClairPlayerCharacter: Local player is null"));
		return;
	}
	
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!InputSubsystem)
	{
		UE_LOG(ClairLog, Warning, TEXT("AClairPlayerCharacter: InputSubsystem is null"));
		return;
	}
	
	InputSubsystem->ClearAllMappings();
	
	if (InputContext)
	{
		InputSubsystem->AddMappingContext(InputContext, 0);
	}

	CameraComp->SetWorldTransform(CameraTransform);
	
	OnContextChanged.Broadcast(PlayerContext);
}

void AClairPlayerCharacter::SetSelectActionContext()
{
	SetContext(EPlayerContext::SelectAction,
		       FTransform(PlayerCameraRotation, GetActorLocation() + PlayerCameraLocationOffSet),
		    Inputs->SelectActionContext);
}

void AClairPlayerCharacter::SetSelectItemContext()
{
	SetContext(EPlayerContext::SelectConsumable,
			   FTransform(PlayerCameraRotation, GetActorLocation() + PlayerCameraLocationOffSet),
			Inputs->SelectItemContext);
}

void AClairPlayerCharacter::SetSelectAbilityContext()
{
	SetContext(EPlayerContext::SelectAbility,
		       FTransform(PlayerCameraRotation, GetActorLocation() + PlayerCameraLocationOffSet),
		    Inputs->SelectAbilityContext);
}

// Saves the selected ability and switch input context and HUD to select target. Loads targets and move camera to the
// first target
void AClairPlayerCharacter::SetSelectTargetContext(EAbilityInputID InputID)
{
	// Check that the ability can be activated before selecting it. e.g. if the character doesn't have enough action
	// points to use the ability
	if (ClairAbilitySystemComp->CanActivateAbility(InputID) == false)
	{
		return;
	}

	// Saves selected ability that will be activated if the player select a target
	SelectedAbility = InputID;

	GetTargets();
	check(Targets.IsValidIndex(CurrentTargetIndex));
		
	SetContext(EPlayerContext::SelectTarget,
		       FTransform(TargetCameraRotation, FVector (Targets[CurrentTargetIndex]->GetActorLocation() + TargetCameraLocationOffSet)),
		    Inputs->SelectTargetContext);
}

// If there is a consumable, activate it and end turn
void AClairPlayerCharacter::TryActivateConsumable(EClairConsumableKey Key)
{
	if (ClairConsumableComp->GetConsumableCount(Key))
	{
		ClairConsumableComp->ActivateConsumable(Key);
		EndTurn();
	}
}

// Gets every bot in the world and sorts them with their Y axis location
void AClairPlayerCharacter::GetTargets()
{
	// Reset target index
	CurrentTargetIndex = 0;
		
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AClairBotCharacter::StaticClass(),Targets);
		
	Targets.Sort([](const AActor& FirstActor, const AActor& SecondActor)
				 { return FirstActor.GetActorLocation().Y < SecondActor.GetActorLocation().Y; });
}

// Moves camera to next target and update target index
void AClairPlayerCharacter::SelectNextTarget()
{
	// Do nothing if the last target is already selected
	if (CurrentTargetIndex < Targets.Num() - 1)
	{
		CurrentTargetIndex++;
		CameraComp->SetWorldLocation(FVector (Targets[CurrentTargetIndex]->GetActorLocation() + TargetCameraLocationOffSet));
	}
}

// Moves camera to previous target and update target index
void AClairPlayerCharacter::SelectPreviousTarget()
{
	// Do nothing if the first target is already selected
	if (CurrentTargetIndex > 0)
	{
		CurrentTargetIndex--;		
		CameraComp->SetWorldLocation(FVector (Targets[CurrentTargetIndex]->GetActorLocation() + TargetCameraLocationOffSet));
	}	
}

// Set corresponding context and activate selected ability on the current target
void AClairPlayerCharacter::ActivateAbility()
{	
	SetContext(EPlayerContext::AbilityActivated,
		       FTransform(AbilityCameraRotation,GetActorLocation() + AbilityCameraLocationOffSet));
	
	ClairAbilitySystemComp->ActivateAbilityOnTarget(SelectedAbility, Targets[CurrentTargetIndex]);
}

// End the turn of the character if the ability isn't a dodge
void AClairPlayerCharacter::OnAbilityEndedHandler(UGameplayAbility* GameplayAbility)
{
	if (GameplayAbility->GetCurrentAbilitySpec()->InputID == static_cast<int32>(EAbilityInputID::Dodge))
	{
		return;
	}
	
	EndTurn();
}

void AClairPlayerCharacter::EndTurn()
{	
	SetContext(EPlayerContext::WaitNextTurn,
		       FTransform(PlayerCameraRotation, GetActorLocation() + PlayerCameraLocationOffSet),
		    Inputs->WaitNextTurnContext);
	
	Super::EndTurn();
}

