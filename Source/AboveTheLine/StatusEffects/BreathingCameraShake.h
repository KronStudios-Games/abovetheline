// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LegacyCameraShake.h"
#include "BreathingCameraShake.generated.h"

/**
 * Rhythmic slow oscillation simulating labored breathing.
 * Used for low oxygen and exhaustion effects.
 */
UCLASS()
class ABOVETHELINE_API UBreathingCameraShake : public ULegacyCameraShake
{
	GENERATED_BODY()

public:
	UBreathingCameraShake();
};
