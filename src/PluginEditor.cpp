#include "PluginProcessor.h"
#include "PluginEditor.h"

void CustomLookAndFeel::drawLinearSlider(juce::Graphics& g,
                                        int x, int y, int w, int h,
                                        float sliderPos,
                                        float minSliderPos,
                                        float maxSliderPos,
                                        juce::Slider::SliderStyle style,
                                        juce::Slider& slider)
{
    using namespace juce;

    auto bounds = Rectangle<float>(x, y, w, h);
    auto center = bounds.getCentre();
    Rectangle<float>track;
    Rectangle<float>filledTrack;

    track.setLeft(center.getX() - 3);
    track.setRight(center.getX() + 3);
    track.setTop(bounds.getY());
    track.setBottom(bounds.getBottom());

    filledTrack.setLeft(center.getX() - 3);
    filledTrack.setRight(center.getX() + 3);
    filledTrack.setTop(sliderPos);
    filledTrack.setBottom(bounds.getBottom());

    g.setColour(Colours::rebeccapurple.darker());
    g.fillRoundedRectangle(track, 3.f);
    g.setColour(Colours::rebeccapurple);
    g.fillRoundedRectangle(filledTrack, 3.f);

    Rectangle<float>thumb;

    thumb.setLeft(center.getX() - 12);
    thumb.setRight(center.getX() + 12);
    thumb.setTop(sliderPos - 5);
    thumb.setBottom(sliderPos + 5);

    g.setColour(Colours::greenyellow);
    g.fillRoundedRectangle(thumb, 3.f);

    if (auto* vswl = dynamic_cast<VerticalSliderWithLabels*>(&slider))
    {
        g.setFont(vswl->getTextHeight());
        auto text = vswl->getDisplayString();
        auto strWidth = g.getCurrentFont().getStringWidth(text);

        Rectangle<float> r;
        r.setSize(strWidth + 4, vswl->getTextHeight() + 2);
        r.setCentre(center.getX(),sliderPos);

        g.setColour(Colours::black);
        g.fillRect(r);

        g.setColour(Colours::white);
        g.drawFittedText(text,r.toNearestInt(),juce::Justification::centred, 1);
    }
}

//==============================================================================

void VerticalSliderWithLabels::paint(juce::Graphics& g)
{
    using namespace juce;

    auto range = getRange();
    auto sliderBounds = getSliderBounds();

    auto sliderPos = jmap(getValue(),
                        range.getStart(),range.getEnd(),
                        static_cast<double>(sliderBounds.getBottom()),
                        static_cast<double>(sliderBounds.getY()));
    getLookAndFeel().drawLinearSlider(g,
                                    sliderBounds.getX(),
                                    sliderBounds.getY(),
                                    sliderBounds.getWidth(),
                                    sliderBounds.getHeight(),
                                    static_cast<float>(sliderPos),
                                    static_cast<float>(sliderBounds.getBottom()),
                                    static_cast<float>(sliderBounds.getY()),
                                    getSliderStyle(),
                                    *this);
    g.setColour(Colours::white);
    g.setFont(getTextHeight());

    auto minLabel = String(range.getStart());
    auto maxLabel = String(range.getEnd());

    g.drawFittedText(maxLabel,
                    sliderBounds.getX(),sliderBounds.getY() - getTextHeight(),
                    sliderBounds.getWidth(),getTextHeight(),
                    Justification::centred,1);
    g.drawFittedText(minLabel,
                sliderBounds.getX(),sliderBounds.getBottom(),
                sliderBounds.getWidth(),getTextHeight(),
                Justification::centred,1);
}

juce::Rectangle<int> VerticalSliderWithLabels::getSliderBounds() const
{

    auto bounds = getLocalBounds();
    bounds.removeFromTop(getTextHeight());
    bounds.removeFromBottom(getTextHeight());
    bounds.reduce(bounds.getWidth() / 3 , 0);
    return bounds;
}

