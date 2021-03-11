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
    setSize (600, 380);
    getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colour::fromRGB(255, 145, 191));
    getLookAndFeel().setColour(juce::Slider::trackColourId, juce::Colour::fromRGB(244, 189, 217));
    getLookAndFeel().setColour(juce::Slider::backgroundColourId, juce::Colour::fromRGB(238, 211, 230));

    getLookAndFeel().setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour::fromRGB(238, 211, 230));
    getLookAndFeel().setColour(juce::Slider::textBoxOutlineColourId, juce::Colour::fromRGB(255, 145, 191));
    getLookAndFeel().setColour(juce::Slider::textBoxTextColourId, juce::Colour::fromRGB(6, 6, 12));
    getLookAndFeel().setColour(juce::Slider::textBoxHighlightColourId, juce::Colour::fromRGB(255, 145, 191));
    
    getLookAndFeel().setColour(juce::Label::textColourId, juce::Colour::fromRGB(6, 6, 12));

    getLookAndFeel().setColour(juce::ResizableWindow::backgroundColourId, juce::Colour::fromRGB(232, 233, 243));

    createSlider(LtoLSlider, LtoLLabel, -100, 100, "%", "Left mix");
    createSlider(LtoRSlider, LtoRLabel, -100, 100, "%", "Left to right");
    createSlider(RtoRSlider, RtoRLabel, -100, 100, "%", "Right mix");
    createSlider(RtoLSlider, RtoLLabel, -100, 100, "%", "Right to left");
    createSlider(OffsetSlider, OffsetLabel, -2048, 2048, " smp.", "Stereo offset");
    createSlider(WidthSlider, WidthLabel, 0, 100, "%", "Width");

    addAndMakeVisible(audioProcessor.visualizer);
    audioProcessor.visualizer.setColours(juce::Colour::fromRGB(238, 211, 230), juce::Colour::fromRGB(255, 145, 191));

    footer.setColour(juce::Toolbar::backgroundColourId, getLookAndFeel().findColour(juce::Slider::trackColourId));
    addAndMakeVisible(footer);

    addAndMakeVisible(footerTxt);
    footerTxt.setText("Stereotype by ari", juce::dontSendNotification);
    footerTxt.setColour(juce::Label::backgroundColourId, juce::Colour::fromRGB(244, 189, 217));
    footerTxt.setColour(juce::Label::textColourId, juce::Colour::fromRGB(6, 6, 12));
}

StereotypeAudioProcessorEditor::~StereotypeAudioProcessorEditor()
{
}

//==============================================================================
void StereotypeAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void StereotypeAudioProcessorEditor::resized()
{
    auto sliderLeft = 120;
    auto topOffset = 130;

    audioProcessor.visualizer.setBounds(10, 10, getWidth() - 20, 110);

    LtoLSlider.setBounds(sliderLeft, 0 + topOffset, getWidth() - sliderLeft - 10, 20);
    LtoRSlider.setBounds(sliderLeft, 40 + topOffset, getWidth() - sliderLeft - 10, 20);
    RtoRSlider.setBounds(sliderLeft, 80 + topOffset, getWidth() - sliderLeft - 10, 20);
    RtoLSlider.setBounds(sliderLeft, 120 + topOffset, getWidth() - sliderLeft - 10, 20);
    OffsetSlider.setBounds(sliderLeft, 160 + topOffset, getWidth() - sliderLeft - 10, 20);
    WidthSlider.setBounds(sliderLeft, 200 + topOffset, getWidth() - sliderLeft - 10, 20);

    auto area = getLocalBounds();

    footerTxt.setBounds(area.removeFromBottom(20));
}

void StereotypeAudioProcessorEditor::createSlider(juce::Slider& slider, juce::Label& label, double rstart, double rend, std::string suffix, std::string name) {
    addAndMakeVisible(slider);
    slider.setRange(rstart, rend);
    slider.setTextValueSuffix(suffix);
    slider.setColour(juce::Slider::textBoxOutlineColourId, getLookAndFeel().findColour(juce::Slider::textBoxOutlineColourId));
    addAndMakeVisible(label);
    label.setText(name, juce::dontSendNotification);

    label.attachToComponent(&slider, true);
}