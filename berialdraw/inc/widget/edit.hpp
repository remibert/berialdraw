#pragma once
namespace berialdraw
{
	/** The Edit class allows users to enter and modify text. 
	It supports features like placeholder text, validation, and event hooks for input changes. */
	class Edit: public Widget, public TextStyle, public BorderStyle, public EditStyle, public Entry
	{
	/** @image html sample_edit_1.svg "example" width=200px height=200px
	@example sample_edit.cpp*/
	public:
		/** Create widget */
		Edit(Widget * parent);

		/** Destroy widget */
		virtual ~Edit();

		/** Serialize the content of widget into json */
		virtual void serialize(JsonIterator & it);

		/** Unserialize the content of widget from json */
		virtual void unserialize(JsonIterator & it);

		/** Copy all styles of the edit */
		void copy(const Edit & edit);

		/** Copy all styles of the edit */
		void copy(const Edit * edit);

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
#endif
	protected:
/// @cond DOXYGEN_IGNORE
		/** Remove operator = */
		Edit& operator=(const Edit& other) = delete;
		
		/** Call back on key */
		void on_key(Widget * widget, const KeyEvent & evt);

		/** Call back on selection */
		void on_select(Widget * widget, const SelectEvent & evt);

		/** Call back on click */
		void on_click(Widget * widget, const ClickEvent & evt);

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
		Point m_cursor_shift;
		TextBox m_text_box;
		uint32_t m_select_from = UINT32_MAX;
/// @endcond 
	};
}
