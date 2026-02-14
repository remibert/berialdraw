#pragma once
namespace berialdraw
{
	/** The BorderStyle class specifies the properties of a widget's border, including thickness, color, 
	and corner radius. It is used to create visually distinct borders around widgets. */
	class BorderStyle : public Style
	{
	public:
		/** Constructor */
		BorderStyle();

		/** Destructor */
		~BorderStyle(){}

		/** Serialize the content of widget into json */
		void serialize(JsonIterator & it);

		/** Unserialize the content of widget from json */
		void unserialize(JsonIterator & it);

		/** Set properties with another */
		void set(const BorderStyle & other);

		/** Copy operator */
		BorderStyle& operator=(const BorderStyle& other);

		/** Create new border style */
		static Style * create();


		/** Get the radius */
		inline Dim radius() const
		{
			return m_radius >> 6;
		}

		/** Get the radius with a precision of 64th of a pixel */
		inline Dim radius_() const
		{
			return m_radius;
		}
	
		/** Set the radius in pixels */
		void radius(Dim v);

		/** Set the radius with a precision of 64th of a pixel */
		void radius_(Dim v);


		/** Get the thickness */
		inline Dim thickness() const
		{
			return m_thickness >> 6;
		}

		/** Get the thickness with a precision of 64th of a pixel */
		inline Dim thickness_() const
		{
			return m_thickness;
		}
	
		/** Set the thickness in pixels */
		void thickness(Dim v);

		/** Set the thickness with a precision of 64th of a pixel */
		void thickness_(Dim v);


		/** Get the border color */
		inline uint32_t border_color() const
		{
			return m_border_color;
		}

		/** Set the border color */
		void border_color(uint32_t col);

		/** Set the border color with alpha */
		void border_color(uint32_t col, uint8_t alpha);


		/** Get the focus color */
		inline uint32_t focus_color() const
		{
			return m_focus_color;
		}

		/** Set the focus color */
		void focus_color(uint32_t col);

		/** Set the focus color with alpha */
		void focus_color(uint32_t col, uint8_t alpha);


		/** Get the focus gap between the widget and border */
		inline Dim focus_gap() const
		{
			return m_focus_gap >> 6;
		}

		/** Set the focus gap between the widget and border in pixels */
		void focus_gap(Dim v);


		/** Get the focus thickness (16 pixels max)*/
		inline Dim focus_thickness() const
		{
			return m_focus_thickness;
		}

		/** Set the focus thickness in pixels (16 pixels max)*/
		void focus_thickness(Dim v);

	protected:
/// @cond DOXYGEN_IGNORE
		uint32_t m_border_color  = 0xFF7F7F7F;
		uint32_t m_focus_color   = 0xFF7F7F7F;
		Dim m_thickness = 0;
		Dim m_radius = 0;
		uint16_t m_focus_gap = 0;
		uint16_t m_focus_thickness = 0;
/// @endcond 
	};
}
