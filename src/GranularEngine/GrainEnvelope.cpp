//
// Created by Evan Neumann on 07/03/2026.
//

#include "GrainEnvelope.h"

GrainEnvelope::GrainEnvelope()
{

}

GrainEnvelope::~GrainEnvelope(){}

void GrainEnvelope::configure(EnvelopeType type,int totalSamples)
{
    this->type= type;
    this->totalSamples;
}

float GrainEnvelope::getHannEnvelope(float samplePhase)
{
    float HannReturnAmplitude;

    HannReturnAmplitude = 2.f * M_PI * samplePhase;
    return HannReturnAmplitude;
}