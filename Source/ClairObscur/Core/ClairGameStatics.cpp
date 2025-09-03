#include "ClairGameStatics.h"

DEFINE_LOG_CATEGORY(ClairLog);

void UClairGameStatics::LogOnScreen(const FString& Message)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, Message);
}

void UClairGameStatics::LogOnScreenPersistant(const FString& Message)
{
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green, Message);
}
