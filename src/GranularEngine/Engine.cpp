//
// Created by Evan Neumann on 10/03/2026.
// v1.2
//

#include "Engine.h"

Engine::Engine()
{
    sampleRate = 0.0;
    samplesPerBlock = 0;
    numChannels = 0;
    previousDuration = 0.0;
}

Engine::~Engine() = default;

void Engine::prepare(const double sampleRate, const int samplesPerBlock,const int numChannels)
{
    this->sampleRate = sampleRate;
    this->samplesPerBlock = samplesPerBlock;
    this->numChannels = numChannels;
    auto bufSize = sampleRate * GranularSettings::maxGrainDurationInSeconds;

    circularBuffer.prepare(numChannels,static_cast<int>(bufSize));
    grainPool.prepare();
    scheduler.prepare(sampleRate, samplesPerBlock,1,bufSize);
}


void Engine::process(juce::AudioBuffer<float>& bufferRef, GranularSettings& settings)
{
    for (auto& grain : grainPool)

    circularBuffer.write(bufferRef);

    scheduler.process(settings,grainPool ,circularBuffer.getWriteHead());

    if (settings.grainDuration != previousDuration)
    {
        grainPool.returnAllGrains();
        previousDuration = settings.grainDuration;
    }
    bufferRef.clear();

    for (auto& grain : grainPool)
    {
        if (!grain.getActive()) continue;

        for (auto samples = 0 ; samples < samplesPerBlock; samples++)
        {
            if (grain.isFinished())break;

            int const indexA = static_cast<int>(grain.getReadPosition());
            int const indexB = indexA + 1;

            const float* sampleA = circularBuffer.read(0,grain.getStartSample(),indexA);
            const float* sampleB = circularBuffer.read(0,grain.getStartSample(),indexB);

            const float grainNxtSample = grain.getNextSample(sampleA, sampleB);

            for (auto channels=0 ; channels < numChannels ; channels++)
            {
                bufferRef.addSample(channels,samples,grainNxtSample);
            }
            if (grain.isFinished())
            {
                grainPool.returnGrain(&grain);
            }
        }
    }
    //this is a limiting module that may not be needed, if your instance generates huge volume explosions, uncomment this

    // for ( int ch = 0; ch< bufferRef.getNumChannels(); ch++ )
    // {
    //     auto* data = bufferRef.getWritePointer(ch);
    //     for (int s = 0; s < bufferRef.getNumSamples(); s++)
    //     {
    //         data[s] = juce::jlimit(-1.0f,1.0f,data[s]);
    //     }
    // }
}