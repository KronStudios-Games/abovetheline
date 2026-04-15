// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatusEffectsComponent.generated.h"

class UMaterialParameterCollection;
class UMaterialParameterCollectionInstance;
class UTextureRenderTarget2D;
class UMaterialInterface;
class UMaterialInstanceDynamic;
class UTextBlock;

/**
 * Manages player status effect severities and drives post-process visuals.
 * Updates a Material Parameter Collection each tick so PP materials react in real time.
 * Optionally draws fog accumulation to a render target for the mask fogging effect.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ABOVETHELINE_API UStatusEffectsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStatusEffectsComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// ---- Severity Properties ----

	/** Damage flash severity (0 = none, 1 = maximum). Decays automatically. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status Effects|Damage", meta=(ClampMin="0.0", ClampMax="1.0"))
	float DamageSeverity = 0.0f;

	/** Current health fraction (1 = full, 0 = dead). Drives low-health desaturation/blur. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status Effects|Health", meta=(ClampMin="0.0", ClampMax="1.0"))
	float HealthLevel = 1.0f;

	/** Gas exposure intensity (0 = none, 1 = maximum). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status Effects|Gas", meta=(ClampMin="0.0", ClampMax="1.0"))
	float GasSeverity = 0.0f;

	/** Mask fog buildup (0 = clear, 1 = fully fogged). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status Effects|Fog", meta=(ClampMin="0.0", ClampMax="1.0"))
	float FogSeverity = 0.0f;

	/** Severe poisoning intensity (0 = none, 1 = maximum). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status Effects|Poison", meta=(ClampMin="0.0", ClampMax="1.0"))
	float PoisonSeverity = 0.0f;

	/** Low oxygen intensity (0 = full O2, 1 = suffocating). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status Effects|Oxygen", meta=(ClampMin="0.0", ClampMax="1.0"))
	float OxygenSeverity = 0.0f;

	// ---- Configuration ----

	/** Material Parameter Collection that post-process materials read from. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status Effects|Config")
	TSoftObjectPtr<UMaterialParameterCollection> PlayerStateMPC;

	/** Render target for mask fog accumulation. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status Effects|Config")
	UTextureRenderTarget2D* FogRenderTarget = nullptr;

	/** Material drawn to the fog render target each tick. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status Effects|Config")
	UMaterialInterface* FogDrawMaterial = nullptr;

	/** Camera shake class for breathing / low-oxygen sway. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status Effects|Config")
	TSubclassOf<UCameraShakeBase> BreathingCameraShakeClass;

	/** Camera shake class for damage hit reactions. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status Effects|Config")
	TSubclassOf<UCameraShakeBase> DamageCameraShakeClass;

	/** Rate at which DamageSeverity auto-decays per second. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status Effects|Damage", meta=(ClampMin="0.0", ClampMax="10.0"))
	float DamageDecayRate = 2.0f;

	// ---- Debug ----

	/** When true, automatically cycles through each effect one at a time for preview. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status Effects|Debug")
	bool bDebugCycleEffects = false;

	/** Seconds each effect takes to ramp up and back down. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status Effects|Debug", meta=(ClampMin="1.0", ClampMax="20.0", EditCondition="bDebugCycleEffects"))
	float DebugCycleDuration = 4.0f;

	// ---- Setters ----

	/** Apply a damage flash. Additive, clamped to [0,1]. Triggers camera shake. */
	UFUNCTION(BlueprintCallable, Category="Status Effects")
	void ApplyDamage(float Severity);

	/** Set health fraction (1 = full, 0 = dead). */
	UFUNCTION(BlueprintCallable, Category="Status Effects")
	void SetHealthLevel(float NewHealthLevel);

	UFUNCTION(BlueprintCallable, Category="Status Effects")
	void SetGasSeverity(float NewSeverity);

	UFUNCTION(BlueprintCallable, Category="Status Effects")
	void SetFogSeverity(float NewSeverity);

	UFUNCTION(BlueprintCallable, Category="Status Effects")
	void SetPoisonSeverity(float NewSeverity);

	UFUNCTION(BlueprintCallable, Category="Status Effects")
	void SetOxygenSeverity(float NewSeverity);

private:

	UPROPERTY(Transient)
	UMaterialParameterCollectionInstance* MPCInstance = nullptr;

	UPROPERTY(Transient)
	UMaterialInstanceDynamic* FogDrawMID = nullptr;

	void UpdateMPC();
	void UpdateFogRenderTarget();
	void UpdateCameraShakes(float DeltaTime);
	void UpdateDebugCycle(float DeltaTime);

	bool bBreathingShakeActive = false;
	float DebugCycleTime = 0.0f;

	UPROPERTY(Transient)
	UTextBlock* DebugTextBlock = nullptr;

	void CreateDebugHUD();
	void UpdateDebugHUD(int32 EffectIndex, float Intensity);
};
