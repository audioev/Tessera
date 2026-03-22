//
// Created by Evan Neumann on 10/03/2026.
//

#include "Scheduler.h"

Scheduler::Scheduler()
{

}

Scheduler::~Scheduler()
{

}

void Scheduler::prepare(double sampleRate, int samplesPerBlock)
{
    this->sampleRate = sampleRate;
    this->samplesPerBlock = samplesPerBlock;
}

void Scheduler::process(const GranularSettings& settings,GrainPool& grainPool,int bufferWriteHead)
{
    interOnset = static_cast<int>(sampleRate) / settings.grainDensity;
    nextOnset += samplesPerBlock;
    if (nextOnset > interOnset)
    {
        Grain* grain = grainPool.getInactiveGrain();
        if (grain != nullptr)
        {
            grain->configure(bufferWriteHead,settings.playbackRate , 1 , (settings.grainDuration * sampleRate), settings.type);
        }
    }

}


