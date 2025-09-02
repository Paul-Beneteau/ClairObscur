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
	if (GameplayTags.HasTag(VirtuoseStanceTag) && PreviousTurnGameplayTags.HasTag(VirtuoseStanceTag))
	{
		GameplayTags.RemoveTag(VirtuoseStanceTag);
	}
	if (GameplayTags.HasTag(OffensiveStanceTag) && PreviousTurnGameplayTags.HasTag(OffensiveStanceTag))
	{
		GameplayTags.RemoveTag(OffensiveStanceTag);
	}
	
	Super::EndTurn();
}
