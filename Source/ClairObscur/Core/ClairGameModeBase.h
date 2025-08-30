// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ClairGameModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCombatStatusChanged);

UCLASS()
class CLAIROBSCUR_API AClairGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:		
	UPROPERTY(BlueprintAssignable)
	FOnCombatStatusChanged OnCombatWin;
	UPROPERTY(BlueprintAssignable)
	FOnCombatStatusChanged OnCombatLost;
	UPROPERTY(BlueprintAssignable)
	FOnCombatStatusChanged OnCombatRestart;

	virtual void StartPlay() override;

	UFUNCTION(BlueprintCallable)
	void RestartCombat() const;
	
protected:
	UPROPERTY(EditDefaultsOnly)
	FName LevelName;

	// Delay to wait for the level to be loaded before restarting turn queue
	UPROPERTY(EditDefaultsOnly)
	float OpenLevelDelay { 0.2f };
	
	// Check if the combat ended to send OnCombatWin/OnCombatLost event and pause TurnManagerSubsystem.
	UFUNCTION()
	void CheckCombatEnd();
};
