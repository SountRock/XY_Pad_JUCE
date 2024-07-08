/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
/* эти макросы создают ввод и вывод - нужны в синтезаторе
#ifndef JucePlugin_PreferredChannelConfigurations
	 : AudioProcessor (BusesProperties()
						#if ! JucePlugin_IsMidiEffect
						#if ! JucePlugin_IsSynth
					   .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
						#endif
					   .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
						#endif
					   )
#endif
*/
XY_PadAudioProcessor::XY_PadAudioProcessor() : AudioProcessor(BusesProperties()
		.withInput("Input", juce::AudioChannelSet::stereo(), true)
		.withOutput("Output", juce::AudioChannelSet::stereo(), true)), 
parameters(*this, nullptr, "xypad", { 
	std::make_unique<juce::AudioParameterFloat>("pan", "Pan", juce::NormalisableRange<float>{-1.f, 1.f, 0.01f}, 0.f),
	std::make_unique<juce::AudioParameterFloat>("gain", "Gain", juce::NormalisableRange<float>{-30.f, 12.f, 0.01f}, 0.f)
	})
{
/*
	parameters(*this, nullptr, "xypad", {
	std::make_unique<juce::AudioParameterFloat>("pan", "Pan", juce::NormalisableRange<float>{-1.f, 1.f, 0.01f}, 0.f),
	std::make_unique<juce::AudioParameterFloat>("gain", "Gain", juce::NormalisableRange<float>{-60.f, 0.f, 0.01f}, 0.f)
		})
//1 - указатель на класс аудио процессора, для разменования, 2 -  диспечер отмены, 3 - родительский индефикатор, 4 - список уникальных 
//указательей на аудиопараметры
*/

	parameters.addParameterListener("gain", this);
	parameters.addParameterListener("pan", this);
}

XY_PadAudioProcessor::~XY_PadAudioProcessor()
{
	parameters.removeParameterListener("gain", this);
	parameters.removeParameterListener("pan", this);
}

//==============================================================================
const juce::String XY_PadAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool XY_PadAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool XY_PadAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool XY_PadAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double XY_PadAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int XY_PadAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int XY_PadAudioProcessor::getCurrentProgram()
{
    return 0;
}

void XY_PadAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String XY_PadAudioProcessor::getProgramName (int index)
{
    return {};
}

void XY_PadAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void XY_PadAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{ 
	juce::dsp::ProcessSpec spec{ sampleRate, samplesPerBlock, getTotalNumInputChannels() };
	gainProcessor.prepare(spec);
	panProcessor.prepare(spec);

	//
	gainProcessor.setGainDecibels(parameters.getRawParameterValue("gain")->load());
	panProcessor.setPan(parameters.getRawParameterValue("pan")->load());
	//
}

void XY_PadAudioProcessor::releaseResources()
{
	gainProcessor.reset();
	panProcessor.reset();
}


bool XY_PadAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true;
}

void XY_PadAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
	dsp::AudioBlock<float> audioBlock { buffer };
	dsp::ProcessContextReplacing<float> context{ audioBlock };
	gainProcessor.process(context);
	panProcessor.process(context);
}

//==============================================================================
bool XY_PadAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* XY_PadAudioProcessor::createEditor()
{
    return new XY_PadAudioProcessorEditor (*this);
}

//==============================================================================
void XY_PadAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void XY_PadAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

juce::AudioProcessorValueTreeState& XY_PadAudioProcessor::getAPVTS()
{
	return parameters;
}

void XY_PadAudioProcessor::parameterChanged(const String& parameterID, float newValue)
{
	//отслеживаем изменение параметров
	if (parameterID.equalsIgnoreCase("gain")) {
		gainProcessor.setGainDecibels(newValue);
	}

	if (parameterID.equalsIgnoreCase("pan")) {
		panProcessor.setPan(newValue);
	}
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new XY_PadAudioProcessor();
}
