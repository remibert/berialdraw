#pragma once
namespace berialdraw
{

	/** The CommonStyle class provides a set of general styling properties, such as background color, 
	and margin, that can be applied across multiple widgets for consistent design. */
	class CommonStyle : public Style
	{
	public:
		/** Constructor */
		CommonStyle();
	
		/** Destructor */	
		~CommonStyle(){}

		/** Copy operator */
		CommonStyle& operator=(const CommonStyle& other);


		/** Serialize the content of widget into json */
		void serialize(JsonIterator & it);

		/** Unserialize the content of widget from json */
		void unserialize(JsonIterator & it);

		/** Set properties with another */
		void set(const CommonStyle & other);

		/** Create new shape style */
		static Style * create();

		/** Get the margin */
		const Margin & margin() const;
		
		/** Set the margin */
		void margin(const Margin & m);
		
		/** Set the margin in pixels */
		void margin(Dim top, Dim left, Dim bottom, Dim right);
		
		/** Set the margin with a precision of 64th of a pixel */
		void margin_(Dim top, Dim left, Dim bottom, Dim right);

		/** Set the margin */
		void margin(Dim value);

		/** Set the margin */
		void margin(Dim horizontal, Dim vertical);

		/** Set the margin top in pixels */
		void margin_top(Dim top);

		/** Set the margin left in pixels */
		void margin_left(Dim left);

		/** Set the margin bottom in pixels */
		void margin_bottom(Dim bottom);

		/** Set the margin right in pixels */
		void margin_right(Dim right);
		

		/** Get the angle */
		Coord angle() const;

		/** Get the angle with a precision of 64th of a degres */
		Coord angle_() const;
	
		/** Set the angle in pixels */
		void angle(Coord v);

		/** Set the angle with a precision of 64th of a degres */
		void angle_(Coord v);



		/** Get the center */
		const Point & center() const;
		
		/** Set the center */
		void center(const Point & s);

		/** Set the center with x and y in pixels */
		void center(Coord x, Coord y=0);

		/** Set the center with a precision of 64th of a pixel */
		void center_(Coord x, Coord y=0);


		/** Get the position */
		const Point & position() const;
		
		/** Set the position */
		void position(const Point & s);

		/** Set the position with x and y in pixels */
		void position(Coord x, Coord y=0);

		/** Set the position with a precision of 64th of a pixel */
		void position_(Coord x, Coord y=0);


		/** Get the color */
		uint32_t color() const;

		/** Set the color */
		void color(uint32_t col);

		/** Set the color with alpha */
		void color(uint32_t col, uint8_t alpha);

		/** Set the color */
		void color(Color col);

		/** Enlighten the back color
		@param level value added [-100..100] */
		void enlighten(int8_t level);

		/** Saturate the back color
		@param level value added [-100..100] */
		void saturate(int8_t level);

		/** Change to pastel the back color
		@param level value added [-100..100] */
		void to_pastel(int8_t level);

		/** Get the size */
		const Size & size() const;
		
		/** Set the size */
		void size(const Size & s);

		/** Set the size with width and height in pixels */
		void size(Dim w, Dim h=0);

		/** Set the size with a precision of 64th of a pixel */
		void size_(Dim w, Dim h=0);



		/** Get the align */
		Align align() const;

		/** Set the align */
		void align(Align v);


		/** Select the displayed border of the rectangle */
		void borders(uint16_t side);

		/** Gets the displayed border of the rectangle */
		uint16_t borders() const;


		/** Get the hidden widget state */
		bool hidden() const;

		/** Set the hidden widget state */
		void hidden(bool v);

#ifndef SWIG
/// @cond DOXYGEN_IGNORE
		/** Place the widget in absolute coordinates on the window */
		void place_absolutly(const Point & root, const Size & content, Area & foreclip, const Size & min_size, const Size & max_size);

		/** Place the widget in the layout cell */
		void place_in_layout(const Area & area, const Size & content, const Margin & margin, Extend extend, Area & foreclip, Align align);


	protected:
		Dim reduce(Dim  dimension, Dim reduction);

		Coord get_position(Coord position, Coord size, Coord marge_begin, Coord marge_end, Coord area, Align align);

		Dim adapt_size(Dim content, Dim size, Dim mini, Dim maxi);

		Margin m_margin = {0,0,0,0};
		Point m_center;
		Point m_position;
		Size m_size;
		Coord m_angle = 0;
		uint32_t m_color   = 0xFF000000;

		unsigned int m_align:4;
		unsigned int m_angle_modified:1;
		unsigned int m_geometry_modified:1;
		unsigned int m_hidden :1;
		int m_light:8;
		int m_saturation:8;
		Borders m_borders;
/// @endcond 
#endif
	}; 
}
