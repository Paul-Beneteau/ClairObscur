// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/SpscQueue.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TurnManagerSubsystem.generated.h"

// Used to store an actor with TurnCharacter interface and his number of turns per round
USTRUCT()
struct FTurnCharacter
{
	GENERATED_BODY()

	UPROPERTY()
	AActor* Actor { nullptr };

	// Number of turns in a round. A round consists of a series of turns where all TurnCharacters takes at least one turn
	// e.g. TurnCharacter 1 has 3 TurnsPerRound and TurnCharacter 2 has 1 TurnsPerRound. TurnCharacter 1 will take 3 
	// turns during the round and TurnCharacter 2 will take 1 Turn
	float TurnsPerRound { 0.0f };

	// Number of remaining turns in the round. it decreases by one each time a TurnCharacter takes a turn.
	float RemainingTurnsPerRound { 0.0f };
};

// Manages actor that implements TurnCharacterInterface in the world and makes them take turn in an order managed by the
// TurnManager
UCLASS()
class CLAIROBSCUR_API UTurnManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	// Initializes class members then TurnCharacters managed by the TurnManager takes their turn in the order computed
	// from their speed.
	void Start();
		
protected:
	// Contains every actor that implements TurnCharacterInterface in the world 
	UPROPERTY()
	TArray<FTurnCharacter> WorldTurnCharacters;

	// FIFO that Contains next TurnCharacters that will take turn
	TSpscQueue<AActor*> TurnQueue;

	int32 TurnQueueSize { 6 };

	// Saves the slowest TurnCharacter used to compute the number of turn per round of each TurnCharacter
	UPROPERTY()
	AActor* SlowestTurnCharacter { nullptr };

	// Initializes WorldTurnCharacters member with actors in the world that implements TurnCharacterInterface
	void InitWorldTurnCharacters();	
	// Saves Slowest TurnCharacter in SlowestTurnCharacter member
	void InitSlowestTurnCharacter();
	// Computes TurnCharacter TurnsPerRound
	void InitTurnsPerRound();	
	// Initializes TurnQueue with first TurnCharacters that will take turn
	void InitTurnQueue();

	void NextTurnCharacterTakesTurn();

	// Adds next TurnCharacter in the FIFO TurnQueue. Return the added TurnCharacter
	AActor* EnQueueNextTurnCharacter();

	bool IsEndOfRound(const AActor* NextTurnCharacter) const; 

	// Update the remaining turn per round of every TurnCharacter. It should be used at the end of a round
	void UpdateRemainingTurnsPerRound();	
};
