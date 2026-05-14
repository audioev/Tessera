//
// Created by Evan Neumann on 07/03/2026.
//

#include "GrainEnvelope.h"

#include <iostream>
#include <ostream>

GrainEnvelope::GrainEnvelope()
{
    type = static_cast<EnvelopeType>(0);
    totalSamples = 0;
    currentSample = 0;
}

GrainEnvelope::~GrainEnvelope() = default;

void GrainEnvelope::configure(EnvelopeType type,float totalSamples)
{
    this->type= type;
    this->totalSamples = totalSamples;
}

float GrainEnvelope::calculate(float phase)
{

    switch (type)
    {
        case EnvelopeType::Hann: return getHannEnvelope(phase);
        case EnvelopeType::Gaussian: return getGaussianEnvelope(phase);
        case EnvelopeType::Trapezoid: return getTrapezoidEnvelope(phase);
    }
}

float GrainEnvelope::getHannEnvelope(float samplePhase)
{
    float HannReturnAmplitude;

    HannReturnAmplitude = 0.5f * (1.0f - cos(2.0f * M_PI * samplePhase));
    return HannReturnAmplitude;
}

float GrainEnvelope::getGaussianEnvelope(float samplePhase)
{
    float GaussianReturnAmplitude;
    float gWindowWidth = 0.35f;

    GaussianReturnAmplitude = std::exp(-0.5f * std::pow(samplePhase - 0.5f/ gWindowWidth,2.0f));
    return GaussianReturnAmplitude;
}

float GrainEnvelope::getTrapezoidEnvelope(float samplePhase)
{
    float TrapezoidReturnAmplitude;
    float flat = 0.45f;
    float taper = (1.0f - flat) /2.0f;

    if (samplePhase < flat)
    {
        TrapezoidReturnAmplitude = samplePhase / taper;
    }else if ( samplePhase > 1.0f -taper)
    {
        TrapezoidReturnAmplitude = (1.0f -samplePhase) /taper;
    }else
    {
        TrapezoidReturnAmplitude = 1.0f;
    }

    return TrapezoidReturnAmplitude;
}