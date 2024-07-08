/*
  ==============================================================================

    XYPad.h
    Created: 27 Feb 2023 11:54:01pm
    Author:  LKDat

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace Gui {
	class XYPad : public Component, Slider::Listener {
	public:
		enum class Axis { X, Y };

		//����� ��������
		class Thumb : public Component {
		public:
			Thumb();
			void paint(Graphics& g) override;
			void mouseDown(const MouseEvent& event) override;
			void mouseDrag(const MouseEvent& event) override;
			std::function<void(Point<double>)> moveCallback; //��������� ������ Thumb � XYPad ������������ ������� ��������
		private:
			ComponentDragger dragger; //��������� ������������� ������
			ComponentBoundsConstrainer container; //������ ��������� � �������� �������� ����� ��������� ������

			JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Thumb);
		};

		XYPad();
		void resized() override;
		void paint(Graphics& g) override;
		void registerSilder(Slider* slider, Axis axis);
		void deregisterSilder(Slider* slider);

	private:
		void sliderValueChanged(Slider* slider) override;

		std::vector<Slider*> xSliders, ySliders;
		Thumb thumb;

		std::mutex vectorMutex;

		static constexpr int thumbSize = 40;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(XYPad);
	};
}
