#pragma once

#include "PluginProcessor.h"
#include <juce_dsp/juce_dsp.h>
#include <array>

//=============================================================================
/*
 * Defines the LookAndFeel style of our rotarySlider object
 */

struct CustomLookAndFeel : juce::LookAndFeel_V4
{
    void drawRotarySlider (juce::Graphics& g,
                                int x, int y , int w, int h,
                                float sliderPosProportional,
                                float rotaryStartAngle,
                                float rotaryEndAngle,
                                juce::Slider& ) override;

    // void drawButton (juce::Graphics& g,
    //                         int x, int y , int w, int h,
    //                         float )
};

//=============================================================================

struct RotarySliderWithLabels : juce::Slider
{
    RotarySliderWithLabels(juce::RangedAudioParameter& rap, const juce::String& unitSuffix) :
    juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                juce::Slider::TextEntryBoxPosition::NoTextBox),
    param(&rap),
    suffix(unitSuffix)
    {
        setLookAndFeel(&lnf);
    }

    ~RotarySliderWithLabels()
    {
        setLookAndFeel(nullptr);
    }

    struct LabelPos
    {
        float pos;
        juce::String label;
    };

    juce::Array<LabelPos> labels;

    void paint(juce::Graphics& g )override;
    juce::Rectangle<int> getSliderBounds() const;
    int getTextHeight() const { return 14; }
    juce::String getDisplayString() const;

private:
    CustomLookAndFeel lnf;
    juce::RangedAudioParameter* param;
    juce::String suffix;
};
//==============================================================================
class AudioPluginAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor&);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioPluginAudioProcessor& processorRef;

    //declare the rotary sliders we use here
    RotarySliderWithLabels grainDensitySlider,
    grainDurationSlider,
    playBackSpeedSlider,
    grainAttackSlider,
    grainDecaySlider,
    grainSustainSlider,
    globalAttackSlider,
    globalDecaySlider,
    globalSustainSlider,
    globalReleaseSlider;

    //APVTS is used to manage global plugin state
    using APVTS = juce::AudioProcessorValueTreeState;
    //Attachment creates a listener to attach to the param objects
    using Attachment = APVTS::SliderAttachment;

    Attachment grainDensitySliderAttachment,
    grainDurationSliderAttachment,
    playBackSpeedSliderAttachment,
    grainAttackSliderAttachment,
    grainDecaySliderAttachment,
    grainSustainSliderAttachment,
    globalAttackSliderAttachment,
    globalDecaySliderAttachment,
    globalSustainSliderAttachment,
    globalReleaseSliderAttachment;

    //allows for ease of access to components
    std::vector<juce::Component*> getComps();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
