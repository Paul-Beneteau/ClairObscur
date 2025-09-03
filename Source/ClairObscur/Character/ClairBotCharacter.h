#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ClairCharacter.h"
#include "ClairPlayerCharacter.h"
#include "ClairBotCharacter.generated.h"

UCLASS()
class CLAIROBSCUR_API AClairBotCharacter : public AClairCharacter
{
	GENERATED_BODY()

public:
	virtual void TakeTurn_Implementation() override;

protected:
	// Attack used to hit player
	UPROPERTY(EditDefaultsOnly, Category="Attack")
	EClairAbilityKey DefaultAttack { EClairAbilityKey::PrimaryAttack };

	virtual void OnAbilityEndedHandler(UGameplayAbility* GameplayAbility) override;
};
