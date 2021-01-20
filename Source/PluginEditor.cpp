/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
StereotypeAudioProcessorEditor::StereotypeAudioProcessorEditor (StereotypeAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 160);

    addAndMakeVisible(LtoLSlider);
    LtoLSlider.setRange(-100, 100);
    LtoLSlider.setTextValueSuffix("%");
    addAndMakeVisible(LtoLLabel);
    LtoLLabel.setText("Left mix", juce::dontSendNotification);
    LtoLLabel.attachToComponent(&LtoLSlider, true);

    addAndMakeVisible(LtoRSlider);
    LtoRSlider.setRange(-100, 100);
    LtoRSlider.setTextValueSuffix("%");
    addAndMakeVisible(LtoRLabel);
    LtoRLabel.setText("Left to right", juce::dontSendNotification);
    LtoRLabel.attachToComponent(&LtoRSlider, true);

    addAndMakeVisible(RtoRSlider);
    RtoRSlider.setRange(-100, 100);
    RtoRSlider.setTextValueSuffix("%");
    addAndMakeVisible(RtoRLabel);
    RtoRLabel.setText("Right mix", juce::dontSendNotification);
    RtoRLabel.attachToComponent(&RtoRSlider, true);

    addAndMakeVisible(RtoLSlider);
    RtoLSlider.setRange(-100, 100);
    RtoLSlider.setTextValueSuffix("%");
    addAndMakeVisible(RtoLLabel);
    RtoLLabel.setText("Right to left", juce::dontSendNotification);
    RtoLLabel.attachToComponent(&RtoLSlider, true);
}

StereotypeAudioProcessorEditor::~StereotypeAudioProcessorEditor()
{
}

//==============================================================================
void StereotypeAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    /*
    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
    */
}

void StereotypeAudioProcessorEditor::resized()
{
    auto sliderLeft = 120;
    LtoLSlider.setBounds(sliderLeft, 10, getWidth() - sliderLeft - 10, 20);
    LtoRSlider.setBounds(sliderLeft, 50, getWidth() - sliderLeft - 10, 20);
    RtoRSlider.setBounds(sliderLeft, 90, getWidth() - sliderLeft - 10, 20);
    RtoLSlider.setBounds(sliderLeft, 130, getWidth() - sliderLeft - 10, 20);
}
