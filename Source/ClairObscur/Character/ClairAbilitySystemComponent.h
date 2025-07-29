// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ClairAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class CLAIROBSCUR_API UClairAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void Initialize(AActor* InOwnerActor, AActor* InAvatarActor);
};
