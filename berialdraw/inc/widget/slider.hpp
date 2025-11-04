#pragma once
namespace berialdraw
{
	/** The Slider class allows users to select a value from a predefined range by dragging a handle. 
	It is commonly used for settings like volume or brightness control and supports horizontal and vertical orientations. */
	class Slider: public Widget, public BorderStyle, public SliderStyle
	{
	/** @image html sample_slider_1.svg "example" width=200px height=200px
	@example sample_slider.cpp*/
	public:
		/** Create widget */
		Slider(Widget * parent);

		/** Destroy widget */
		virtual ~Slider();

		/** Serialize the content of widget into json */
		virtual void serialize(JsonIterator & it);

		/** Unserialize the content of widget from json */
		virtual void unserialize(JsonIterator & it);

		/** Return the size of content without marges */
		virtual Size content_size();

		/** Copy all styles of the slider */
		void copy(const Slider & slider);

#ifndef SWIG
		/** Copy all styles of the slider */
		void copy(const Slider * slider);
#ifdef _DEBUG
		static void test();
		static void test1();
		static void test2();
		static void test3();
		static void test4();
		static void test5();
#endif
	protected:
/// @cond DOXYGEN_IGNORE
		/** Remove operator = */
		Slider& operator=(const Slider& other) = delete;

		void draw_track();
		Dim get_location(Dim width);

		/** Call back on key */
		void on_key(Widget * widget, const KeyEvent & evt);

		/** Call back on click */
		void on_click(Widget * widget, const ClickEvent & evt);

		/** Call back on select */
		void on_select(Widget * widget, const SelectEvent & evt);

		/** Touch handle */
		void touch_handle(const Point & touch_position);


		/** Paint on screen memory the content of this widget */
		virtual void paint(const Region & parent_region);

		/** Place all widget in area */
		virtual void place(const Area & area, bool in_layout);

		/** Get the widget hovered */
		virtual Widget * hovered(const Region & parent_region, const Point & position);

/// @endcond
#endif
	};
}
