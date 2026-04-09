//
// Created by Evan Neumann on 07/03/2026.
//

#pragma once
#include "EnvelopeType.h"

class GrainEnvelope
{
    public:
    GrainEnvelope();
    ~GrainEnvelope();

    void configure(EnvelopeType env,int totalSamples);
    float calculate(float phase);

private:
    EnvelopeType type;
    int totalSamples;
    int currentSample;

    float getHannEnvelope(float samplePhase);
    float getGaussianEnvelope(float samplePhase);
    float getTrapezoidEnvelope(float samplePhase);
};


