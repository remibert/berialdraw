#ifndef Switch_hpp_INCLUDED
#define Switch_hpp_INCLUDED
namespace berialdraw
{
	/** The Switch class represents a binary on/off control, similar to a toggle button. 
	It is commonly used for settings or preferences. */
	class Switch: public Widget, public BorderStyle, public SwitchStyle
	{
	/** @image html sample_switch_1.svg "example" width=200px height=200px
	@example sample_switch.cpp*/
	public:
		/** Create widget */
		Switch(Widget * parent);

		/** Destroy widget */
		virtual ~Switch();

		/** Serialize the content of widget into json */
		virtual void serialize(JsonIterator & it);

		/** Unserialize the content of widget from json */
		virtual void unserialize(JsonIterator & it);

		/** Return the size of content without marges */
		virtual Size content_size();

		/** Copy all styles of the switchar */
		void copy(const Switch & switch_);

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
		Switch& operator=(const Switch& other) = delete;

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
#endif
