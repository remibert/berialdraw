#ifndef Button_hpp_INCLUDED
#define Button_hpp_INCLUDED
namespace berialdraw
{
	/** The Button class represents a UI element that users can click to trigger actions. 
	Buttons can display text, and support customizable styles and event callbacks. */
	class Button: public Widget, public TextStyle, public BorderStyle
	{
	/** @image html sample_buton_1.svg "example" width=200px height=200px
	@example sample_button.cpp
	@verbinclude resources/styles/pearl/button.json */
	public:
		/** Create widget */
		Button(Widget * parent);

		/** Destroy widget */
		virtual ~Button(); 

		/** Serialize the content of widget into json */
		virtual void serialize(JsonIterator & it);

		/** Unserialize the content of widget from json */
		virtual void unserialize(JsonIterator & it);

#ifdef _DEBUG
	public:
		static void test();
		static void test1();
		static void test2();
		static void test3();
		static void test4();
		static void test5();
		static void test6();
		static void test7();
		static void test8();
		static void test9();
		static void test10();
		static void test11();
#endif
	protected:
/// @cond DOXYGEN_IGNORE
		/** Indicates if the window must be refreshed */
		virtual bool dirty();

		/** Indicates if the paint is required for this current */
		virtual bool paintable(const Region & parent_region) {return true;}

		/** Paint on screen memory the content of this widget */
		virtual void paint(const Region & parent_region);

		/** Place all widget in area */
		virtual void place(const Area & area, bool in_layout);

		/** Return the size of content without marges */
		virtual Size content_size();

		/** Get the widget hovered */
		virtual Widget * hovered(const Region & parent_region, const Point & position);

		Area m_text_foreclip;
		Area m_text_backclip;
		TextBox m_text_box;
/// @endcond 
	};
}
#endif