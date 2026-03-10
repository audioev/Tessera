//
// Created by Evan Neumann on 07/03/2026.
//

#pragma once
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_dsp/juce_dsp.h>
#include "EnvelopeType.h"

class GrainEnvelope
{
    public:
    GrainEnvelope();
    ~GrainEnvelope();

    void configure(EnvelopeType env,int grainSamples);

    EnvelopeType type;
    int totalSamples;
    int currentSample;

    float getHannEnvelope(float sample);
    float getGaussianEnvelope(float sample);
    float getTrapezoidEnvelope(float sample);
private:


};


