#pragma once

namespace berialdraw
{
	/** The List class provides a scrollable list container for displaying items with optional icons and arrows. It supports selection, enabled/disabled state, and menu-style arrows. */
	class List : public ScrollableContent
	{
	public:
		/** Create widget */
		List(Widget * parent);

		/** Destroy widget */
		virtual ~List();

		/** Create new list item */
		ListItem* new_item();

		/** Serialize the content of widget into json */
		virtual void serialize(JsonIterator & it) override;

		/** Unserialize the content of widget from json */
		virtual void unserialize(JsonIterator & it) override;

		/** Get the style cascade mode for this widget */
		virtual StyleCascadeMode style_cascade_mode() const override;

		/** Copy all styles of the list */
		void copy(const List& list);

		/** Copy all styles of the list */
		void copy(const List* list);


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

		/** Internal column container */
		Column * m_column;
/// @endcond
	};
}
