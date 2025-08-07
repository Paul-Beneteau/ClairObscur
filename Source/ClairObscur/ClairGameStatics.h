#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ClairGameStatics.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(ClairLog, Display, All);

/**
 * 
 */
UCLASS()
class CLAIROBSCUR_API UClairGameStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Log")
	static void LogOnScreen(const FString& Message);

	UFUNCTION(BlueprintCallable, Category = "Log")
	static void LogOnScreenPersistant(const FString& Message);
};
