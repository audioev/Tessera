//
// Created by Evan Neumann on 17/03/2026.
//
#pragma once

#include "EnvelopeType.h"

struct GranularSettings
{
    static constexpr float maxGrainDurationInSeconds{1.0f};

    int grainDensity{8};
    float grainDuration{0.05f};
    float playbackRate{1.f};

    float grainAttack{0.1f};
    float grainDecay{0.1f};
    float grainSustain{0.1f};

    float globalAttack{0.01f};
    float globalDecay{0.01f};
    float globalSustain{0.1f};
    float globalRelease{0.1f};

    EnvelopeType type{EnvelopeType::Gaussian};
};