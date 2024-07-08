/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Component/XYPad.h"

//==============================================================================
/**
*/
class XY_PadAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    XY_PadAudioProcessorEditor (XY_PadAudioProcessor&);
    ~XY_PadAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
	using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    XY_PadAudioProcessor& audioProcessor;

	juce::Slider gainSlider { juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextBoxBelow };
	juce::Slider panSlider { juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextBoxBelow };
	SliderAttachment gainAttachment, panAttachment; //будет отражаться и в процессоре и в редакторе
	juce::Label gainLabel{ "gainLabel", "gain" };
	juce::Label panLabel{ "panLabel", "pan" };

	Gui::XYPad xyPad;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (XY_PadAudioProcessorEditor)
};
