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

		/** Recompute the min and max size of rows and columns of the widget
		@param widget the parent widget */
		Size calc_sizes(Widget * widget);

		/** Resize the cells according to the area size
		@param area the area to fit cells in */
		Size resize(const Area & area);

		/** Places widgets according to the area position 
		@param widget the parent widget
		@param area the area to place widgets in */
		void place(Widget *widget, const Area & area);

		/** Recalculates the boundaries based on content */
		void rebound(Widget * widget);

		/** Get the number of rows */
		Dim row_count() const { return m_rows_count; }

		/** Get the number of columns */
		Dim column_count() const { return m_columns_count; }

		/** Get row positions (coordinates where each row starts) */
		const Dim* get_row_positions() const { return m_row_positions; }

		/** Get column positions (coordinates where each column starts) */
		const Dim* get_col_positions() const { return m_col_positions; }

		/** Set horizontal grid line thickness */
		void horizontal_line_thickness(Dim v) { m_horizontal_line_thickness = v; }

		/** Get horizontal grid line thickness */
		Dim horizontal_line_thickness() const { return m_horizontal_line_thickness; }

		/** Set vertical grid line thickness */
		void vertical_line_thickness(Dim v) { m_vertical_line_thickness = v; }

		/** Get vertical grid line thickness */
		Dim vertical_line_thickness() const { return m_vertical_line_thickness; }

		/** Get the placed height of a specific row */
		Dim row_height(Dim row) const { return (row < m_rows_count && m_heights) ? (m_heights[row].m_placed /*& 0xFFFFFFFC0*/) : 0; }

		/** Get the placed width of a specific column */
		Dim column_width(Dim col) const { return (col < m_columns_count && m_widths) ? (m_widths[col].m_placed/* & 0xFFFFFFFC0*/) : 0; }

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

		Dim* m_row_positions = 0;
		Dim* m_col_positions = 0;

		Size m_min_size = {0,0};
		Size m_max_size = {0,0};
		Size m_marged_size = {0,0};

		Dim m_rows_count = 0;
		Dim m_columns_count = 0;
		Dim m_horizontal_line_thickness = 0;
		Dim m_vertical_line_thickness = 0;
/// @endcond
	};
}
/// @endcond 
