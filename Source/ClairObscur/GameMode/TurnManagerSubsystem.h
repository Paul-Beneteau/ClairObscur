// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/SpscQueue.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TurnManagerSubsystem.generated.h"

// Used to store an actor with TurnCharacterInterface and his number of turns per round
USTRUCT()
struct FTurnCharacter
{
	GENERATED_BODY()

	UPROPERTY()
	AActor* Actor { nullptr };

	// Number of turns in a round. A round consists of a series of turns where all characters takes at least one turn
	// e.g. character 1 has 3 TurnsPerRound and character 2 has 1 TurnsPerRound. character 1 will take 3 turns during
	// the round and character 2 will take 1 Turn
	float TurnsPerRound { 0.0f };

	// Number of remaining turns in the round. it decreases by one each time a character takes a turn.
	float RemainingTurnsPerRound { 0.0f };
};

// Manages actor that implements TurnCharacterInterface in the world and makes them take turn in an order managed by the
// TurnManager
UCLASS()
class CLAIROBSCUR_API UTurnManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	// Initializes class members then characters managed by the TurnManager takes their turn in the order computed
	// from their speed.
	void Start();
	
protected:
	// Contains every actor that implements TurnCharacterInterface in the world 
	UPROPERTY()
	TArray<FTurnCharacter> Characters;

	// FIFO that Contains next TurnCharacters that will take turn
	TSpscQueue<AActor*> TurnQueue;

	int32 TurnQueueSize { 6 };

	// Saves the slowest character used to compute the number of turn per round of each character
	UPROPERTY()
	AActor* SlowestCharacter { nullptr };

	// Next character in queue takes his turn
	UFUNCTION()
	void StartNextTurn();
	
	// Initializes Characters class member with actors in the world that implements TurnCharacterInterface
	void InitCharacters();	
	// Saves slowest character in SlowestCharacter class member
	void InitSlowestCharacter();
	// Computes every character TurnsPerRound
	void InitTurnsPerRound();	
	// Initializes TurnQueue with first characters that will take turn
	void InitTurnQueue();

	AActor* DeQueueCharacter();	
	// Queue next character in the FIFO TurnQueue. Return the queued character
	AActor* EnQueueCharacter();

	// Is this the end of a round after the character takes his turn
	bool IsEndOfRound(const AActor* Character) const; 

	// Update the remaining turn per round of every character. It should be used at the end of a round
	void UpdateRemainingTurnsPerRound();
};
