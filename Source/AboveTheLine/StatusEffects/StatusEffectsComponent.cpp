// Copyright Epic Games, Inc. All Rights Reserved.

#include "StatusEffectsComponent.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetTree.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTP_FirstPerson, Log, All);

UStatusEffectsComponent::UStatusEffectsComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Default MPC asset reference
	PlayerStateMPC = TSoftObjectPtr<UMaterialParameterCollection>(
		FSoftObjectPath(TEXT("/Game/PostProcess/MPC_PlayerState.MPC_PlayerState")));
}

void UStatusEffectsComponent::BeginPlay()
{
	Super::BeginPlay();

	// Resolve MPC and get the world parameter collection instance
	if (UMaterialParameterCollection* MPC = PlayerStateMPC.LoadSynchronous())
	{
		MPCInstance = GetWorld()->GetParameterCollectionInstance(MPC);
	}

	// Create dynamic material instance for fog drawing
	if (FogDrawMaterial)
	{
		FogDrawMID = UMaterialInstanceDynamic::Create(FogDrawMaterial, this);
	}

	// Push initial values
	UpdateMPC();

	// Create debug HUD if cycle mode is on
	if (bDebugCycleEffects)
	{
		CreateDebugHUD();
	}
}

void UStatusEffectsComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Debug cycle overrides all severity values
	if (bDebugCycleEffects)
	{
		UpdateDebugCycle(DeltaTime);
	}
	else
	{
		// Auto-decay damage severity
		if (DamageSeverity > 0.0f)
		{
			DamageSeverity = FMath::Max(0.0f, DamageSeverity - DamageDecayRate * DeltaTime);
		}
	}

	UpdateMPC();
	UpdateFogRenderTarget();
	UpdateCameraShakes(DeltaTime);
}

void UStatusEffectsComponent::UpdateMPC()
{
	if (!MPCInstance)
	{
		return;
	}

	MPCInstance->SetScalarParameterValue(FName("DamageSeverity"), DamageSeverity);
	MPCInstance->SetScalarParameterValue(FName("HealthLevel"), HealthLevel);
	MPCInstance->SetScalarParameterValue(FName("GasSeverity"), GasSeverity);
	MPCInstance->SetScalarParameterValue(FName("FogSeverity"), FogSeverity);
	MPCInstance->SetScalarParameterValue(FName("PoisonSeverity"), PoisonSeverity);
	MPCInstance->SetScalarParameterValue(FName("OxygenSeverity"), OxygenSeverity);
}

void UStatusEffectsComponent::UpdateFogRenderTarget()
{
	if (!FogRenderTarget || !FogDrawMID || FogSeverity <= 0.0f)
	{
		return;
	}

	FogDrawMID->SetScalarParameterValue(FName("FogSeverity"), FogSeverity);
	UKismetRenderingLibrary::DrawMaterialToRenderTarget(this, FogRenderTarget, FogDrawMID);
}

void UStatusEffectsComponent::UpdateCameraShakes(float DeltaTime)
{
	ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());
	if (!OwnerChar)
	{
		return;
	}

	APlayerController* PC = Cast<APlayerController>(OwnerChar->GetController());
	if (!PC || !PC->PlayerCameraManager)
	{
		return;
	}

	// Breathing shake: active when OxygenSeverity exceeds threshold
	if (OxygenSeverity > 0.3f && BreathingCameraShakeClass)
	{
		if (!bBreathingShakeActive)
		{
			PC->PlayerCameraManager->StartCameraShake(BreathingCameraShakeClass, OxygenSeverity);
			bBreathingShakeActive = true;
		}
	}
	else if (bBreathingShakeActive)
	{
		PC->PlayerCameraManager->StopAllInstancesOfCameraShake(BreathingCameraShakeClass);
		bBreathingShakeActive = false;
	}
}

