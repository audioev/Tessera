//
// Created by Evan Neumann on 05/03/2026.
//



#include "Grain.h"

//#include "../../../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks/CoreGraphics.framework/Headers/CGRenderingBufferProvider.h"

Grain::Grain()
{
    startSample = 0;
    duration = 0;
    currentSample = 0;
    pitch = 0;
    amplitude = 0.f;
    isActive = false;
    totalSamples = 0;
}

Grain::~Grain(){}

void Grain::setActive(bool status)
{
    this->isActive = status;
}

bool Grain::getActive()
{
    return this->isActive;
}

void Grain::configure(int duration, int startSample, float pitch, float amplitude,int totalSamples ,EnvelopeType type)
{
    this->duration = duration;
    this->startSample = startSample;
    this->amplitude = amplitude;
    this->pitch = pitch;
    this->totalSamples = totalSamples;
    envelope.configure(type,totalSamples);
}

float Grain::getNextSample(const float* sample)
{
    float const phase = static_cast<float>(currentSample) / static_cast<float>(totalSamples);
    float rawSample = *sample;
    rawSample = rawSample * envelope.calculate(phase) * amplitude;
    currentSample++;
    return rawSample;
}

bool Grain::isFinished()
{
    if (currentSample >= totalSamples)
    {
        return false;
    }
    return true;
}

void Grain::reset()
{
    startSample = 0;
    duration = 0;
    currentSample = 0;
    pitch = 0;
    amplitude = 0.f;
    isActive = false;
}
