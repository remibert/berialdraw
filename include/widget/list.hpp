#pragma once

namespace berialdraw
{
	/** The List class provides a scrollable list container for displaying items with optional icons and arrows. It supports selection, enabled/disabled state, and menu-style arrows. */
	class List : public ScrollableContent, public ListStyle, public BorderStyle
	{
	public:
		/** Create widget */
		List(Widget * parent);

		/** Destroy widget */
		virtual ~List();


		///** Add an item to the list */
		//uint16_t add(const String & text);

		///** Add an item to the list */
		//uint16_t add(const String & text, const String & leading, const String & trailing);

		/** Create new list item */
		ListItem* new_item();

		/** Serialize the content of widget into json */
		virtual void serialize(JsonIterator & it) override;

		/** Unserialize the content of widget from json */
		virtual void unserialize(JsonIterator & it) override;

		/** Get the style cascade mode for this widget */
		virtual StyleCascadeMode style_cascade_mode() const override;

		/** Return the size of content without marges */
		virtual Size content_size() override;

#ifdef _DEBUG
		static void test();
		static void test1();
		static void test2();
		static void test3();
		static void test4();
		static void test5();
		static void test6();
		static void test7();
#endif

	protected:
/// @cond DOXYGEN_IGNORE
		/** Remove operator = */
		List& operator=(const List& other) = delete;

		/** Paint on screen memory the content of this widget */
		virtual void paint(const Region & parent_region) override;

		/** Place all widget in area */
		virtual void place(const Area& area, bool in_layout) override;

		/** Internal column container */
		Column * m_column;
/// @endcond
	};
}