void UStatusEffectsComponent::UpdateDebugCycle(float DeltaTime)
{
	DebugCycleTime += DeltaTime;

	// 6 effects, each gets DebugCycleDuration seconds (triangle wave: ramp up then down)
	const float TotalCycle = DebugCycleDuration * 6.0f;
	const float LoopTime = FMath::Fmod(DebugCycleTime, TotalCycle);
	const int32 CurrentEffect = FMath::FloorToInt(LoopTime / DebugCycleDuration);
	const float PhaseTime = FMath::Fmod(LoopTime, DebugCycleDuration);

	// Triangle wave: ramp 0→1 in first half, 1→0 in second half
	const float HalfDur = DebugCycleDuration * 0.5f;
	const float Intensity = (PhaseTime < HalfDur)
		? PhaseTime / HalfDur
		: 1.0f - (PhaseTime - HalfDur) / HalfDur;

	// Reset all
	DamageSeverity = 0.0f;
	HealthLevel = 1.0f;
	GasSeverity = 0.0f;
	FogSeverity = 0.0f;
	PoisonSeverity = 0.0f;
	OxygenSeverity = 0.0f;

	// Effect names for debug display
	static const TCHAR* EffectNames[] = {
		TEXT("Damage"),
		TEXT("Low Health"),
		TEXT("Gas Exposure"),
		TEXT("Mask Fogging"),
		TEXT("Severe Poisoning"),
		TEXT("Low Oxygen")
	};

	// Set the active one
	switch (CurrentEffect)
	{
	case 0: DamageSeverity = Intensity; break;
	case 1: HealthLevel = 1.0f - Intensity; break; // Low health = inverted
	case 2: GasSeverity = Intensity; break;
	case 3: FogSeverity = Intensity; break;
	case 4: PoisonSeverity = Intensity; break;
	case 5: OxygenSeverity = Intensity; break;
	}

	UpdateDebugHUD(CurrentEffect, Intensity);
}

void UStatusEffectsComponent::ApplyDamage(float Severity)
{
	DamageSeverity = FMath::Clamp(DamageSeverity + Severity, 0.0f, 1.0f);

	// Trigger damage camera shake
	if (DamageCameraShakeClass)
	{
		if (ACharacter* OwnerChar = Cast<ACharacter>(GetOwner()))
		{
			if (APlayerController* PC = Cast<APlayerController>(OwnerChar->GetController()))
			{
				if (PC->PlayerCameraManager)
				{
					PC->PlayerCameraManager->StartCameraShake(DamageCameraShakeClass, Severity);
				}
			}
		}
	}
}

void UStatusEffectsComponent::SetHealthLevel(float NewHealthLevel)
{
	HealthLevel = FMath::Clamp(NewHealthLevel, 0.0f, 1.0f);
}

void UStatusEffectsComponent::SetGasSeverity(float NewSeverity)
{
	GasSeverity = FMath::Clamp(NewSeverity, 0.0f, 1.0f);
}

void UStatusEffectsComponent::SetFogSeverity(float NewSeverity)
{
	FogSeverity = FMath::Clamp(NewSeverity, 0.0f, 1.0f);
}

void UStatusEffectsComponent::SetPoisonSeverity(float NewSeverity)
{
	PoisonSeverity = FMath::Clamp(NewSeverity, 0.0f, 1.0f);
}

void UStatusEffectsComponent::SetOxygenSeverity(float NewSeverity)
{
	OxygenSeverity = FMath::Clamp(NewSeverity, 0.0f, 1.0f);
}

void UStatusEffectsComponent::CreateDebugHUD()
{
	ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());
	if (!OwnerChar) return;

	APlayerController* PC = Cast<APlayerController>(OwnerChar->GetController());
	if (!PC) return;

	UUserWidget* Widget = CreateWidget<UUserWidget>(PC);
	if (!Widget) return;

	// Build widget tree: CanvasPanel > TextBlock
	UCanvasPanel* Canvas = NewObject<UCanvasPanel>(Widget);
	Widget->WidgetTree->RootWidget = Canvas;

	DebugTextBlock = Widget->WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("DebugText"));
	Canvas->AddChild(DebugTextBlock);

	// Position top-center
	if (UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(DebugTextBlock->Slot))
	{
		Slot->SetAnchors(FAnchors(0.5f, 0.0f, 0.5f, 0.0f));
		Slot->SetAlignment(FVector2D(0.5f, 0.0f));
		Slot->SetPosition(FVector2D(0.0f, 30.0f));
		Slot->SetAutoSize(true);
	}

	// Style
	FSlateFontInfo Font = DebugTextBlock->GetFont();
	Font.Size = 24;
	DebugTextBlock->SetFont(Font);
	DebugTextBlock->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	DebugTextBlock->SetShadowOffset(FVector2D(2.0f, 2.0f));
	DebugTextBlock->SetShadowColorAndOpacity(FLinearColor(0.0f, 0.0f, 0.0f, 0.8f));

	Widget->AddToViewport(100);
}

void UStatusEffectsComponent::UpdateDebugHUD(int32 EffectIndex, float Intensity)
{
	if (!DebugTextBlock) return;

	static const TCHAR* EffectNames[] = {
		TEXT("Damage"),
		TEXT("Low Health"),
		TEXT("Gas Exposure"),
		TEXT("Mask Fogging"),
		TEXT("Severe Poisoning"),
		TEXT("Low Oxygen")
	};

	const FString Msg = FString::Printf(TEXT("[%d/6]  %s  —  %.0f%%"),
		EffectIndex + 1, EffectNames[EffectIndex], Intensity * 100.0f);
	DebugTextBlock->SetText(FText::FromString(Msg));
}
