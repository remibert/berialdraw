#pragma once
namespace berialdraw
{
	/** The ProgressBar class displays a graphical bar that indicates a task's completion percentage. 
	It supports both determinate and indeterminate modes. */
	class ProgressBar: public Widget, public BorderStyle, public ProgressBarStyle
	{
	/** @image html sample_progress_1.svg "example" width=200px height=200px
	@example sample_progress_bar.cpp*/
	public:
		/** Create widget */
		ProgressBar(Widget * parent);

		/** Destroy widget */
		virtual ~ProgressBar();

		/** Serialize the content of widget into json */
		virtual void serialize(JsonIterator & it);

		/** Unserialize the content of widget from json */
		virtual void unserialize(JsonIterator & it);

		/** Return the size of content without marges */
		virtual Size content_size();

		/** Copy all styles of the progress_bar */
		void copy(const ProgressBar & progress_bar);
	
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
		ProgressBar& operator=(const ProgressBar& other) = delete;
		
		void draw_track();
		Dim get_bar_length(Dim width);

		/** Paint on screen memory the content of this widget */
		virtual void paint(const Region & parent_region);

		/** Place all widget in area */
		virtual void place(const Area & area, bool in_layout);

		/** Get the widget hovered */
		virtual Widget * hovered(const Region & parent_region, const Point & position);

/// @endcond

	};
}
