//
// Created by Evan Neumann on 05/03/2026.
//

#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include  "GrainEnvelope.h"


class Grain
{
public:
    Grain();
    ~Grain();

    void configure( int currentSample, float pitch, float amplitude,int totalSamples ,float playbackSpeed,EnvelopeType type);
    void setActive(bool status);
    bool getActive();
    float getNextSample( const float *sample);
    int getCurrentSample(){return currentSample;}
    int getStartSample(){return startSample;}
    int getTotalSamples(){return totalSamples;}
    bool isFinished();

private:
    int duration;
    int startSample;
    float currentSample;
    float pitch;
    float playbackSpeed;
    float amplitude;
    bool isActive;
    float totalSamples;

    GrainEnvelope envelope;
};


