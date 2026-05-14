//
// Created by Evan Neumann on 10/03/2026.
//

#include "Scheduler.h"

Scheduler::Scheduler()
    : nextOnset(0), interOnset(0), sprayedStartSamples(0), maxSprayInSamples(0)
{
    playbackOffset = 0.f;
    overlapOffset = 0.f;
    sampleRate = 0.f;
    samplesPerBlock = 0.f;
    bufferWriteHead = 0;
    randomOffset = 0.f;
    overlapOffset = 0.f;
    bufSize = 0;
    minGrainDuration = 0.f;
}

Scheduler::~Scheduler() = default;

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

    while (nextOnset >= interOnset)
    {
        Grain* grain = grainPool.getInactiveGrain();
        if (grain != nullptr)
        {
            int grainDurInSamples = static_cast<int>((settings.grainDuration/1000.f) * sampleRate);

            randomOffset = (random.nextFloat() * 2.0f - 1.0f) * settings.randomness * maxSprayInSamples;

            sprayedStartSamples  =((bufferWriteHead - grainDurInSamples + static_cast<int>(randomOffset)-static_cast<int>(playbackOffset))+ bufSize)%bufSize;

            playbackOffset += static_cast<float>(grainDurInSamples) * settings.playbackSpeed - 1.0f;
            playbackOffset = std::fmod(playbackOffset,static_cast<float>(bufSize));

            grain->configure(sprayedStartSamples,
                            settings.pitch ,
                            1 ,
                            grainDurInSamples,
                            settings.type);
        }
        nextOnset -= interOnset;
    }
}


