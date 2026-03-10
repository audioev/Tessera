//
// Created by Evan Neumann on 05/03/2026.
//

#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
// #include "juce_audio_processors_headless/processors/juce_AudioProcessor.h"
#include  "GrainEnvelope.h"


class Grain
{
public:
    Grain();
    ~Grain();

    void configure(int duration, int startSample, int endSample , float pitch, float amplitude, GrainEnvelope envelope);
    void getNextSample(float *sample);
    bool  isFinished();
    void reset();

private:
    int duration;
    int startSample;
    int currentSample;
    float pitch;
    float amplitude;
    bool isActive;

    GrainEnvelope envelope;
};


