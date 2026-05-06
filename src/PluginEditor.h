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
                                juce::Slider& slider) override;
    void drawLinearSlider(juce::Graphics& g,
                            int x,
                            int y,
                            int width,
                            int height,
                            float sliderPos,
                            float minSliderPos,
                            float maxSliderPos,
                            juce::Slider::SliderStyle style,
                            juce::Slider& slider) override;
    void drawDrawableButton(juce::Graphics&, juce::DrawableButton&,
                            bool shouldDrawButtonAsHighlighted,
                            bool shouldDrawButtonAsDown) override;
};

//=============================================================================
struct VerticalSliderWithLabels : juce::Slider
{
    VerticalSliderWithLabels(juce::RangedAudioParameter& rap, const juce::String& unitSuffix) :
    juce::Slider(juce::Slider::SliderStyle::LinearVertical,
                juce::Slider::TextEntryBoxPosition::NoTextBox),
    param(&rap),
    suffix(unitSuffix)
    {
        setLookAndFeel(&lnf);
    }

    ~VerticalSliderWithLabels()
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

struct XYPadController : juce::Component
{
    XYPadController(AudioPluginAudioProcessor&);
    ~XYPadController();

    void paint(juce::Graphics& g) override;
    void resized() override;
    void mouseDrag(const juce::MouseEvent& event) override;
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override;

private:
    AudioPluginAudioProcessor& processorRef;
    juce::Point<float> handlePosition {0.5f,0.5f};
    juce::Rectangle<int> getRenderArea() const;
    bool isDragging {false};
};

//==============================================================================

struct EnvelopeSelectorComponent : juce::Component,
                                    juce::Button::Listener
{
    EnvelopeSelectorComponent(juce::AudioProcessorValueTreeState& apvts) :
    apvts(apvts)
    {
        for (auto* btn : {&gaussianDrawableButton, &hannDrawableButton ,&trapezoidDrawableButton })
        {
            btn->setRadioGroupId(1);
            btn->setClickingTogglesState(true);
            btn->addListener(this);
            addAndMakeVisible(btn);
        }
        setLookAndFeel(&lnf);
        hannDrawableButton.setToggleState(true,juce::dontSendNotification);
    }
    ~EnvelopeSelectorComponent()
    {
        setLookAndFeel(nullptr);
    }

    void paint(juce::Graphics& g) override;
    void resized() override;
    void buttonClicked(juce::Button* button) override;

private:
    juce::DrawableButton gaussianDrawableButton{"Gaussian",juce::DrawableButton::ImageRaw};
    juce::DrawableButton hannDrawableButton{"Hann",juce::DrawableButton::ImageRaw};
    juce::DrawableButton trapezoidDrawableButton{"Trapezoid",juce::DrawableButton::ImageRaw};
    CustomLookAndFeel lnf;
    //EnvelopeType& type;
    juce::AudioProcessorValueTreeState& apvts;
};

//==============================================================================

struct WaveFormComponent : juce::Component,
                            juce::Timer
{
    WaveFormComponent(AudioPluginAudioProcessor&);
    ~WaveFormComponent();

    void timerCallback() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    AudioPluginAudioProcessor& processorRef;

    static constexpr int MaxSamples = 48000;
    std::array<float, MaxSamples> sampleBuf;
    int writeHead = 0;
    int totalSamplesCollected = 0;

    juce::Rectangle<int> getRenderArea() const;
    juce::Rectangle<int> getAnalysisArea() const;
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
    randomnessSlider,
    dryWetSlider;

    VerticalSliderWithLabels postGainSlider;

    //APVTS is used to manage global plugin state
    using APVTS = juce::AudioProcessorValueTreeState;
    //Attachment creates a listener to attach to the param objects
    using Attachment = APVTS::SliderAttachment;

    WaveFormComponent waveFormComponent;

    EnvelopeSelectorComponent envelopeSelector;

    Attachment grainDensitySliderAttachment,
    grainDurationSliderAttachment,
    playBackSpeedSliderAttachment,
    randomnessSliderAttachment,
    postGainSliderAttachment,
    dryWetSliderAttachment;

    XYPadController xypadController;

    juce::Rectangle<int> grainEnvBox, waveFormBox, gainBox;
    juce::TextButton powerButton;
    juce::ButtonParameterAttachment powerButtonAttachment;

    //allows for ease of access to components
    std::vector<juce::Component*> getComps();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
