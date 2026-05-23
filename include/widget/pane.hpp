#pragma once
namespace berialdraw
{
	/** The Pane class is a simple container for grouping and managing other widgets. */
	class Pane: public Widget, public BorderStyle
	{
	/** @image html sample_pane_1.svg "example" width=200px height=200px
	@example sample_pane.cpp*/
	public:
		/** Create widget */
		Pane(Widget * parent);

		/** Destroy widget */
		virtual ~Pane();

		/** Serialize the content of widget into json */
		virtual void serialize(JsonIterator & it) override;

		/** Unserialize the content of widget from json */
		virtual void unserialize(JsonIterator & it) override;

		/** Get the style cascade mode for this widget */
		virtual StyleCascadeMode style_cascade_mode() const override;

		/** Copy all styles of the pane */
		void copy(const Pane & pane);

		/** Copy all styles of the pane */
		void copy(const Pane * pane);

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
		Pane& operator=(const Pane& other) = delete;
		
		/** Return the size of content without marges */
		virtual Size content_size() override;

		/** Paint on screen memory the content of this widget */
		virtual void paint(const Region & parent_region) override;

		/** Place all widget in area */
		virtual void place(const Area & area, bool in_layout) override;
/// @endcond
	};
}
