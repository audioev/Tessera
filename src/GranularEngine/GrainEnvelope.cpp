//
// Created by Evan Neumann on 07/03/2026.
//

#include "GrainEnvelope.h"

#include <iostream>
#include <ostream>

GrainEnvelope::GrainEnvelope()
{

}

GrainEnvelope::~GrainEnvelope(){}

void GrainEnvelope::configure(EnvelopeType type,int totalSamples)
{
    this->type= type;
    this->totalSamples;
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
    float gWindowWidth = 0.15f;

    GaussianReturnAmplitude = std::exp(-0.5f * (std::exp(samplePhase - 0.5f)/ gWindowWidth));
    return GaussianReturnAmplitude;
}

float GrainEnvelope::getTrapezoidEnvelope(float samplePhase)
{
    float TrapezoidReturnAmplitude;

    return samplePhase;
}