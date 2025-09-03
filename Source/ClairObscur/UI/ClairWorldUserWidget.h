#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ClairWorldUserWidget.generated.h"

class USizeBox;

// Render widget in the world instead of the screen
UCLASS()
class CLAIROBSCUR_API UClairWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, Category = "UI", meta = (ExposeOnSpawn=true))
	TObjectPtr<AActor> AttachedActor;
		
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))		
	TObjectPtr<USizeBox> ParentSizeBox;
	
protected:
	// Render the widget to the screen from his attached actor world location.
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
