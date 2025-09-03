// Fill out your copyright notice in the Description page of Project Settings.


#include "ClairStanceCharacter.h"

void AClairStanceCharacter::EndTurn()
{
	// Stances last for 1 turn.
	if (GameplayTags.HasTag(DefensiveStanceTag) && PreviousTurnGameplayTags.HasTag(DefensiveStanceTag))
	{
		GameplayTags.RemoveTag(DefensiveStanceTag);
	}
	
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
