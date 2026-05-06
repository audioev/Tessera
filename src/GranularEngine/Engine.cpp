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
}

Engine::~Engine()
{
}


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
    //we write the current samples inside the inout buffer to the circular buffer
    //samplesperblock is the number of samples
    circularBuffer.write(bufferRef);
    //----------------------------------------------------------------------
    //std::cout << "grain density" << settings.grainDensity << std::endl;
    //----------------------------------------------------------------------
    scheduler.process(settings,grainPool ,circularBuffer.getWriteHead());
    std::cout << "pitch: " << settings.pitch << std::endl;
    std::cout << "playback speed: " << settings.playbackSpeed << std::endl;
    //dry/wet mix control needs to be blended here
    bufferRef.clear();
    int activeGrains = 0;
    //outer loop:
    //traverse each grain in the the pool(256)
    for (auto& grain : grainPool)
    {
        //VEY IMPORTANT
        //is the current grain is active
        //continue the through to the next grain.
        //if this is removed, we process possibly currently playing grains -> not good
        if (!grain.getActive()) continue;
        activeGrains++;
        //inner loop 1
        //traverse each sample of type float for the size of the sampleBlock
        for (auto samples = 0 ; samples < samplesPerBlock; samples++)
        {
            //returns a pntr to the current input sample that is begin processed in our current grain

            const float* readPntr = circularBuffer.read(0,grain.getStartSample(),grain.getCurrentSample());
            //get the float value of the currently processed sample witht he applied envelope
            /*
             * this may be inccorect and need to be altered. why?
             * if we have a block of samples, say the block that exists within point x and y in the buffer
             * this is a "grain"
             * here we are applying the enevlope to each individual sample in the grain
             * we may want to apply the envelope as a window function to the buffer within points x and y
             * perhaps
             */
            //mono sample write, for stereo must be altered
            float grainNxtSample = grain.getNextSample(readPntr);
            //----------------------------------------------------------------------
            //std::cout << "in channel loop"<< std::endl;
            //----------------------------------------------------------------------
            //inner loop 2
            //basically flip flops between both channels per sample
            for (auto channels=0 ; channels < numChannels ; channels++)
            {
                //----------------------------------------------------------------------
                //std::cout << "in sample loop"<< std::endl;
                //std::cout << grainNxtSample << " " << std::endl ;
                //----------------------------------------------------------------------
                //call addsample to add the new current grain sample to the audio thread buffer
                //basically replace the old sample with the new
                bufferRef.addSample(channels,samples,grainNxtSample);
            }
        }
        //if the grain is finished
        if (grain.isFinished())
        {
            //return it back to the pool as an inactive grain to be reused
            grainPool.returnGrain(&grain);
        }
    }
}