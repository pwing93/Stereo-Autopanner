/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
StereoAutopannerAudioProcessor::StereoAutopannerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    addParameter(gain = new AudioParameterFloat("gain", "Unused", 0.0, 1.0, 0.5));
    addParameter(mS = new AudioParameterFloat ("mS", "Time - mS", 10, 5000, 2500));
}

StereoAutopannerAudioProcessor::~StereoAutopannerAudioProcessor()
{
}

//==============================================================================
const String StereoAutopannerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool StereoAutopannerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool StereoAutopannerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool StereoAutopannerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double StereoAutopannerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int StereoAutopannerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int StereoAutopannerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void StereoAutopannerAudioProcessor::setCurrentProgram (int index)
{
}

const String StereoAutopannerAudioProcessor::getProgramName (int index)
{
    return {};
}

void StereoAutopannerAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void StereoAutopannerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void StereoAutopannerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool StereoAutopannerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void StereoAutopannerAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    auto* channeldataL = buffer.getWritePointer(0);
    auto* channeldataR = buffer.getWritePointer(1);
    
    float mSeconds = mS->get() / 1000;
    
    int numberSamples = getSampleRate() * mSeconds;
    const float radsPerSample = (2 * double_Pi) / numberSamples;
    
    float gSlider = gain->get();
    
    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
        auto inputL = channeldataL[i];
        auto inputR = channeldataR[i];
        
        float sinValue = std::sin(nextRad) + 1;
        sinValue = (sinValue * double_Pi) / 4;
        
        inputL = inputL * cos(sinValue);
        inputR = inputR * sin(sinValue);
        
        channeldataL[i] = inputL;
        channeldataR[i] = inputR;
        
        nextRad += radsPerSample;
    }
    
    if (nextRad > numberSamples)
    {
        nextRad = 0;
    }
}

//==============================================================================
bool StereoAutopannerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* StereoAutopannerAudioProcessor::createEditor()
{
    //return new StereoAutopannerAudioProcessorEditor (*this);
    return new GenericAudioProcessorEditor(this);
}

//==============================================================================
void StereoAutopannerAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void StereoAutopannerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new StereoAutopannerAudioProcessor();
}
