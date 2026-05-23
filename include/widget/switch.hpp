#pragma once
namespace berialdraw
{
	/** The Switch class represents a binary on/off control, similar to a toggle button. 
	It is commonly used for settings or preferences. */
	class Switch: public Widget, public TextStyle, public BorderStyle, public SwitchStyle
	{
	/** @image html sample_switch_1.svg "example" width=200px height=200px
	@example sample_switch.cpp*/
	public:
		/** Create widget */
		Switch(Widget * parent);

		/** Destroy widget */
		virtual ~Switch();

		/** Serialize the content of widget into json */
		virtual void serialize(JsonIterator & it) override;

		/** Unserialize the content of widget from json */
		virtual void unserialize(JsonIterator & it) override;

		/** Get the style cascade mode for this widget */
		virtual StyleCascadeMode style_cascade_mode() const override;

		/** Copy all styles of the switch */
		void copy(const Switch & switch_);

		/** Copy all styles of the switch */
		void copy(const Switch * switch_);

		/** Return the size of content without margins */
		virtual Size content_size() override;

		/** Place all widget in area */
		virtual void place(const Area & area, bool in_layout) override;

#ifdef _DEBUG
		static void test();
		static void test1();
		static void test2();
		static void test3();
		static void test4();
		static void test5();
		static void test6();
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
		virtual void paint(const Region & parent_region) override;

		/** Paint the switch on screen memory the content of this widget */
		virtual void paint_switch(Region & parent_region);

		/** Get the widget hovered */
		virtual Widget * hovered(const Region & parent_region, const Point & position) override;

		Area m_text_foreclip;
		Area m_text_backclip;
		Area m_switch_foreclip;
		TextBox m_text_box;
		Size m_text_size;
/// @endcond
	};
}