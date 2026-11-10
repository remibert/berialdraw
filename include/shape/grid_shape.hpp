#pragma once
namespace berialdraw
{
	/** Draw a regular grid on a Canvas. Rows and columns represent cell counts. */
	class GridShape : public Shape
	{
	public:
		/** Create a grid shape and add it to the canvas */
		GridShape(Canvas * canvas);

		/** Get the number of rows */
		inline Dim rows() const
		{
			return m_rows;
		}

		/** Set the number of rows */
		void rows(Dim count);

		/** Get the number of columns */
		inline Dim columns() const
		{
			return m_columns;
		}

		/** Set the number of columns */
		void columns(Dim count);

		/** Get or set the grid line color */
		inline uint32_t grid_color() const { return m_grid_style.grid_color(); }
		inline void grid_color(uint32_t color) { m_grid_style.grid_color(color); }
		inline void grid_color(uint32_t color, uint8_t alpha) { m_grid_style.grid_color(color, alpha); }

		/** Get or set grid line visibility */
		inline bool grid_visible() const { return m_grid_style.grid_visible(); }
		inline void grid_visible(bool visible) { m_grid_style.grid_visible(visible); }

		/** Get or set grid line thickness */
		inline Dim horizontal_thickness() const { return m_grid_style.horizontal_thickness(); }
		inline Dim horizontal_thickness_q6() const { return m_grid_style.horizontal_thickness_q6(); }
		inline void horizontal_thickness(Dim thickness) { m_grid_style.horizontal_thickness(thickness); }
		inline void horizontal_thickness_q6(Dim thickness) { m_grid_style.horizontal_thickness_q6(thickness); }
		inline Dim vertical_thickness() const { return m_grid_style.vertical_thickness(); }
		inline Dim vertical_thickness_q6() const { return m_grid_style.vertical_thickness_q6(); }
		inline void vertical_thickness(Dim thickness) { m_grid_style.vertical_thickness(thickness); }
		inline void vertical_thickness_q6(Dim thickness) { m_grid_style.vertical_thickness_q6(thickness); }

		/** Paint the shape */
		virtual void paint(const Point & shift) override;

		/** Get the grid outline */
		virtual const Polygon & polygon() const override
		{
			return m_polygon;
		}

#ifdef _DEBUG
		static void test();
#endif

	protected:
/// @cond DOXYGEN_IGNORE
		GridShape& operator=(const GridShape& other) = delete;
		Polygon m_polygon;
		GridStyle m_grid_style;
		Dim m_rows = 0;
		Dim m_columns = 0;
/// @endcond
	};
}