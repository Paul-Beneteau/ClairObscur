// Fill out your copyright notice in the Description page of Project Settings.


#include "ClairStanceCharacter.h"

void AClairStanceCharacter::TakeTurn_Implementation()
{
	Super::TakeTurn_Implementation();

	GameplayTags.RemoveTag(DefensiveStanceTag);
}

// Check if a stance tag should be removed
void AClairStanceCharacter::EndTurn()
{
	// If the stance lasted for a turn, remove it.	
	if (GameplayTags.HasTag(VirtuoseStanceTag) && PreviousTurnTags.HasTag(VirtuoseStanceTag))
	{
		GameplayTags.RemoveTag(VirtuoseStanceTag);
	}
	if (GameplayTags.HasTag(OffensiveStanceTag) && PreviousTurnTags.HasTag(OffensiveStanceTag))
	{
		GameplayTags.RemoveTag(OffensiveStanceTag);
	}

	// Save current tag in previous turn tags for next turn
	PreviousTurnTags.Reset();
	
	for (FGameplayTag Tag : GameplayTags)
	{
		PreviousTurnTags.AddTag(Tag);
	}
	
	Super::EndTurn();
}
