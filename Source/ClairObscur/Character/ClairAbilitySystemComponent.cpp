// Fill out your copyright notice in the Description page of Project Settings.


#include "ClairAbilitySystemComponent.h"

void UClairAbilitySystemComponent::Initialize(AActor* InOwnerActor, AActor* InAvatarActor)
{
	InitAbilityActorInfo(InOwnerActor, InAvatarActor);
}
