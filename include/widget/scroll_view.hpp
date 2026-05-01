#pragma once
namespace berialdraw
{
	/** The ScrollView class provides a scrollable container for content that exceeds the visible area. 
	It supports both vertical and horizontal scrolling. */
	class ScrollView: public ScrollableContent
	{
	/** @image html sample_scroll_1.svg "example" width=200px height=200px
	@example sample_scrollview.cpp*/
	public:
		/** Create widget */
		ScrollView(Widget * parent);

		/** Destroy widget */
		virtual ~ScrollView();

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
		static void test_create_window(Window & window);
#endif
	protected:
/// @cond DOXYGEN_IGNORE
		/** Remove operator = */
		ScrollView& operator=(const ScrollView& other) = delete;
/// @endcond
	};
}
