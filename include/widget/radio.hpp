#pragma once
namespace berialdraw
{
	/** The Radio class represents a selectable radio button control used for mutually exclusive selections.
	Radio buttons are grouped by the group property, and only one can be selected at a time within the same group. */
	class Radio: public Widget, public TextStyle, public BorderStyle, public RadioStyle
	{
	/** @image html sample_radio_1.svg "example" width=200px height=200px
	@example sample_radio.cpp*/
	public:
		/** Create widget */
		Radio(Widget * parent);

		/** Destroy widget */
		virtual ~Radio();

		/** Serialize the content of widget into json */
		virtual void serialize(JsonIterator & it);

		/** Unserialize the content of widget from json */
		virtual void unserialize(JsonIterator & it);

		/** Copy all styles of the radio button */
		void copy(const Radio * radio);

		/** Copy all styles of the radio button */
		void copy(const Radio & radio);

		/** Return the size of content without margins */
		virtual Size content_size();

		/** Place all widget in area */
		virtual void place(const Area & area, bool in_layout);

#ifdef _DEBUG
		static void test();
		static void test1();
		static void test2();
		static void test3();
		static void test4();
#endif

	protected:
/// @cond DOXYGEN_IGNORE
		/** Remove operator = */
		Radio& operator=(const Radio& other) = delete;

		/** Call back on key */
		void on_key(Widget * widget, const KeyEvent & evt);

		/** Call back on click */
		void on_click(Widget * widget, const ClickEvent & evt);

		/** Deselect all radio siblings with the same group in the window */
		void deselect_all();

		/** Helper to deselect radios in the same group recursively */
		void deselect_radio(Widget * widget, const String & my_group);

		/** Paint on screen memory the content of this widget */
		virtual void paint(const Region & parent_region);

		/** Get the widget hovered */
		virtual Widget * hovered(const Region & parent_region, const Point & position);

		Area m_text_foreclip;
		Area m_text_backclip;
		Area m_radio_foreclip;
		TextBox m_text_box;
		Size m_text_size;
/// @endcond
	};
}
