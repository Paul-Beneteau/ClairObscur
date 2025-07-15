#include "ClairAttributeComp.h"

UClairAttributeComp::UClairAttributeComp()
{
}

// Adds the delta to Health and clamp the new Health between 0 and MaxHealth
// Returns true if the attribute has been modified.
bool UClairAttributeComp::ChangeHealth(const int32 Delta)
{
	const int32 HealthBeforeDelta { Health };

	// The new Health value must be between 0 and MaxHealth.
	Health = FMath::Clamp(Health + Delta, 0, MaxHealth);
	
	const int32 DeltaApplied { Health - HealthBeforeDelta }; 

	if (DeltaApplied == 0)
	{
		return false;
	}
	else
	{
		OnHealthChanged.Broadcast(Health, DeltaApplied);
		return true;		
	}
}

// Adds the delta to the action points count and clamp the new action points count between 0 and MaxActionPoints
// Returns true if the attribute has been modified.
bool UClairAttributeComp::ChangeActionPoints(const int32 Delta)
{
	const int32 ActionPointsBeforeDelta { ActionPoints };

	// The new action points count must be between 0 and MaxActionPoints.
	ActionPoints = FMath::Clamp(ActionPoints + Delta, 0, MaxActionPoints);
	
	const int32 DeltaApplied { ActionPoints - ActionPointsBeforeDelta }; 

	if (DeltaApplied == 0)
	{
		return false;
	}
	else
	{
		OnActionPointsChanged.Broadcast(ActionPoints, DeltaApplied);
		return true;		
	}
}

