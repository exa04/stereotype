/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class StereotypeAudioProcessor  : public juce::AudioProcessor,
    public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    StereotypeAudioProcessor();
    ~StereotypeAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    void parameterChanged(const juce::String& parameterID, float newValue) override;

    juce::AudioProcessorValueTreeState& getValueTreeState();

    static juce::String LtoLParam;
    static juce::String LtoRParam;
    static juce::String RtoRParam;
    static juce::String RtoLParam;
    static juce::String OffsetParam;

    juce::Atomic<float> mLtoL{ 100.0f };
    juce::Atomic<float> mLtoR{ 0.0f };
    juce::Atomic<float> mRtoR{ 100.0f };
    juce::Atomic<float> mRtoL{ 0.0f };

    juce::Atomic<int> mOffset{ -120 };

private:

    juce::UndoManager mUndoManager;
    juce::AudioProcessorValueTreeState mState;

    float audioData[2048][2];

    int audioRead = 0;
    int audioWrite = 0;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StereotypeAudioProcessor)
};