juce::String VerticalSliderWithLabels::getDisplayString() const
{
    if (auto choiceParam = dynamic_cast<juce::AudioParameterChoice*>(param) )
        return choiceParam->getCurrentChoiceName();

    juce::String str;
    auto val = param->getValue();
    auto denormVal = param->getNormalisableRange().convertFrom0to1(val);

    str << juce::String(denormVal,1);

    if( suffix.isNotEmpty() )
    {
        str << " " << suffix;
    }

    return str;
}

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

    g.setColour(Colours::rebeccapurple);
    g.fillEllipse(bounds);

    g.setColour(Colours::greenyellow);
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

    g.setColour(juce::Colours::white);
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
    auto val = param->getValue();
    auto denormVal = param->getNormalisableRange().convertFrom0to1(val);

    str << juce::String(denormVal,1);

    if( suffix.isNotEmpty() )
    {
        str << " " << suffix;
    }

    return str;
}

//============================================================================================

WaveFormComponent::WaveFormComponent(AudioPluginAudioProcessor& p ) : processorRef(p)
{
        startTimerHz(30);
}

WaveFormComponent::~WaveFormComponent()
{
    stopTimer();
}

juce::Rectangle<int> WaveFormComponent::getRenderArea() const
{
    auto bounds = getLocalBounds();
    bounds.removeFromTop(4);
    bounds.removeFromBottom(4);
    bounds.removeFromLeft(4);
    bounds.removeFromRight(4);

    return bounds;
}

juce::Rectangle<int> WaveFormComponent::getAnalysisArea() const
{
    auto bounds = getRenderArea();

    bounds.removeFromTop(4);
    bounds.removeFromBottom(4);

    return bounds;
}

void WaveFormComponent::timerCallback()
{
    juce::AudioBuffer<float> tempBuf;
    while (processorRef.leftChannelFifo.getNumCompleteBuffersAvailable() > 0)
    {
        if (processorRef.leftChannelFifo.getAudioBuffer(tempBuf))
        {
            auto* data = tempBuf.getReadPointer(0);
            for (auto i = 0; i < tempBuf.getNumSamples(); ++i)
            {
                sampleBuf[writeHead % MaxSamples] = data[i];
                ++writeHead;
                totalSamplesCollected = juce::jmin(totalSamplesCollected + 1, MaxSamples);
            }
        }
    }
    repaint();
}

void WaveFormComponent:: paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);

    g.setColour(juce::Colours::grey);
    g.drawRect(getRenderArea());

    const auto analysisArea = getAnalysisArea();
    const int W = analysisArea.getWidth();
    const int midY = analysisArea.getCentreY();
    const float halfH = analysisArea.getHeight() * 0.5f;

    if (totalSamplesCollected == 0) return;

    juce::Path waveform;
    for (int p = 0; p < W ; ++p)
    {
        int sampleStart = static_cast<int>(static_cast<float>(p) / W * totalSamplesCollected);
        int sampleEnd = juce::jmax(static_cast<int>(static_cast<float>(p + 1) / W * totalSamplesCollected) , sampleStart + 1);

        float minVal = 0.f, maxVal = 0.f;

        for (int sample = sampleStart; sample < sampleEnd; ++sample)
        {
            int index = (writeHead - totalSamplesCollected + sample + MaxSamples) % MaxSamples;
            float indivSample = sampleBuf[index];
            minVal = juce::jmin(minVal, indivSample);
            maxVal = juce::jmax(maxVal, indivSample);
        }

        float yTop = midY - (maxVal * halfH);
        float yBottom = midY - (minVal * halfH);

        if (p == 0)
        {
            waveform.startNewSubPath(static_cast<float>(analysisArea.getX() + p), yTop);
            waveform.startNewSubPath(static_cast<float>(analysisArea.getX() + p), yBottom);
        }else
        {
            waveform.lineTo(static_cast<float>(analysisArea.getX() + p), yTop);
            waveform.lineTo(static_cast<float>(analysisArea.getX() + p), yBottom);
        }

    }

    g.setColour(juce::Colours::rebeccapurple);
    g.strokePath(waveform, juce::PathStrokeType(1.5f));
}

