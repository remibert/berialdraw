#pragma once
namespace berialdraw
{
	/** The GridStyle class specifies the appearance of grid lines, including colors and thickness. */
	class GridStyle : public Style
	{
	public:
		/** Constructor */
		GridStyle();

		/** Serialize the content of widget into json */
		void serialize(JsonIterator & it) override;

		/** Unserialize the content of widget from json */
		void unserialize(JsonIterator & it) override;

		/** Copy operator */
		GridStyle& operator=(const GridStyle& other);

		/** Set properties with another */
		void set(const GridStyle & other);

		/** Create new grid style */
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


		/** Get the grid visibility state */
		inline bool grid_visible() const
		{
			return m_grid_visible;
		}

		/** Set the grid visibility state */
		void grid_visible(bool visible);


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

		/** Get the property name for this style */
		const char* property_name() const override { return "grid"; }

	protected:
/// @cond DOXYGEN_IGNORE
		uint32_t m_grid_color = 0xFF000000;
		uint16_t m_horizontal_thickness = 1 << 6;
		uint16_t m_vertical_thickness = 1 << 6;
		unsigned int m_grid_visible:1;
/// @endcond
	};
}
