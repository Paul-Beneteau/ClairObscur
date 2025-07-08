#include "ClairAttributeComp.h"

UClairAttributeComp::UClairAttributeComp()
{
}

// Apply the delta to Health and clamp the new Health between 0 and MaxHealth
// Returns true if the attribute has been modified.
bool UClairAttributeComp::ApplyHealthDelta(const int32 Delta)
{
	const int32 HealthBeforeDelta { Health };

	// The new Health must be between 0 and MaxHealth.
	Health = FMath::Clamp(Health + Delta, 0, MaxHealth);
	
	const int32 DeltaApplied { Health - HealthBeforeDelta }; 

	OnHealthChanged.Broadcast(Health, DeltaApplied);
	
	return DeltaApplied != 0;
}

