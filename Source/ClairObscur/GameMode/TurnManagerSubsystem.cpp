// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnManagerSubsystem.h"

#include "TurnCharacterInterface.h"
#include "ClairObscur/ClairGameStatics.h"
#include "Kismet/GameplayStatics.h"

void UTurnManagerSubsystem::Start()
{
	InitWorldTurnCharacters();
	InitSlowestTurnCharacter();	
	InitTurnsPerRound();	
	InitTurnQueue();

	// TODO: Implements ClairPlayerCharacter TakesTurn to remove the for loop magic number which is used for testing
	// purpose.
	for (int i = 0; i < 13; i++)
	{
		NextTurnCharacterTakesTurn();
	}
}

// Retrieves actors in the world that implements TurnCharacterInterface in WorldTurnCharacters member
void UTurnManagerSubsystem::InitWorldTurnCharacters()
{
	TArray<AActor*> TurnCharacters;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UTurnCharacterInterface::StaticClass(), TurnCharacters);

	if (TurnCharacters.IsEmpty())
	{
		UE_LOG(ClairLog, Warning, TEXT("TurnManagerSubsystem: There isn't any TurnCharacter in the world"));
		return;
	}
	
	for (AActor* TurnCharacter : TurnCharacters)
	{		
		WorldTurnCharacters.Emplace(FTurnCharacter { TurnCharacter });
	}
}

// Find slowest TurnCharacter and saves it in SlowestTurnCharacter member
void UTurnManagerSubsystem::InitSlowestTurnCharacter()
{
	check(WorldTurnCharacters.IsValidIndex(0));	
	float SlowestSpeed = ITurnCharacterInterface::Execute_GetSpeed(WorldTurnCharacters[0].Actor);
	SlowestTurnCharacter = WorldTurnCharacters[0].Actor;
		
	for (FTurnCharacter TurnCharacter : WorldTurnCharacters)
	{
		const float TurnCharacterSpeed { ITurnCharacterInterface::Execute_GetSpeed(TurnCharacter.Actor) };
		if (TurnCharacterSpeed < SlowestSpeed)
		{
			SlowestSpeed = TurnCharacterSpeed;
			SlowestTurnCharacter = TurnCharacter.Actor;
		}
	}
}

// Computes numbers of turns per round for each TurnCharacters. A round consists of a series of turns where every
// TurnCharacters takes at least one turn. This number of turn per round is equal to the TurnCharacterSpeed divided by
// the slowest TurnCharacter speed.
void UTurnManagerSubsystem::InitTurnsPerRound()
{
	for (FTurnCharacter& TurnCharacter : WorldTurnCharacters)
	{
		float SlowestSpeed { ITurnCharacterInterface::Execute_GetSpeed(SlowestTurnCharacter) };
		// Cannot divide TurnCharacter speed by 0
		check(SlowestSpeed != 0.0f);
			
		// The number of turn per round is equal to the TurnCharacter speed divided by the slowest TurnCharacter speed.
		// e.g. TurnCharacter 1 has 300 speed and TurnCharacter 2 has 100 speed. TurnCharacter 1 takes 3 turns in
		// the round (300 / 100) and TurnCharacter 2 takes 1 turn (100 / 100)
		TurnCharacter.TurnsPerRound = ITurnCharacterInterface::Execute_GetSpeed(TurnCharacter.Actor) / SlowestSpeed;
		TurnCharacter.RemainingTurnsPerRound = TurnCharacter.TurnsPerRound;
	}	
}

// Fill the TurnQueue with firsts TurnCharacters that takes turn. Number of TurnCharacters is defined by TurnQueueSize
void UTurnManagerSubsystem::InitTurnQueue()
{
	for (int32 i = 0; i < TurnQueueSize; i++)
	{
		const AActor* NextTurnCharacter = EnQueueNextTurnCharacter();
		
		if (IsEndOfRound(NextTurnCharacter))
		{
			UpdateRemainingTurnsPerRound();
		}		
	}
}

// Next TurnCharacter takes his turn, is removed from the queue and the next TurnCharacter is added to the queue
// Checks for end of the turn to update TurnCharacters numbers of remaining turns per round
void UTurnManagerSubsystem::NextTurnCharacterTakesTurn()
{
	const TOptional<AActor*> TurnCharacter { TurnQueue.Dequeue() };	
	check(TurnCharacter.Get(nullptr));
	
	ITurnCharacterInterface::Execute_TakeTurn(TurnCharacter.Get(nullptr));

	const AActor* NextTurnCharacter = EnQueueNextTurnCharacter();

	if (IsEndOfRound(NextTurnCharacter))
	{
		UpdateRemainingTurnsPerRound();
	}
}

// Enqueue the TurnCharacter with the hightest TurnsPerRound that is next character to take turn after all TurnCharacter
// in the queue took their turn.
AActor* UTurnManagerSubsystem::EnQueueNextTurnCharacter()
{
	AActor* NextTurnCharacter { nullptr };
	check(WorldTurnCharacters.IsValidIndex(0));
	float* HighestTurnsPerRound { &WorldTurnCharacters[0].RemainingTurnsPerRound };

	// Finds the TurnCharacter with the hightest TurnsPerRound 
	for (FTurnCharacter& TurnCharacter : WorldTurnCharacters)
	{		
		if (TurnCharacter.RemainingTurnsPerRound >= *HighestTurnsPerRound)
		{
			HighestTurnsPerRound = &TurnCharacter.RemainingTurnsPerRound;
			NextTurnCharacter = TurnCharacter.Actor;
		}
	}

	// Subtracts 1 to RemainingTurnsPerRound because the TurnCharacter will take a turn
	*HighestTurnsPerRound = *HighestTurnsPerRound - 1;
	
	TurnQueue.Enqueue(NextTurnCharacter);
	
	return NextTurnCharacter;
}

bool UTurnManagerSubsystem::IsEndOfRound(const AActor* NextTurnCharacter) const
{
	// A round ends after The slowest TurnCharacter takes his turn.
	return SlowestTurnCharacter == NextTurnCharacter;
}

// Adds number of turns per round to the remaining number of turns per round for every TurnCharacter.
// It Should be used at the end of a round
void UTurnManagerSubsystem::UpdateRemainingTurnsPerRound()
{
	for (FTurnCharacter& TurnCharacter : WorldTurnCharacters)
	{			
		TurnCharacter.RemainingTurnsPerRound += TurnCharacter.TurnsPerRound;
	}	
}