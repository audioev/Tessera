//
// Created by Evan Neumann on 10/03/2026.
//

#pragma once
#include "GrainPool.h"
#include "GranularSettings.h"

class Scheduler
{
public:
    Scheduler();
    ~Scheduler();

    void prepare(double sampleRate, int samplesPerBlock, int overlapOffset,int bufSize);
    void process(const GranularSettings& settings,GrainPool& grainPool,int bufferWriteHead);
    //float synthesize();

    private:
    int overlapOffset ;
    GrainPool grainPool;
    double sampleRate;
    int samplesPerBlock;
    int interOnset;
    int nextOnset;
    int bufferWriteHead;
    float randomOffset;
    juce::Random random;
    float maxSprayInSamples;
    int sprayedStartSamples;
    int bufSize;
    float minGrainDuration;
};


