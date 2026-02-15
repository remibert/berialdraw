#pragma once
namespace berialdraw
{
	/** The TableViewStyle class specifies the appearance of table views, including grid colors, 
	thickness, header properties, and alternating row colors for improved readability. */
	class TableViewStyle : public Style
	{
	public:
		/** Constructor */
		TableViewStyle();

		/** Serialize the content of widget into json */
		void serialize(JsonIterator & it);

		/** Unserialize the content of widget from json */
		void unserialize(JsonIterator & it);

		/** Copy operator */
		TableViewStyle& operator=(const TableViewStyle& other);

		/** Set properties with another */
		void set(const TableViewStyle & other);

		/** Create new table view style */
		static Style * create();


		/** Get the grid color */
		inline uint32_t grid_color() const
		{
			return m_grid_color;
		}

		/** Set the grid color */
		void grid_color(uint32_t col);

		/** Set the grid color with alpha */
		void grid_color(uint32_t col, uint8_t alpha);


		/** Get the horizontal grid thickness in pixels */
		inline Dim horizontal_thickness() const
		{
			return m_horizontal_thickness >> 6;
		}

		/** Get the horizontal grid thickness with a precision of 64th of a pixel */
		inline Dim horizontal_thickness_() const
		{
			return m_horizontal_thickness;
		}

		/** Set the horizontal grid thickness in pixels */
		void horizontal_thickness(Dim v);

		/** Set the horizontal grid thickness with a precision of 64th of a pixel */
		void horizontal_thickness_(Dim v);


		/** Get the vertical grid thickness in pixels */
		inline Dim vertical_thickness() const
		{
			return m_vertical_thickness >> 6;
		}

		/** Get the vertical grid thickness with a precision of 64th of a pixel */
		inline Dim vertical_thickness_() const
		{
			return m_vertical_thickness;
		}

		/** Set the vertical grid thickness in pixels */
		void vertical_thickness(Dim v);

		/** Set the vertical grid thickness with a precision of 64th of a pixel */
		void vertical_thickness_(Dim v);


		/** Get the grid visibility state */
		inline bool grid_visible() const
		{
			return m_grid_visible;
		}

		/** Set the grid visibility state */
		void grid_visible(bool visible);


		/** Get the header visibility state */
		inline bool header_visible() const
		{
			return m_header_visible;
		}

		/** Set the header visibility state */
		void header_visible(bool visible);


		/** Get the header background color */
		inline uint32_t header_background_color() const
		{
			return m_header_background_color;
		}

		/** Set the header background color */
		void header_background_color(uint32_t col);

		/** Set the header background color with alpha */
		void header_background_color(uint32_t col, uint8_t alpha);


		/** Get the alternating row color 1 */
		inline uint32_t alternating_row_color1() const
		{
			return m_alternating_row_color1;
		}

		/** Set the alternating row color 1 */
		void alternating_row_color1(uint32_t col);

		/** Set the alternating row color 1 with alpha */
		void alternating_row_color1(uint32_t col, uint8_t alpha);


		/** Get the alternating row color 2 */
		inline uint32_t alternating_row_color2() const
		{
			return m_alternating_row_color2;
		}

		/** Set the alternating row color 2 */
		void alternating_row_color2(uint32_t col);

		/** Set the alternating row color 2 with alpha */
		void alternating_row_color2(uint32_t col, uint8_t alpha);

	protected:
/// @cond DOXYGEN_IGNORE
		uint32_t m_grid_color = 0xFF000000;
		Dim m_horizontal_thickness = 1 << 6;
		Dim m_vertical_thickness = 1 << 6;
		bool m_grid_visible = true;
		bool m_header_visible = true;
		uint32_t m_header_background_color = 0xFFCCCCCC;
		uint32_t m_alternating_row_color1 = 0xFFFFFFFF;
		uint32_t m_alternating_row_color2 = 0xFFF0F0F0;
/// @endcond
	};
}
