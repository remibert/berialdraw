#pragma once
namespace berialdraw
{
	/** The Icon class is a specialized button that combines an image and a text label. 
	The image is displayed above the text, making it suitable for representing actions, features, 
	or items in a visually intuitive way. Icons can trigger actions like regular buttons while 
	providing a compact, user-friendly design. */
	class Icon: public Widget, public BorderStyle, public IconStyle, public TextStyle
	{
	/** @image html sample_icon_1.svg "example" width=200px height=200px
	@example sample_icon.cpp*/
	public:
		/** Create widget */
		Icon(Widget * parent);

		/** Destroy widget */
		virtual ~Icon();

		/** Serialize the content of widget into json */
		virtual void serialize(JsonIterator & it);

		/** Unserialize the content of widget from json */
		virtual void unserialize(JsonIterator & it);

		/** Return the size of content without marges */
		virtual Size content_size();

		/** Copy all styles of the icon */
		void copy(const Icon & icon);

		/** Copy all styles of the icon */
		void copy(const Icon * icon);

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
		Dim compute_zoom(Dim value, Dim zoom);

		/** Remove operator = */
		Icon& operator=(const Icon& other) = delete;

		/** Return the icon size */
		Size icon_size();

		/** Paint on screen memory the content of this widget */
		virtual void paint(const Region & parent_region);

		/** Place all widget in area */
		virtual void place(const Area & area, bool in_layout);

		/** Get the widget hovered */
		virtual Widget * hovered(const Region & parent_region, const Point & position);

		Area m_text_foreclip;
		Area m_icon_foreclip;
		Area m_text_backclip;
		TextBox m_text_box;
		Sketch * m_sketch;
/// @endcond
	};
}
