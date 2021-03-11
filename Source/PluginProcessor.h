#pragma once

#include <JuceHeader.h>

//==================================================================================
/**
*/

class Visualizer : public juce::AudioVisualiserComponent
{
public:
    Visualizer() : AudioVisualiserComponent(2)
    {
        setBufferSize(512);
        setSamplesPerBlock(256);
    }
};


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
    static juce::String WidthParam;

    juce::Atomic<float> mLtoL{ 100.0f };
    juce::Atomic<float> mLtoR{ 0.0f };
    juce::Atomic<float> mRtoR{ 100.0f };
    juce::Atomic<float> mRtoL{ 0.0f };
    juce::Atomic<int> mOffset{ 0.0f };
    juce::Atomic<float> mWidth{ 50.0f };

    Visualizer visualizer;

private:

    juce::UndoManager mUndoManager;
    juce::AudioProcessorValueTreeState mState;

    float audioData[2048][2];
    float mid[2048];
    float side[2048];

    int audioRead = 0;
    int audioWrite = 0;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StereotypeAudioProcessor)
};