void WaveFormComponent:: resized()
{
    repaint();
}

//============================================================================================

AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p),
grainDensitySlider(*processorRef.apvts.getParameter("grainDensity"),"Grains"),
grainDurationSlider(*processorRef.apvts.getParameter("grainDuration"),"Ms"),
playBackSpeedSlider(*processorRef.apvts.getParameter("playBackSpeed"),"x"),
globalAttackSlider(*processorRef.apvts.getParameter("globalAttack"),"Ms"),
globalDecaySlider(*processorRef.apvts.getParameter("globalDecay"),"Ms"),
globalSustainSlider(*processorRef.apvts.getParameter("globalSustain"),"Ms"),
globalReleaseSlider(*processorRef.apvts.getParameter("globalRelease"),"Ms"),
randomnessSlider(*processorRef.apvts.getParameter("randomness"),"%"),
postGainSlider(*processorRef.apvts.getParameter("postGain"),"Db"),
dryWetSlider(*processorRef.apvts.getParameter("dryWet"), "%"),

waveFormComponent(processorRef),

grainDensitySliderAttachment(processorRef.apvts,"grainDensity",grainDensitySlider),
grainDurationSliderAttachment(processorRef.apvts,"grainDuration",grainDurationSlider),
playBackSpeedSliderAttachment(processorRef.apvts,"playBackSpeed",playBackSpeedSlider),
globalAttackSliderAttachment(processorRef.apvts, "globalAttack",globalAttackSlider),
globalDecaySliderAttachment(processorRef.apvts, "globalDecay",globalDecaySlider),
globalReleaseSliderAttachment(processorRef.apvts, "globalRelease",globalReleaseSlider),
globalSustainSliderAttachment(processorRef.apvts,"globalSustain",globalSustainSlider),
powerButtonAttachment(*processorRef.apvts.getParameter("bypass"),powerButton),
randomnessSliderAttachment(processorRef.apvts,"randomness",randomnessSlider),
postGainSliderAttachment(processorRef.apvts,"postGain",postGainSlider),
dryWetSliderAttachment(processorRef.apvts,"dryWet",dryWetSlider)
{
    grainDensitySlider.labels.add({0.f,"1"});
    grainDensitySlider.labels.add({1.f,"256"});

    grainDurationSlider.labels.add({0.f,"2ms"});
    grainDurationSlider.labels.add({1.f,"100ms"});

    playBackSpeedSlider.labels.add({0.f,".5x"});
    playBackSpeedSlider.labels.add({0.5f,"1x"});
    playBackSpeedSlider.labels.add({1.f,"2x"});
    //
    globalAttackSlider.labels.add({0.f,"0.1ms"});
    globalAttackSlider.labels.add({1.f,"100ms"});

    globalDecaySlider.labels.add({0.f,"0.01ms"});
    globalDecaySlider.labels.add({1.f,"10ms"});

    globalReleaseSlider.labels.add({0.f,"0.01ms"});
    globalReleaseSlider.labels.add({1.f,"100ms"});

    globalSustainSlider.labels.add({0.f,"0.1ms"});
    globalSustainSlider.labels.add({1.f,"100ms"});

    randomnessSlider.labels.add({0.f,"0%"});
    randomnessSlider.labels.add({1.f,"100%"});

    postGainSlider.labels.add({0.f,"-24dB"});
    postGainSlider.labels.add({0.5f,"0dB"});
    postGainSlider.labels.add({1.f,"+6dB"});

    dryWetSlider.labels.add({0.f,"0%"});
    dryWetSlider.labels.add({0.5f,"50%"});
    dryWetSlider.labels.add({1.f,"100%"});

    powerButton.setButtonText("Bypass");
    powerButton.setClickingTogglesState(true);
    powerButton.onClick = [this]()
    {
        bool bypassed = powerButton.getToggleState();
        for (auto* comp : getComps() )
            comp->setEnabled(!bypassed);
        repaint();
    };

    for (auto* comp : getComps() )
    {
        addAndMakeVisible(comp);
    }

    addAndMakeVisible(powerButton);
    const auto& params = processorRef.getParameters();
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (1000, 600);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    bool bypass = powerButton.getToggleState();

    g.fillAll(bypass? juce::Colours::grey : juce::Colours::black);
    //bypass interactivity
    g.setColour(juce::Colours::white);
    // bordered boxes
    g.drawRect(grainEnvBox, 1);
    g.drawRect(globalEnvBox, 1);
    g.drawRect(waveFormBox, 1);
    g.drawRect(gainBox, 1);
    // section labels
    g.setFont(12.f);
    g.drawFittedText("Grain Parameters",
        10, 245, 180, 20,
        juce::Justification::centred, 1);
    // g.drawFittedText("Grain ADR",
    //     grainEnvBox.getX(), grainEnvBox.getY() - 20,
    //     grainEnvBox.getWidth(), 20,
    //     juce::Justification::centred, 1);
    g.drawFittedText("Global ADSR",
        globalEnvBox.getX(), globalEnvBox.getY() - 20,
        globalEnvBox.getWidth(), 20,
        juce::Justification::centred, 1);

}

void AudioPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    //auto bounds = getLocalBounds().reduced (10);

    int bottomY = 260;
    //int bottomH = getHeight() - bottomY - 10;

    int grainParamX = 10;
    int knobLargeSize = 100;
    int knobSmallSize = 80;

    //bypass button
    powerButton.setBounds(10,10,50,50);

    waveFormBox = juce::Rectangle(10,70,980,170);
    waveFormComponent.setBounds(waveFormBox);

    //granular parameters area
    grainDensitySlider.setBounds(grainParamX , bottomY + 20, knobLargeSize, knobLargeSize);
    grainDurationSlider.setBounds(grainParamX, bottomY + 140, knobSmallSize, knobSmallSize);
    playBackSpeedSlider.setBounds(grainParamX + knobSmallSize + 10 , bottomY + 140, knobSmallSize, knobSmallSize);
    randomnessSlider.setBounds(grainParamX + knobSmallSize + 10,bottomY + 20,knobSmallSize,knobSmallSize);

    //global envelope area
    int globalEnvX = 630;
    int globalEnvW = 260;
    int globalEnvH = 200;
    globalEnvBox = juce::Rectangle(globalEnvX,bottomY + 10,globalEnvW,globalEnvH);

    int globalKnobSize = 90;
    int globalPadX = (globalEnvW - globalKnobSize * 2) / 3;
    int globalPadY = (globalEnvH - globalKnobSize * 2) / 3;

    globalAttackSlider.setBounds(globalEnvBox.getX() + globalPadX, globalEnvBox.getY() + globalPadY, globalKnobSize, globalKnobSize);
    globalDecaySlider.setBounds(globalEnvBox.getX() + globalPadX * 2 + globalKnobSize, globalEnvBox.getY() + globalPadY, globalKnobSize, globalKnobSize);
    globalSustainSlider.setBounds(globalEnvBox.getX() + globalPadX, globalEnvBox.getY() + globalPadY * 2 + globalKnobSize, globalKnobSize, globalKnobSize);
    globalReleaseSlider.setBounds(globalEnvBox.getX() + globalPadX * 2 + globalKnobSize, globalEnvBox.getY() + globalPadY * 2 + globalKnobSize, globalKnobSize, globalKnobSize);
    dryWetSlider.setBounds(450,bottomY,globalKnobSize,globalKnobSize);
    postGainSlider.setBounds(350,bottomY,60,220);

}

std::vector<juce::Component*> AudioPluginAudioProcessorEditor::getComps()
{
    return {
        &grainDensitySlider,
        &grainDurationSlider,
        &playBackSpeedSlider,
        &globalAttackSlider,
        &globalDecaySlider,
        &globalSustainSlider,
        &globalReleaseSlider,
        &waveFormComponent,
        &randomnessSlider,
        &postGainSlider,
        &dryWetSlider,
    };
}