// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnManagerSubsystem.h"

#include "TurnCharacterInterface.h"
#include "ClairObscur/ClairGameStatics.h"
#include "Kismet/GameplayStatics.h"

void UTurnManagerSubsystem::Start()
{
	// Get Characters in world and compute their turns per round used to get the turn order and fill the queue
	GetCharacters();
	GetSlowestCharacter();	
	ComputeTurnsPerRound();	
	FillTurnQueue();
	
	// Call StartNextTurn() when a character turn ends
	OnTurnEnded.AddUniqueDynamic(this, &UTurnManagerSubsystem::StartNextTurn);
	
	// Send Turn ended event instead of StartNextTurn() call to initialize UI
	OnTurnEnded.Broadcast();
}

void UTurnManagerSubsystem::EndTurn() const
{
	// Call StartNextTurn()
	OnTurnEnded.Broadcast();
}

void UTurnManagerSubsystem::Pause()
{
	// Remove StartNextTurn() binding so that next character won't take his turn
	OnTurnEnded.RemoveAll(this);
}

void UTurnManagerSubsystem::Pause(const float UnpauseDelay)
{
	OnTurnEnded.RemoveAll(this);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UTurnManagerSubsystem::Unpause, UnpauseDelay, false);
}

void UTurnManagerSubsystem::Unpause()
{
	// Call StartNextTurn() when a character turn ends
	OnTurnEnded.AddUniqueDynamic(this, &UTurnManagerSubsystem::StartNextTurn);
	
	// Send Turn ended event instead of StartNextTurn() call to initialize UI
	OnTurnEnded.Broadcast();
}

void UTurnManagerSubsystem::ResetTurnQueue()
{
	TurnQueue.Reset();
	
	GetCharacters();
	GetSlowestCharacter();	
	ComputeTurnsPerRound();
	
	FillTurnQueue();
}

void UTurnManagerSubsystem::RemoveCharacter(const AActor* InCharacter)
{	
	Characters.RemoveAll([&](const FTurnCharacter TurnCharacter)
	{
		return TurnCharacter.Actor == InCharacter;
	});

	TurnQueue.RemoveAll([&](const AActor* TurnQueueCharacter)
	{
		return TurnQueueCharacter == InCharacter;
	});
	
	FillTurnQueue();
}

// Dequeue a character that takes his turn then enqueue another character. Updates character remaining turns per round
// if this is the end of a round
void UTurnManagerSubsystem::StartNextTurn()
{
	AActor* DequeuedCharacter = DeQueueCharacter();

	if (DequeuedCharacter == nullptr)
	{
		UE_LOG(ClairLog, Warning, TEXT("TurnManagerSubsystem: DequeuedCharacter is null"));
		return;
	}
	
	if (IsEndOfRound(EnQueueCharacter()))
	{
		UpdateRemainingTurnsPerRound();
	}

	ITurnCharacterInterface::Execute_TakeTurn(DequeuedCharacter);
}

// Retrieves actors in the world that implements TurnCharacterInterface in Characters class member.
void UTurnManagerSubsystem::GetCharacters()
{
	TArray<AActor*> TurnCharacters;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UTurnCharacterInterface::StaticClass(), TurnCharacters);
	
	if (TurnCharacters.IsEmpty())
	{
		UE_LOG(ClairLog, Warning, TEXT("TurnManagerSubsystem: There isn't any character implementing ITurnCharacterInterface in the world"));
		return;
	}

	Characters.Reset();
	
	for (AActor* TurnCharacter : TurnCharacters)
	{
		// If character isn't dying
		if (TurnCharacter->GetLifeSpan() == 0.0f)
		{
			Characters.Emplace(FTurnCharacter { TurnCharacter });			
		}
	}
}

