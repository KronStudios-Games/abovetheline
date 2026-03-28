// Copyright Epic Games, Inc. All Rights Reserved.

#include "DamageCameraShake.h"

UDamageCameraShake::UDamageCameraShake()
{
	OscillationDuration = 0.25f;
	OscillationBlendInTime = 0.02f;
	OscillationBlendOutTime = 0.15f;
	bSingleInstance = false; // Allow overlapping hits

	// Quick aggressive rotation
	RotOscillation.Pitch.Amplitude = 3.0f;
	RotOscillation.Pitch.Frequency = 25.0f;
	RotOscillation.Pitch.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom;
	RotOscillation.Pitch.Waveform = EOscillatorWaveform::SineWave;

	RotOscillation.Yaw.Amplitude = 2.0f;
	RotOscillation.Yaw.Frequency = 20.0f;
	RotOscillation.Yaw.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom;
	RotOscillation.Yaw.Waveform = EOscillatorWaveform::SineWave;

	RotOscillation.Roll.Amplitude = 1.5f;
	RotOscillation.Roll.Frequency = 18.0f;
	RotOscillation.Roll.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom;
	RotOscillation.Roll.Waveform = EOscillatorWaveform::SineWave;

	// Sharp positional jolt
	LocOscillation.X.Amplitude = 0.5f;
	LocOscillation.X.Frequency = 30.0f;
	LocOscillation.X.Waveform = EOscillatorWaveform::SineWave;

	LocOscillation.Z.Amplitude = 1.0f;
	LocOscillation.Z.Frequency = 25.0f;
	LocOscillation.Z.Waveform = EOscillatorWaveform::SineWave;
}
