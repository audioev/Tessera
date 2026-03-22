//
// Created by Evan Neumann on 10/03/2026.
//

#pragma once
#include "Grain.h"
#include "GrainPool.h"
#include "GranularSettings.h"

class Scheduler
{
public:
    Scheduler();
    ~Scheduler();

    void prepare(double sampleRate, int samplesPerBlock);
    void process(const GranularSettings& settings,GrainPool& grainPool,int bufferWriteHead);
    float synthesize();

    private:
    GrainPool grainPool;
    double sampleRate;
    int samplesPerBlock;
    int interOnset;
    int nextOnset;
    int bufferWriteHead;
};