// Find the slowest character and saves it in SlowestCharacter class member
void UTurnManagerSubsystem::GetSlowestCharacter()
{
	check(Characters.IsValidIndex(0));	
	float SlowestSpeed = ITurnCharacterInterface::Execute_GetSpeed(Characters[0].Actor);
	SlowestCharacter = Characters[0].Actor;
		
	for (FTurnCharacter TurnCharacter : Characters)
	{
		const float TurnCharacterSpeed { ITurnCharacterInterface::Execute_GetSpeed(TurnCharacter.Actor) };
		if (TurnCharacterSpeed < SlowestSpeed)
		{
			SlowestSpeed = TurnCharacterSpeed;
			SlowestCharacter = TurnCharacter.Actor;
		}
	}
}

// Computes numbers of turns per round for each character. A round consists of a series of turns where every
// TurnCharacters takes at least one turn. This number of turn per round is equal to the character speed divided by
// the slowest character speed.
void UTurnManagerSubsystem::ComputeTurnsPerRound()
{
	for (FTurnCharacter& Character : Characters)
	{
		float SlowestSpeed { ITurnCharacterInterface::Execute_GetSpeed(SlowestCharacter) };
		// Cannot divide character speed by 0
		check(SlowestSpeed != 0.0f);
			
		// The number of turn per round is equal to the character speed divided by the slowest character speed.
		// e.g. cCharacter 1 has 300 speed and character 2 has 100 speed. character 1 takes 3 turns in the round
		// (300 / 100) and character 2 takes 1 turn (100 / 100)
		Character.TurnsPerRound = ITurnCharacterInterface::Execute_GetSpeed(Character.Actor) / SlowestSpeed;
		Character.RemainingTurnsPerRound = Character.TurnsPerRound;
	}	
}

// Fill the TurnQueue with firsts characters that takes turn. Number of characters is defined by TurnQueueSize
void UTurnManagerSubsystem::FillTurnQueue()
{
	for (int32 i = TurnQueue.Num(); i < TurnQueueSize; i++)
	{
		const AActor* NextTurnCharacter = EnQueueCharacter();
		
		if (IsEndOfRound(NextTurnCharacter))
		{
			UpdateRemainingTurnsPerRound();
		}		
	}
}

AActor* UTurnManagerSubsystem::DeQueueCharacter()
{
	if (TurnQueue.IsValidIndex(0) == false)
	{
		return nullptr;
	}
	
	AActor* DequeuedCharacter = TurnQueue[0];
	
	TurnQueue.RemoveAt(0);
	
	return DequeuedCharacter;
}

// Enqueue the character with the hightest TurnsPerRound which will be next character to take turn after all characters
// in the queue took their turn.
AActor* UTurnManagerSubsystem::EnQueueCharacter()
{
	AActor* QueuedCharacter { nullptr };
	check(Characters.IsValidIndex(0));
	float* HighestTurnsPerRound { &Characters[0].RemainingTurnsPerRound };

	// Finds the character with the hightest TurnsPerRound 
	for (FTurnCharacter& Character : Characters)
	{
		if (Character.RemainingTurnsPerRound >= *HighestTurnsPerRound)
		{
			HighestTurnsPerRound = &Character.RemainingTurnsPerRound;
			QueuedCharacter = Character.Actor;
		}
	}

	// Subtracts 1 to RemainingTurnsPerRound because the queued character will take a turn
	*HighestTurnsPerRound = *HighestTurnsPerRound - 1;
	
	TurnQueue.Add(QueuedCharacter);
	
	return QueuedCharacter;
}

bool UTurnManagerSubsystem::IsEndOfRound(const AActor* Character) const
{
	// A round ends after The slowest TurnCharacter takes his turn.
	return SlowestCharacter == Character;
}

// Adds number of turns per round to the remaining number of turns per round for every character. It Should be used at
// the end of a round
void UTurnManagerSubsystem::UpdateRemainingTurnsPerRound()
{
	for (FTurnCharacter& Character : Characters)
	{			
		Character.RemainingTurnsPerRound += Character.TurnsPerRound;
	}	
}

void UTurnManagerSubsystem::PrintQueue()
{
	UE_LOG(ClairLog, Display, TEXT("UTurnManagerSubsystem::PrintQueue"));
	for (AActor* Actor : TurnQueue)
	{
		UE_LOG(ClairLog, Display, TEXT("Character in queue: %s"), *GetNameSafe(Actor));
	}
}