// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClairPlayerCharacter.h"
#include "ClairStanceCharacter.generated.h"

/**
 * 
 */
UCLASS()
class CLAIROBSCUR_API AClairStanceCharacter : public AClairPlayerCharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Tag")
	FGameplayTag VirtuoseStanceTag;
	
	UPROPERTY(EditDefaultsOnly, Category = "Tag")
	FGameplayTag OffensiveStanceTag;
	
	UPROPERTY(EditDefaultsOnly, Category = "Tag")
	FGameplayTag DefensiveStanceTag;

	virtual void TakeTurn_Implementation() override;
	
protected:
	FGameplayTagContainer PreviousTurnTags;

	virtual void EndTurn() override;
};
