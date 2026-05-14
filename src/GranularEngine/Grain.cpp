//
// Created by Evan Neumann on 05/03/2026.
//



#include "Grain.h"

Grain::Grain()
{
    startSample = 0;
    currentSample = 0;
    pitch = 1.0f;
    amplitude = 0.f;
    isActive = false;
    totalSamples = 0;
    duration = 0.f;
    playbackSpeed = 0.f;
    readPosition = 0.f;
}

Grain::~Grain() = default;

void Grain::setActive(bool status)
{
    this->isActive = status;
}

bool Grain::getActive() const
{
    return this->isActive;
}

void Grain::configure( int startSample, float pitch, float amplitude,float totalSamples,EnvelopeType type)
{
   // this->duration = duration;
    currentSample = 0;
    readPosition = 0.f;
    //not bounded
    this->startSample = startSample;
    this->amplitude = amplitude;
    this->pitch = pitch;
    this->playbackSpeed = playbackSpeed;
    this->totalSamples = totalSamples;
    envelope.configure(type,totalSamples);
    setActive(true);
}

float Grain::getNextSample(const float* sampleA , const float* sampleB)
{

    float const frac = readPosition - static_cast<float>(static_cast<int>(readPosition));
    float const interp = *sampleA + frac *(*sampleB - *sampleA);
    float const phase = juce::jlimit(0.f,1.f,readPosition / totalSamples);

    float const result = interp * envelope.calculate(phase) * amplitude;

    readPosition += pitch;
    return result;
}

bool Grain::isFinished() const
{
    if (totalSamples == 0.f) return true;
    if (readPosition >= totalSamples)
    {
        return true;
    }
    return false;
}

