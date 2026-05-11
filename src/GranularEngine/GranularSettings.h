//
// Created by Evan Neumann on 17/03/2026.
//
#pragma once

#include "EnvelopeType.h"

struct GranularSettings
{
    static constexpr float maxGrainDurationInSeconds{1.0f};

    bool bypass {false};

    int grainDensity{8};
    float grainDuration{0.05f};
    float playbackSpeed{0.5f};
    float pitch{1.f};
    float randomness{0.f};
    float postGain{0.5f};
    float dryWet{0.9f};

    EnvelopeType type{EnvelopeType::Gaussian};
};
