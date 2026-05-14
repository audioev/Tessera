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

    void configure( int currentSample, float pitch, float amplitude,float totalSamples,EnvelopeType type);
    void setActive(bool status);
    [[nodiscard]] bool getActive() const;
    float getNextSample( const float* sampleA, const float* sampleB);

    [[nodiscard]] int getStartSample() const{return startSample;}
    [[nodiscard]] float getReadPosition() const {return readPosition;}
    [[nodiscard]] bool isFinished () const;

private:
    int duration;
    int startSample;
    float currentSample;
    float pitch;
    float playbackSpeed;
    float amplitude;
    bool isActive;
    float totalSamples;
    float readPosition;

    GrainEnvelope envelope;
};


