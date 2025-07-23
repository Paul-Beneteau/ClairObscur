// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ClairWorldUserWidget.generated.h"

class USizeBox;

/**
 * 
 */
UCLASS()
class CLAIROBSCUR_API UClairWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, Category = "UI", meta = (ExposeOnSpawn=true))
	AActor* AttachedActor;
		
	UPROPERTY(meta = (BindWidget))		
	USizeBox* ParentSizeBox;
	
protected:
	// Render the widget to the screen from his attached actor world location.
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
