//
// Created by Evan Neumann on 10/03/2026.
//

#include "Engine.h"

Engine::Engine()
{
    sampleRate = 0.0;
    samplesPerBlock = 0;
    numChannels = 0;
}

Engine::~Engine(){}


void Engine::prepare(double sampleRate, int samplesPerBlock, int numChannels)
{
    this->sampleRate = sampleRate;
    this->samplesPerBlock = samplesPerBlock;
    this->numChannels = numChannels;
    auto bufSize = sampleRate * GranularSettings::maxGrainDurationInSeconds;

    circularBuffer.prepare(numChannels,static_cast<int>(bufSize));
    grainPool.prepare();
    scheduler.prepare(sampleRate, samplesPerBlock);
}

void Engine::process(juce::AudioBuffer<float>& bufferRef, const GranularSettings& settings)
{
    circularBuffer.write(bufferRef);

    scheduler.process(settings,grainPool ,circularBuffer.getWriteHead());

    for (auto& grain : grainPool)
    {
        if (!grain.getActive()) continue;
        for (auto samples = 0 ; samples < samplesPerBlock; samples++)
        {
            const float* readPntr = circularBuffer.read(0,grain.getStartSample(),grain.getCurrentSample());
            std::cout << "startsample"<<grain.getStartSample() << std::endl;
            std::cout << "currentsample"<<grain.getCurrentSample() << std::endl;
            std::cout << "totalSmaples"<<grain.getTotalSamples() << std::endl;
            float grainNxtSample = grain.getNextSample(readPntr);
            //std::cout << "in channel loop"<< std::endl;
            for (auto channels=0 ; channels < numChannels ; channels++)
            {
                //std::cout << "in sample loop"<< std::endl;

                //std::cout << grainNxtSample << " " << std::endl ;
                if (samples < bufferRef.getNumSamples()-1)
                {
                    bufferRef.addSample(channels,samples,grainNxtSample);
                }
            }
        }
        if (grain.isFinished())
        {
            grainPool.returnGrain(&grain);
            continue;
        }
    }
}