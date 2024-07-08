/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
XY_PadAudioProcessorEditor::XY_PadAudioProcessorEditor (XY_PadAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), gainAttachment(p.getAPVTS(), "gain", gainSlider), 
	panAttachment(p.getAPVTS(), "pan", panSlider)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
	addAndMakeVisible(gainSlider);
	addAndMakeVisible(panSlider);
	addAndMakeVisible(gainLabel);
	addAndMakeVisible(panLabel);
	addAndMakeVisible(xyPad);

	gainLabel.setJustificationType(Justification::centred);
	panLabel.setJustificationType(Justification::centred);
	gainLabel.attachToComponent(&gainSlider, false); //прикрепляем gainLabel к gainSlider. false - компонент стоит снизу объекта, а не слева
	panLabel.attachToComponent(&panSlider, false);

	xyPad.registerSilder(&gainSlider, Gui::XYPad::Axis::Y);
	xyPad.registerSilder(&panSlider, Gui::XYPad::Axis::X);

	setSize(500, 300);
	setResizable(true, true);
}

XY_PadAudioProcessorEditor::~XY_PadAudioProcessorEditor()
{
	xyPad.deregisterSilder(&gainSlider);
	xyPad.deregisterSilder(&panSlider);
}

//==============================================================================
void XY_PadAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void XY_PadAudioProcessorEditor::resized()
{
	const auto container = getLocalBounds().reduced(20);
	auto bounds = container;
	gainSlider.setBounds(bounds.removeFromLeft(container.proportionOfWidth(0.25f)));
	xyPad.setBounds(bounds.removeFromLeft(container.proportionOfWidth(0.5f)).reduced(20));
	panSlider.setBounds(bounds.removeFromRight(container.proportionOfWidth(0.25f)));
}
