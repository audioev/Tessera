//
// Created by Evan Neumann on 14/03/2026.
//

#pragma once
#include <juce_audio_basics/juce_audio_basics.h>

class CircularBuffer
{
    public:
    CircularBuffer()
    {
        writeHead = 0;
    };

    ~CircularBuffer() = default;

    void prepare(int numChannels, int numSamples)
    {
        buffer.setSize(numChannels,numSamples);
        buffer.clear();
    }

    void write(juce::AudioBuffer<float>& input)
    {
        for (int sample = 0; sample < input.getNumSamples(); sample++)
        {
            for (int channel = 0; channel < input.getNumChannels(); channel++)
            {
                buffer.setSample(channel, writeHead, input.getSample(channel,sample));
            }
            writeHead = (writeHead + 1) % buffer.getNumSamples();
        }
    }

    const float* read(const int channel, const int startSample, const int currentSample)
    {
        return buffer.getReadPointer(channel, (startSample + currentSample) % buffer.getNumSamples());
    }

    int getWriteHead(){return writeHead;}
    int getBufSize() const {return buffer.getNumSamples();}

    private:
    int writeHead = 0;
    juce::AudioBuffer<float> buffer;
};
