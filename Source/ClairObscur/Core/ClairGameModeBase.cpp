#include "ClairGameModeBase.h"

#include "TurnManagerSubsystem.h"
#include "ClairObscur/Core/ClairGameStatics.h"
#include "ClairObscur/Character/ClairAttributeComp.h"
#include "ClairObscur/Character/ClairBotCharacter.h"
#include "ClairObscur/Character/ClairCharacter.h"
#include "ClairObscur/Character/ClairPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

void AClairGameModeBase::StartPlay()
{
	Super::StartPlay();
	
	TArray<AActor*> Characters;
	
	UGameplayStatics::GetAllActorsOfClass(this, AClairCharacter::StaticClass(), Characters);

	// Bind OnDeath event of characters in world to check when the combat ends
	for (AActor* Character : Characters)
	{
		if (UClairAttributeComp* AttributeComp = Character->FindComponentByClass<UClairAttributeComp>())
		{
			AttributeComp->OnDeathStatusChanged.AddUniqueDynamic(this, &AClairGameModeBase::CheckCombatEnd);
		}
	}

	if (UTurnManagerSubsystem* TurnManagerSubsystem = GetGameInstance()->GetSubsystem<UTurnManagerSubsystem>())
	{	
		TurnManagerSubsystem->Start();
	}
}

// Get the number of player and bot character to check if the combat is win or lost. Send event corresponding to the
// win/loose
void AClairGameModeBase::CheckCombatEnd()
{
	int32 PlayerCharacterCount { 0 };
	int32 BotCharacterCount { 0 };
	TArray<AActor*> Characters;

	UGameplayStatics::GetAllActorsOfClass(this, AClairCharacter::StaticClass(), Characters);

	// Get player and bot character count
	for (AActor* Character : Characters)
	{
		AClairPlayerCharacter* PlayerCharacter = Cast<AClairPlayerCharacter>(Character) ;

		// If this is a player character which is not pending kill
		if (PlayerCharacter && PlayerCharacter->GetLifeSpan() == 0.0f)
		{
			PlayerCharacterCount++;
		}

		AClairBotCharacter* BotCharacter = Cast<AClairBotCharacter>(Character) ;

		// If this is a player character which is not pending kill
		if (BotCharacter && BotCharacter->GetLifeSpan() == 0.0f)
		{
			BotCharacterCount++;
		}
	}
	
	UTurnManagerSubsystem* TurnManagerSubsystem = GetGameInstance()->GetSubsystem<UTurnManagerSubsystem>();
	
	// If there is no more player combat is lost. Pause TurnManagerSubsystem so bots won't play turn while player is dead
	if (PlayerCharacterCount == 0)
	{		
		if (TurnManagerSubsystem)
		{
			TurnManagerSubsystem->Pause();
		}
		
		OnCombatLost.Broadcast();
	}

	// If there is not more bot and there is still a player alive combat is win 
	if (BotCharacterCount == 0 && PlayerCharacterCount != 0)
	{		
		OnCombatWin.Broadcast();
	}
}

// Open the default level, send OnCombatRestart event to change the current UI and reset turn order
void AClairGameModeBase::RestartCombat() const
{
	if (LevelName.IsNone())
	{
		UE_LOG(ClairLog, Warning, TEXT("AClairGameModeBase: Level name has not been set"));
		return;
	}

	OnCombatRestart.Broadcast();
	
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);

	// Reset turn order and unpause TurnManagerSubsystem with a delay to wait for the level to be loaded 
	if (UTurnManagerSubsystem* TurnManagerSubsystem = GetGameInstance()->GetSubsystem<UTurnManagerSubsystem>())
	{
		FTimerHandle ResetTimerHandle;
		FTimerHandle UnpauseTimerHandle;
		
		GetWorld()->GetTimerManager().SetTimer(ResetTimerHandle, TurnManagerSubsystem,
			&UTurnManagerSubsystem::ResetTurnQueue, OpenLevelDelay, false);
		
		GetWorld()->GetTimerManager().SetTimer(UnpauseTimerHandle, TurnManagerSubsystem,
			&UTurnManagerSubsystem::Unpause, OpenLevelDelay + 0.1f, false);
	}
}
