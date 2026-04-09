//
// Created by Evan Neumann on 10/03/2026.
//

#include "Scheduler.h"

Scheduler::Scheduler()
    : nextOnset(0), interOnset(0), sprayedStartSamples(0), maxSprayInSamples(0)
{

}

Scheduler::~Scheduler()
{

}

void Scheduler::prepare(const double sampleRate, const int samplesPerBlock, const int overlapOffset, const int bufSize)
{
    this->sampleRate = sampleRate;
    this->samplesPerBlock = samplesPerBlock;
    minGrainDuration = static_cast<float>(samplesPerBlock) / static_cast<float>(sampleRate);
    this->overlapOffset = overlapOffset;
    this->bufSize = bufSize;
    maxSprayInSamples = bufSize/2;
}

void Scheduler::process(const GranularSettings& settings,GrainPool& grainPool,int bufferWriteHead)
{
    //interonset in samples
    interOnset = static_cast<int>(sampleRate / settings.grainDensity);
    nextOnset += samplesPerBlock;
    std::cout << "nextOnset: "<< nextOnset << " interonset: "<< interOnset << " grain desnity "<< settings.grainDensity<<std::endl;
    if (nextOnset >= interOnset)
    {
        Grain* grain = grainPool.getInactiveGrain();
        std::cout << "grain pntr: "<< grain << std::endl;
        if (grain != nullptr)
        {
            randomOffset = (random.nextFloat() * 2.0f - 1.0f) * settings.randomness * maxSprayInSamples;
            sprayedStartSamples  =((bufferWriteHead + static_cast<int>(randomOffset)) + bufSize)%bufSize;
            float clampedDuration = std::max(settings.grainDuration, minGrainDuration);
            int totalSamples = static_cast<int>(clampedDuration * sampleRate);
            std::cout << "grainDuration: " << settings.grainDuration
                      << " sampleRate: " << sampleRate
                      << " totalSamples: " << totalSamples << std::endl;

            grain->configure(sprayedStartSamples,settings.playbackRate , 1 ,
                static_cast<int>(settings.grainDuration * sampleRate), settings.type);
            nextOnset = 0;
        }
    }

}


