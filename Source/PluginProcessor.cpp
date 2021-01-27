/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

juce::String StereotypeAudioProcessor::LtoLParam("Left mix");
juce::String StereotypeAudioProcessor::LtoRParam("Left to right");
juce::String StereotypeAudioProcessor::RtoRParam("Right mix");
juce::String StereotypeAudioProcessor::RtoLParam("Right to left");
juce::String StereotypeAudioProcessor::OffsetParam("Stereo offset");

StereotypeAudioProcessor::StereotypeAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
    mState(*this, &mUndoManager, "Stereotype",
        {
        std::make_unique<juce::AudioParameterFloat>(LtoLParam,
            TRANS("Left mix"),
            juce::NormalisableRange<float>(-100.0f, 100.0f, 0.001),
            mLtoL.get(), "%",
            juce::AudioProcessorParameter::genericParameter,
            [](float v, int) { return juce::String(v, 1); },
            [](const juce::String& t) { return t.dropLastCharacters(3).getFloatValue(); }),

        std::make_unique<juce::AudioParameterFloat>(LtoRParam,
            TRANS("Left to right"),
            juce::NormalisableRange<float>(-100.0f, 100.0f, 0.001),
            mLtoR.get(), "%",
            juce::AudioProcessorParameter::genericParameter,
            [](float v, int) { return juce::String(v, 1); },
            [](const juce::String& t) { return t.dropLastCharacters(3).getFloatValue(); }),

        std::make_unique<juce::AudioParameterFloat>(RtoRParam,
            TRANS("Right mix"),
            juce::NormalisableRange<float>(-100.0f, 100.0f, 0.001),
            mRtoR.get(), "%",
            juce::AudioProcessorParameter::genericParameter,
            [](float v, int) { return juce::String(v, 1); },
            [](const juce::String& t) { return t.dropLastCharacters(3).getFloatValue(); }),

        std::make_unique<juce::AudioParameterFloat>(RtoLParam,
            TRANS("Right to left"),
            juce::NormalisableRange<float>(-100.0f, 100.0f, 0.001),
            mRtoL.get(), "%",
            juce::AudioProcessorParameter::genericParameter,
            [](float v, int) { return juce::String(v, 1); },
            [](const juce::String& t) { return t.dropLastCharacters(3).getFloatValue(); }),

        std::make_unique<juce::AudioParameterFloat>(OffsetParam,
            TRANS("Stereo offset"),
            juce::NormalisableRange<float>(-2047.0f, 2047.0f, 0.001, 0.2f, true),
            mOffset.get(), "smp",
            juce::AudioProcessorParameter::genericParameter,
            [](float v, int) { return juce::String(v, 1); },
            [](const juce::String& t) { return t.dropLastCharacters(3).getFloatValue(); }),

        })
{
    mState.addParameterListener(LtoLParam, this);
    mState.addParameterListener(LtoRParam, this);
    mState.addParameterListener(RtoRParam, this);
    mState.addParameterListener(RtoLParam, this);
    mState.addParameterListener(OffsetParam, this);
}

void StereotypeAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue) {
    if (parameterID == LtoLParam) mLtoL = newValue;
    if (parameterID == LtoRParam) mLtoR = newValue;
    if (parameterID == RtoRParam) mRtoR = newValue;
    if (parameterID == RtoLParam) mRtoL = newValue;
    if (parameterID == OffsetParam) mOffset = newValue;
}

void StereotypeAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    auto* channelDataL = buffer.getWritePointer(0);
    auto* channelDataR = buffer.getWritePointer(1);
    for (int i = 0; i < buffer.getNumSamples(); ++i) {
        audioData[audioWrite][0] = channelDataL[i] * mLtoL.get() / 100;
        audioData[audioWrite][1] = channelDataR[i] * mRtoR.get() / 100;

        audioData[audioWrite][1] += channelDataL[i] * mLtoR.get() / 100;
        audioData[audioWrite][0] += channelDataR[i] * mRtoL.get() / 100;

        if (abs(audioWrite) == audioWrite) {
            channelDataL[i] = audioData[audioWrite][0];
            channelDataR[i] = audioData[audioRead][1];
        }
        else {
            channelDataR[i] = audioData[audioWrite][1];
            channelDataL[i] = audioData[audioRead][0];
        }

        audioWrite = (audioWrite + 1) % 2048;
        audioRead = (2048 + audioWrite - abs(mOffset.get())) % 2048;
    }
}

StereotypeAudioProcessor::~StereotypeAudioProcessor()
{
}

//==============================================================================
const juce::String StereotypeAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool StereotypeAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool StereotypeAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool StereotypeAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double StereotypeAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int StereotypeAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int StereotypeAudioProcessor::getCurrentProgram()
{
    return 0;
}

void StereotypeAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String StereotypeAudioProcessor::getProgramName (int index)
{
    return {};
}

void StereotypeAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void StereotypeAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void StereotypeAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool StereotypeAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

//==============================================================================
bool StereotypeAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* StereotypeAudioProcessor::createEditor()
{
    return new StereotypeAudioProcessorEditor (*this);
}

//==============================================================================
void StereotypeAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream stream(destData, false);
    mState.state.writeToStream(stream);
}

juce::AudioProcessorValueTreeState& StereotypeAudioProcessor::getValueTreeState()
{
    return mState;
}

void StereotypeAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    juce::ValueTree tree = juce::ValueTree::readFromData(data, sizeInBytes);
    if (tree.isValid()) mState.state = tree;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new StereotypeAudioProcessor();
}
