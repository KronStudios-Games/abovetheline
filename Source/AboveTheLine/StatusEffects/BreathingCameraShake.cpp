// Copyright Epic Games, Inc. All Rights Reserved.

#include "BreathingCameraShake.h"

UBreathingCameraShake::UBreathingCameraShake()
{
	OscillationDuration = 0.0f; // Infinite (loops until stopped)
	OscillationBlendInTime = 1.0f;
	OscillationBlendOutTime = 1.0f;
	bSingleInstance = true;

	// Slow rhythmic pitch — breathing up/down
	RotOscillation.Pitch.Amplitude = 0.5f;
	RotOscillation.Pitch.Frequency = 0.4f;
	RotOscillation.Pitch.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom;
	RotOscillation.Pitch.Waveform = EOscillatorWaveform::SineWave;

	// Very slight yaw sway
	RotOscillation.Yaw.Amplitude = 0.2f;
	RotOscillation.Yaw.Frequency = 0.25f;
	RotOscillation.Yaw.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom;
	RotOscillation.Yaw.Waveform = EOscillatorWaveform::SineWave;

	// Subtle roll for disorientation
	RotOscillation.Roll.Amplitude = 0.15f;
	RotOscillation.Roll.Frequency = 0.35f;
	RotOscillation.Roll.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom;
	RotOscillation.Roll.Waveform = EOscillatorWaveform::SineWave;

	// Slight vertical bob from breathing
	LocOscillation.Z.Amplitude = 0.3f;
	LocOscillation.Z.Frequency = 0.4f;
	LocOscillation.Z.Waveform = EOscillatorWaveform::SineWave;
}
