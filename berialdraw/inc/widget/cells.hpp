#pragma once
/// @cond DOXYGEN_IGNORE
namespace berialdraw
{
	class Cell
	{
	public:
		Dim m_mini = 0;
		Dim m_maxi = Size::MAX_SIZE;
		Dim m_marged = 0;
		Dim m_placed = 0;
		SizePolicy m_size_policy = SizePolicy::NORMAL_SIZE;
	};
	class Widget;
	class Cells
	{
	public:
		/** Constructor */
		Cells(Widget * widget=0);

		/** Destructor */
		virtual ~Cells();

		/** Create or recreate cells array 
		@param rows_count count of rows
		@param columns_count count of columns */
		void reallocate(Dim rows_count, Dim columns_count);

		/** Select the first widget according to its row and column
		@param row the row id
		@param column the column id 
		@return the widget handle found or null */
		Widget * first(Widget * widget, Dim row, Dim column);

		/** Select the next widget according to its row and column
		@param row the row id
		@param column the column id 
		@return the widget handle found or null */
		Widget * next(Widget * widget, Dim row, Dim column);

		/** Recompute the min and max size of rows and columns of the widget */
		Size calc_sizes(Widget * widget);

		/** Resize the cells according to the area size */
		Size resize(const Area & area);

		/** Places widgets according to the area position */
		void place(Widget *widget, const Area & area);

		/** Recalculates the boundaries based on content */
		void rebound(Widget * widget);

		/** Get the number of rows */
		Dim row_count() const;

		/** Get the number of columns */
		Dim column_count() const;

#ifdef _DEBUG
		static void test();
		static void test1();
		static void test2();
#endif

	protected:
/// @cond DOXYGEN_IGNORE
		Dim reduce_mini(Cell * cells, Dim count, Dim missing, Dim available);
		Dim resize_dim(Cell * cells, Dim count, Dim area_size, bool on_width);
		Dim reduce_marged(Cell * cells, Dim count, Dim missing);
		Dim flatten_marged(Cell * cells, Dim count, Dim area_size, Dim missing, Dim available);
	
		Cell * m_widths = 0;
		Cell * m_heights = 0;

		Size m_min_size = {0,0};
		Size m_max_size = {0,0};
		Size m_marged_size = {0,0};

		Dim m_rows_count = 0;
		Dim m_columns_count = 0;
/// @endcond
	};
}
/// @endcond 
