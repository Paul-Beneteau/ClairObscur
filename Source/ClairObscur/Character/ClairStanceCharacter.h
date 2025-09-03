#pragma once

#include "CoreMinimal.h"
#include "ClairPlayerCharacter.h"
#include "ClairStanceCharacter.generated.h"

// Player character implementing combat stances (Defensive/Offensive/Virtuose stances) given by abilities
UCLASS()
class CLAIROBSCUR_API AClairStanceCharacter : public AClairPlayerCharacter
{
	GENERATED_BODY()

public:
	// Character stances
	UPROPERTY(EditDefaultsOnly, Category = "Tag")
	FGameplayTag VirtuoseStanceTag;	
	UPROPERTY(EditDefaultsOnly, Category = "Tag")
	FGameplayTag OffensiveStanceTag;	
	UPROPERTY(EditDefaultsOnly, Category = "Tag")
	FGameplayTag DefensiveStanceTag;
	
protected:

	virtual void EndTurn() override;
};
