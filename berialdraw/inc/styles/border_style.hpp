#ifndef BorderStyle_hpp_INCLUDED
#define BorderStyle_hpp_INCLUDED

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
		Dim radius() const;

		/** Get the radius with a precision of 64th of a pixel */
		Dim radius_() const;
	
		/** Set the radius in pixels */
		void radius(Dim v);

		/** Set the radius with a precision of 64th of a pixel */
		void radius_(Dim v);


		/** Get the thickness */
		Dim thickness() const;

		/** Get the thickness with a precision of 64th of a pixel */
		Dim thickness_() const;
	
		/** Set the thickness in pixels */
		void thickness(Dim v);

		/** Set the thickness with a precision of 64th of a pixel */
		void thickness_(Dim v);


		/** Get the border color */
		uint32_t border_color() const;

		/** Set the border color */
		void border_color(uint32_t col);

		/** Set the border color with alpha */
		void border_color(uint32_t col, uint8_t alpha);


		/** Get the focus color */
		uint32_t focus_color() const;

		/** Set the focus color */
		void focus_color(uint32_t col);

		/** Set the focus color with alpha */
		void focus_color(uint32_t col, uint8_t alpha);


		/** Get the focus gap between the widget and border */
		Dim focus_gap() const;

		/** Set the focus gap between the widget and border in pixels */
		void focus_gap(Dim v);


		/** Get the focus thickness (16 pixels max)*/
		Dim focus_thickness() const;

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
#endif
