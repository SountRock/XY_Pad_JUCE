/*
  ==============================================================================

    XYPad.cpp
    Created: 27 Feb 2023 11:54:01pm
    Author:  LKDat

  ==============================================================================
*/

#include "XYPad.h"
namespace Gui {

	//XYPad Thumb section
	XYPad::Thumb::Thumb()
	{
		container.setMinimumOnscreenAmounts(thumbSize, thumbSize, thumbSize, thumbSize); //устанавливает сколько пикселей объета должно отсаваться
		//в пределех контейнера при упоре в него

	}
	void XYPad::Thumb::paint(Graphics& g)
	{
		g.setColour(Colours::whitesmoke);
		g.fillEllipse(getLocalBounds().toFloat());
	}

	void XYPad::Thumb::mouseDown(const MouseEvent& event)
	{
		dragger.startDraggingComponent(this, event);
	}

	void XYPad::Thumb::mouseDrag(const MouseEvent& event)
	{
		dragger.dragComponent(this, event, &container);
		if (moveCallback) {
			moveCallback(getPosition().toDouble());
		}
	}
	
	//XYPad section
	XYPad::XYPad()
	{
		addAndMakeVisible(thumb);
		thumb.moveCallback = [&](Point<double> position) {
			const std::lock_guard<std::mutex> lock(vectorMutex);
			const auto bounds = getLocalBounds().toDouble();
			const auto withPhase = static_cast<double>(thumbSize);
				for (auto* slider : xSliders) {
				slider->setValue(jmap(position.getX(), 0.0, bounds.getWidth() - withPhase, slider->getMinimum(), slider->getMaximum()));
			    }
				for (auto* slider : ySliders) {
					slider->setValue(jmap(position.getY(), bounds.getHeight() - withPhase, 0.0, slider->getMinimum(), slider->getMaximum()));
				}
		};
	}
	void XYPad::resized()
	{
		thumb.setBounds(getLocalBounds().withSizeKeepingCentre(thumbSize, thumbSize));
		if (!xSliders.empty()) {
			sliderValueChanged(xSliders[0]);
		}
		if (!ySliders.empty()) {
			sliderValueChanged(ySliders[0]);
		}

	}
	void XYPad::paint(Graphics& g)
	{
		g.setColour(Colours::darkslateblue);
		g.fillRoundedRectangle(getLocalBounds().toFloat(), 10.f);
	}
	void XYPad::registerSilder(Slider* slider, Axis axis)
	{
		slider->addListener(this);
		//Lock
		const std::lock_guard<std::mutex> lock(vectorMutex);
		if (axis == Axis::X) {
			xSliders.push_back(slider);
		}

		if (axis == Axis::Y) {
			ySliders.push_back(slider);
		}
	}
	void XYPad::deregisterSilder(Slider* slider)
	{
		slider->removeListener(this);
		//Lock
		const std::lock_guard<std::mutex> lock(vectorMutex);
		//remove/erase idiom
		xSliders.erase(std::remove(xSliders.begin(), xSliders.end(), slider), xSliders.end()); //удаляем лишнее из xSliders
		ySliders.erase(std::remove(ySliders.begin(), ySliders.end(), slider), ySliders.end());
	}
	void XYPad::sliderValueChanged(Slider* slider)
	{
		if (thumb.isMouseOverOrDragging(false)) {
			return;
		}

		const auto isXAxisSlider = std::find(xSliders.begin(), xSliders.end(), slider) != xSliders.end();
		const auto bounds = getLocalBounds().toDouble();
		const auto withPhase = static_cast<double>(thumbSize);
		if (isXAxisSlider) {
			thumb.setTopLeftPosition(
				jmap(slider->getValue(), slider->getMinimum(), slider->getMaximum(), 0.0, bounds.getWidth() - withPhase), 
				thumb.getY());
			//x - меняется, y - нет
		}
		else {
			thumb.setTopLeftPosition(
				thumb.getX(), 
				jmap(slider->getValue(), slider->getMinimum(), slider->getMaximum(), bounds.getHeight() - withPhase, 0.0));
			//y - меняется, x - нет
		}
		repaint();
	}
}