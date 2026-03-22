//
// Created by Evan Neumann on 10/03/2026.
//

#pragma once
#include <juce_audio_basics/juce_audio_basics.h>
#include "Grain.h"
#include "GrainEnvelope.h"
#include "GrainPool.h"
#include "Scheduler.h"
#include "CircularBuffer.h"
#include "GranularSettings.h"

class Engine
{
public:
    Engine();
    ~Engine();

    void prepare(double sampleRate, int samplesPerBlock, int numChannels);
    void process(juce::AudioBuffer<float>& bufferRef,const GranularSettings& settings);

private:
    CircularBuffer circularBuffer;
    GrainPool grainPool;
    Scheduler scheduler;
    double sampleRate;
    int samplesPerBlock;
    int numChannels;
};


