#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

void CustomLookAndFeel::drawRotarySlider(juce::Graphics & g,
                                   int x,
                                   int y,
                                   int w,
                                   int h,
                                   float sliderPosProportional,
                                   float rotaryStartAngle,
                                   float rotaryEndAngle,
                                   juce::Slider & slider)
{
    using namespace juce;

    auto bounds  = Rectangle<float>(x, y, w, h);

    g.setColour(Colour(97u, 18u, 167u));
    g.fillEllipse(bounds);

    g.setColour(Colour(255u , 154u, 1u));
    g.drawEllipse(bounds,1.f);

    if(auto* rswl = dynamic_cast<RotarySliderWithLabels*>(&slider))
    {
        auto center = bounds.getCentre();

        Path p;

        Rectangle<float> r;
        r.setLeft(center.getX() - 2);
        r.setRight(center.getX() + 2);
        r.setTop(bounds.getY());
        r.setBottom(center.getY() - rswl->getTextHeight() * 1.5);

        p.addRoundedRectangle(r, 2.f);


        jassert(rotaryStartAngle < rotaryEndAngle);

        auto sliderAngRad = jmap(sliderPosProportional, 0.f, 1.f, rotaryStartAngle, rotaryEndAngle);

        p.applyTransform(AffineTransform().rotated(sliderAngRad, center.getX(), center.getY()));

        g.fillPath(p);

        g.setFont(rswl-> getTextHeight());
        auto text = rswl->getDisplayString();
        auto strWidth = g.getCurrentFont().getStringWidth(text);

        r.setSize(strWidth + 4, rswl->getTextHeight() + 2);
        r.setCentre(bounds.getCentre());

        g.setColour(Colours::black);
        g.fillRect(r);

        g.setColour(Colours::white);
        g.drawFittedText(text,r.toNearestInt(),juce::Justification::centred, 1);

    }
}

//============================================================================================

void RotarySliderWithLabels::paint(juce::Graphics& g)
{
    using namespace juce;

    auto startAng = degreesToRadians(180.f + 45.f);
    auto endAng = degreesToRadians(180.f-45.f) + MathConstants<float>::twoPi;

    auto range = getRange();

    auto sliderBounds = getSliderBounds();

    getLookAndFeel().drawRotarySlider(g,
                                      sliderBounds.getX(),
                                      sliderBounds.getY(),
                                      sliderBounds.getWidth(),
                                      sliderBounds.getHeight(),
                                      jmap(getValue(), range.getStart(), range.getEnd(), 0.0, 1.0),
                                      startAng,
                                      endAng,
                                      *this);

    auto center = sliderBounds.toFloat().getCentre();
    auto radius = sliderBounds.getWidth() * 0.5f;

    g.setColour(Colour(0u, 172u, 1u));
    g.setFont(getTextHeight());

    auto  numChoices = labels.size();
    for( int i = 0; i< numChoices; ++i)
    {
        auto pos  = labels[i].pos;
        jassert(0.f<=pos);
        jassert(pos<=1.f);

        auto ang = jmap(pos, 0.f, 1.f, startAng, endAng);

        auto c = center.getPointOnCircumference(radius + getTextHeight() * 0.5f + 1, ang);

        Rectangle<float> r;
        auto str = labels[i].label;
        r.setSize(g.getCurrentFont().getStringWidth(str), getTextHeight());
        r.setCentre(c);
        r.setY(r.getY() + getTextHeight());
        g.drawFittedText(str, r.toNearestInt(), juce::Justification::centred, 1);
    }
}

//============================================================================================

juce::Rectangle<int> RotarySliderWithLabels::getSliderBounds() const
{

    auto bounds = getLocalBounds();

    auto size = juce::jmin(bounds.getWidth(), bounds.getHeight());

    size -= getTextHeight() * 2;

    juce::Rectangle<int> r;
    r.setSize(size, size);
    r.setCentre(bounds.getCentreX(), 0);
    r.setY(2);

    return r;
}

//============================================================================================

juce::String RotarySliderWithLabels::getDisplayString() const
{
    if (auto choiceParam = dynamic_cast<juce::AudioParameterChoice*>(param) )
        return choiceParam->getCurrentChoiceName();

    juce::String str;
    bool addK = false;

    if( auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(param))
    {
        float val = getValue();
        if( val > 999.f)
        {
            val /= 1000.f;
            addK = true;
        }
        str = juce::String(val, (addK ? 2 : 0));
    }
    else
    {
        jassertfalse;//hopefully shouldnt happen
    }

    if( suffix.isNotEmpty() )
    {
        str << " ";
        if( addK)
            str << "k";

        str << suffix;
    }

    return str;
}

//============================================================================================



AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p),
grainDensitySlider(*processorRef.apvts.getParameter("Grain Density"),"Grains"),
grainDurationSlider(*processorRef.apvts.getParameter("Grain Duration"),"Ms"),
playBackSpeedSlider(*processorRef.apvts.getParameter("Play-Back Speed"),"x"),
grainAttackSlider(*processorRef.apvts.getParameter("Grain-Attack"),"Ms"),
grainDecaySlider(*processorRef.apvts.getParameter("Grain-Decay"),"Ms"),
grainSustainSlider(*processorRef.apvts.getParameter("Grain-Sustain"),"Ms"),
globalAttackSlider(*processorRef.apvts.getParameter("Global-Attack"),"Ms"),
globalDecaySlider(*processorRef.apvts.getParameter("Global-Decay"),"Ms"),
globalSustainSlider(*processorRef.apvts.getParameter("Global-Sustain"),"Ms"),
globalReleaseSlider(*processorRef.apvts.getParameter("Global-Release"),"Ms"),

grainDensitySliderAttachment(processorRef.apvts,"Grain Density",grainDensitySlider),
grainDurationSliderAttachment(processorRef.apvts,"Grain Duration",grainDurationSlider),
playBackSpeedSliderAttachment(processorRef.apvts,"Play-Back Speed",playBackSpeedSlider),
grainAttackSliderAttachment(processorRef.apvts,"Grain-Attack",grainAttackSlider),
grainDecaySliderAttachment(processorRef.apvts,"Grain-Decay",grainDecaySlider),
grainSustainSliderAttachment(processorRef.apvts,"Grain-Sustain",grainSustainSlider),
globalAttackSliderAttachment(processorRef.apvts, "Global-Attack",globalAttackSlider),
globalDecaySliderAttachment(processorRef.apvts, "Global-Decay",globalDecaySlider),
globalReleaseSliderAttachment(processorRef.apvts, "Global-Release",globalReleaseSlider),
globalSustainSliderAttachment(processorRef.apvts,"Global-Sustain",globalSustainSlider)
{
    grainDensitySlider.labels.add({0.f,"1"});
    grainDensitySlider.labels.add({1.f,"256"});

    grainDurationSlider.labels.add({0.f,"5ms"});
    grainDurationSlider.labels.add({1.f,"500ms"});

    playBackSpeedSlider.labels.add({0.f,"-6x"});
    playBackSpeedSlider.labels.add({1.f,"6x"});
    //
    grainAttackSlider.labels.add({0.f,"0.1ms"});
    grainAttackSlider.labels.add({1.f,"100ms"});

    grainDecaySlider.labels.add({0.f,"0.01ms"});
    grainDecaySlider.labels.add({1.f,"10ms"});

    grainSustainSlider.labels.add({0.f,"0.1ms"});
    grainSustainSlider.labels.add({1.f,"100ms"});
    //
    globalAttackSlider.labels.add({0.f,"0.5ms"});
    globalAttackSlider.labels.add({1.f,"500ms"});

    globalDecaySlider.labels.add({0.f,"0.05ms"});
    globalDecaySlider.labels.add({1.f,"50ms"});

    globalReleaseSlider.labels.add({0.f,"0.5ms"});
    globalReleaseSlider.labels.add({1.f,"500ms"});

    globalSustainSlider.labels.add({0.f,"5ms"});
    globalSustainSlider.labels.add({1.f,"500ms"});

    for (auto* comp : getComps() )
    {
        addAndMakeVisible(comp);
    }

    const auto& params = processorRef.getParameters();
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 500);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

}

void AudioPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    auto bounds = getLocalBounds();
    float hRatio = 25.f/100.f;
    //auto responseArea = bounds.removeFromTop(bounds.getHeight() *0.33);

    bounds.removeFromTop(5);

    auto grainParamArea = bounds.removeFromLeft(bounds.getWidth() * 0.33);
    auto grainEnvelopeParamArea = bounds.removeFromRight(bounds.getWidth() * 0.5);

    grainDensitySlider.setBounds(grainParamArea.removeFromTop(bounds.getHeight() * 0.33));
    grainDurationSlider.setBounds(grainParamArea.removeFromTop(bounds.getHeight() * 0.66));
    playBackSpeedSlider.setBounds(grainParamArea);

    grainAttackSlider.setBounds(grainParamArea.removeFromTop(bounds.getHeight() * 0.33));
    grainDecaySlider.setBounds(grainParamArea.removeFromTop(bounds.getHeight() * 0.66));
    grainSustainSlider.setBounds(grainParamArea);

    globalAttackSlider.setBounds(bounds.removeFromTop(bounds.getHeight() * 0.33));
    globalDecaySlider.setBounds(bounds.removeFromTop(bounds.getHeight() * 0.5));
    globalSustainSlider.setBounds(bounds.removeFromTop(bounds.getHeight() * 0.66));
    globalReleaseSlider.setBounds(bounds);
}

std::vector<juce::Component*> AudioPluginAudioProcessorEditor::getComps()
{
    return {
        &grainDensitySlider,
        &grainDurationSlider,
        &playBackSpeedSlider,
        &grainAttackSlider,
        &grainDecaySlider,
        &grainSustainSlider,
        &globalAttackSlider,
        &globalDecaySlider,
        &globalSustainSlider,
        &globalReleaseSlider
    };
}