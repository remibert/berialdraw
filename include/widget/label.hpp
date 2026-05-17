#pragma once
namespace berialdraw
{
	/** The Label class is used to display non-interactive text in the user interface. 
	It supports styling options for fonts, colors, and alignment.*/
	class Label: public Widget, public TextStyle
	{
	/** @image html sample_label_1.svg "example" width=200px height=200px
	@example sample_label.cpp*/
	public:
		/** Create widget */
		Label(Widget * parent);

		/** Destroy widget */
		virtual ~Label();

		/** Serialize the content of widget into json */
		virtual void serialize(JsonIterator & it);

		/** Unserialize the content of widget from json */
		virtual void unserialize(JsonIterator & it);

		/** Get the style cascade mode for this widget */
		virtual StyleCascadeMode style_cascade_mode() const override;

		/** Copy all styles of the label */
		void copy(const Label & label);

		/** Copy all styles of the label */
		void copy(const Label * label);

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
		Label& operator=(const Label& other) = delete;
		
		/** Paint on screen memory the content of this widget */
		virtual void paint(const Region & parent_region);

		/** Place all widget in area */
		virtual void place(const Area & area, bool in_layout);

		/** Return the size of content without marges */
		virtual Size content_size();

		/** Get the widget hovered */
		virtual Widget * hovered(const Region & parent_region, const Point & position);

		TextBox m_text_box;
/// @endcond
	};
}
