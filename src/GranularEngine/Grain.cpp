//
// Created by Evan Neumann on 05/03/2026.
//



#include "Grain.h"

Grain::Grain()
{
    startSample = 0;
    currentSample = 0;
    pitch = 0;
    amplitude = 0.f;
    isActive = false;
    totalSamples = 0;
}

Grain::~Grain() = default;

void Grain::setActive(bool status)
{
    this->isActive = status;
}

bool Grain::getActive()
{
    return this->isActive;
}

void Grain::configure( int startSample, float pitch, float amplitude,int totalSamples ,float playbackSpeed,EnvelopeType type)
{
   // this->duration = duration;
    currentSample = 0;
    //not bounded
    this->startSample = startSample;
    this->amplitude = amplitude;
    this->pitch = pitch;
    this->playbackSpeed = playbackSpeed;
    this->totalSamples = totalSamples;
    //std::cout <<  "grain configured with total samples "<<totalSamples << std::endl;
    envelope.configure(type,totalSamples);
    setActive(true);
}

float Grain::getNextSample(const float* sample)
{
    float const phase = currentSample / static_cast<float>(totalSamples);
    float rawSample = *sample;
    // std::cout<< "rawSample"<< rawSample<<std::endl;
    rawSample = rawSample * envelope.calculate(phase) * amplitude;
    currentSample += pitch;
    return rawSample;
}

bool Grain::isFinished()
{
    // std::cout << "currentSample: " << currentSample
    //       << " totalSamples: " << totalSamples << std::endl;
    if (totalSamples == 0.f) return true;
    if (currentSample >= totalSamples)
    {
        return true;
    }
    return false;
}

