// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LegacyCameraShake.h"
#include "DamageCameraShake.generated.h"

/**
 * Sharp, quick camera shake for damage hit reactions.
 */
UCLASS()
class ABOVETHELINE_API UDamageCameraShake : public ULegacyCameraShake
{
	GENERATED_BODY()

public:
	UDamageCameraShake();
};
