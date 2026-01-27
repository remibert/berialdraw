#pragma once
namespace berialdraw
{
	/** The Checkbox class represents a selectable checkbox control used to toggle between checked and unchecked states.
	It is commonly used for boolean selections or multiple choice options. */
	class Checkbox: public Widget, public BorderStyle, public CheckboxStyle
	{
	/** @image html sample_checkbox_1.svg "example" width=200px height=200px
	@example sample_checkbox.cpp*/
	public:
		/** Create widget */
		Checkbox(Widget * parent);

		/** Destroy widget */
		virtual ~Checkbox();

		/** Serialize the content of widget into json */
		virtual void serialize(JsonIterator & it);

		/** Unserialize the content of widget from json */
		virtual void unserialize(JsonIterator & it);

		/** Return the size of content without marges */
		virtual Size content_size();

		/** Copy all styles of the checkbox */
		void copy(const Checkbox & checkbox_);

		/** Copy all styles of the checkbox */
		void copy(const Checkbox * checkbox_);
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
		Checkbox& operator=(const Checkbox& other) = delete;

		/** Call back on key */
		void on_key(Widget * widget, const KeyEvent & evt);

		/** Call back on click */
		void on_click(Widget * widget, const ClickEvent & evt);

		/** Paint on screen memory the content of this widget */
		virtual void paint(const Region & parent_region);

		/** Place all widget in area */
		virtual void place(const Area & area, bool in_layout);

		/** Get the widget hovered */
		virtual Widget * hovered(const Region & parent_region, const Point & position);

/// @endcond
	};
}
