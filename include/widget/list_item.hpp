#pragma once

namespace berialdraw
{
	/** The list item class */
	class ListItem : public Widget, public TextStyle, public BorderStyle, public ListItemStyle
	{
	/** @image html sample_item_1.svg "example" width=200px height=200px
	@example sample_item.cpp*/
	public:
		/** Create widget */
		ListItem(Widget * parent);

		/** Destroy widget */
		virtual ~ListItem();

		/** Serialize the content of widget into json */
		virtual void serialize(JsonIterator & it) override;

		/** Unserialize the content of widget from json */
		virtual void unserialize(JsonIterator & it) override;

		/** Get the style cascade mode for this widget */
		virtual StyleCascadeMode style_cascade_mode() const override;

		/** Copy all styles of the list item */
		void copy(const ListItem& list_item);

		/** Copy all styles of the list item */
		void copy(const ListItem * list_item);

#if defined(_DEBUG) 
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
		static void test12();
		static void test13();
		static void test14();
		static void test15();
		static void test16();
		static void test17();
		static void test18();
#endif

	protected:
/// @cond DOXYGEN_IGNORE
		/** Remove operator = */
		ListItem& operator=(const ListItem& other) = delete;

		/** Paint on screen memory the content of this widget */
		virtual void paint(const Region& parent_region) override;

		/** Place all widget in area */
		virtual void place(const Area& area, bool in_layout) override;

		/** Return the size of content without marges */
		virtual Size content_size() override;

		/** Get the widget hovered */
		virtual Widget* hovered(const Region& parent_region, const Point& position) override;

		Size select(String & text, std::unique_ptr<TextBox> & text_box, std::unique_ptr<Sketch> & sketch);


		Area m_text_foreclip;
		std::unique_ptr<Sketch>  m_text_sketch;
		std::unique_ptr<TextBox> m_text_box;

		Area m_leading_foreclip;
		std::unique_ptr<Sketch>  m_leading_sketch;
		std::unique_ptr<TextBox> m_leading_box;

		Area m_trailing_foreclip;
		std::unique_ptr<Sketch>  m_trailing_sketch;
		std::unique_ptr<TextBox> m_trailing_box;
/// @endcond
	};
}
