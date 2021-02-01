#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==================================================================================
/**
*/
class StereotypeAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    StereotypeAudioProcessorEditor (StereotypeAudioProcessor&);
    ~StereotypeAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    StereotypeAudioProcessor& audioProcessor;

    juce::Slider LtoLSlider;
    juce::Slider LtoRSlider;
    juce::Slider RtoRSlider;
    juce::Slider RtoLSlider;
    juce::Slider OffsetSlider;
    juce::Slider WidthSlider;
    juce::Label LtoLLabel;
    juce::Label LtoRLabel;
    juce::Label RtoRLabel;
    juce::Label RtoLLabel;
    juce::Label OffsetLabel;
    juce::Label WidthLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StereotypeAudioProcessorEditor)

    juce::AudioProcessorValueTreeState::SliderAttachment LtoLAttachment{
        audioProcessor.getValueTreeState(),
        StereotypeAudioProcessor::LtoLParam,
        LtoLSlider
    };
    juce::AudioProcessorValueTreeState::SliderAttachment LtoRAttachment{
        audioProcessor.getValueTreeState(),
        StereotypeAudioProcessor::LtoRParam,
        LtoRSlider
    };
    juce::AudioProcessorValueTreeState::SliderAttachment RtoRAttachment{
        audioProcessor.getValueTreeState(),
        StereotypeAudioProcessor::RtoRParam,
        RtoRSlider
    };
    juce::AudioProcessorValueTreeState::SliderAttachment RtoLAttachment{
        audioProcessor.getValueTreeState(),
        StereotypeAudioProcessor::RtoLParam,
        RtoLSlider
    };
    juce::AudioProcessorValueTreeState::SliderAttachment OffsetAttachment{
        audioProcessor.getValueTreeState(),
        StereotypeAudioProcessor::OffsetParam,
        OffsetSlider
    };
    juce::AudioProcessorValueTreeState::SliderAttachment WidthAttachment{
        audioProcessor.getValueTreeState(),
        StereotypeAudioProcessor::WidthParam,
        WidthSlider
    };

};
