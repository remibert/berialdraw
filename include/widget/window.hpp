#pragma once
namespace berialdraw
{
	/** The Window class represents a top-level container for UI elements. 
	It provides a frame, title, and close/minimize controls, serving as the main interface for user interaction. */
	class Window: public Widget
	{
	/** @image html sample_window_1.svg "example" width=200px height=200px
	@example sample_window.cpp*/
	public:
/// @cond DOXYGEN_IGNORE
		friend Desktop;
/// @endcond

		/** Create widget */
		Window();

		/** Destroy widget */
		virtual ~Window();

		/** Places the specified window at the top of its parents canvas stack, above all of its sibling window. */
		virtual void back();

		/** Places the specified window at the bottom of its parents canvas stack, below all of its sibling window.*/
		virtual void front();

		/** Serialize the content of widget into json */
		virtual void serialize(JsonIterator & it);

		/** Unserialize the content of widget from json */
		virtual void unserialize(JsonIterator & it);

		/** Copy all styles of the window */
		void copy(const Window & window);

	//protected:
		/** Allocate the window */
		void* operator new(size_t size);
	public:


		/** Copy all styles of the window */
		void copy(const Window * window);

/** @internal 
{
*/
		/** Remove the focus of the widget because it will be destroyed */
		void remove_focus(Widget * widget);

		/** Force flow replacement */
		void force_flow_replacement();

		/** Flow replacement detected */
		bool flow_replacement();

#ifdef _DEBUG
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
#endif
	protected:
/// @cond DOXYGEN_IGNORE
		/** Return the size of content without marges */
		virtual Size content_size();

		/** Treat the click on widget, used to change the focus */
		void on_click_widget(Widget * widget, const ClickEvent & evt);

		/** Treat the select widget, used to change the focus */
		void on_select_widget(Widget * widget, const SelectEvent & evt);

		/** Treat the key to change the focused widget */
		void on_key_focus(Widget * widget, const KeyEvent & evt);

		/** Get the widget hovered */
		virtual Widget * hovered(const Region & parent_region, const Point & position);

		/** Indicates if the window is allocated with a new or create on the stack */
		bool is_allocated();

		/** Place all widget in area */
		virtual void place();

		/** Paint on screen memory the content of this widget */
		virtual void paint(const Region & parent_region);

		Window(const Window & window) = delete;
		Window& operator=(const Window&) = delete;

		unsigned int m_allocated:1;
		Widget * m_with_focus = 0;
/// @endcond
/// }
	};
}
