#ifndef Outline_hpp_INCLUDED
#define Outline_hpp_INCLUDED

struct FT_Outline_;

namespace berialdraw
{
	class Point;

	/** Class to create bezier shapes using the FreeType render engine */
	class Outline
	{
	public:
		/** Constructor */
		Outline();

		/** Copy constructor
		@param other Outline object to copy from */
		Outline(const Outline & other);

		/** Constructor from FT_Outline object
		@param outline FreeType outline object to initialize from */
		Outline(const FT_Outline & outline);

		/** Destructor */
		virtual ~Outline();

		/** Add a point to the outline
		@param p Point object to add */
		void add_point(const Point & p);

		/** Add a cubic bezier curve to the outline
		@param p Point object representing the curve control point */
		void add_cubic(const Point & p);

		/** Add a conic bezier curve to the outline
		@param p Point object representing the curve control point */
		void add_conic(const Point & p);

		/** Resize the FT_Outline buffer
		@param nb_points Number of points to resize the buffer to
		@param nb_contours Number of contours to resize the buffer to */
		void resize(uint32_t nb_points, uint32_t nb_contours);

		/** Select the next contour
		(This is required after each add operation, otherwise nothing will be rendered) */
		void next_contour();

		/** Clear the outline, removing all points */
		void clear();

		/** Append an existing outline to the current outline
		@param outline Outline object to append */
		void append(const Outline & outline);

		/** Get the FreeType outline object
		@return Reference to the FreeType FT_Outline object */
		FT_Outline & get();

		/** Set the zoom ratio for the polygon with a precision of 64th of a pixel 
		@param z zoom value */
		void zoom_(uint32_t z);

		/** Return the zoom ratio for the polygon with a precision of 64th of a pixel 
		@return zoom value */
		uint32_t zoom_();

		/** Set the FreeType outline object
		@param outline FreeType FT_Outline object to set */
		void set(const FT_Outline & outline);

		/** Maximum coordinate value for the outline */
		const int32_t MAX_COORD = (2048 << 6);

#ifdef _DEBUG
		static void test();
#endif

	protected:
/// @cond DOXYGEN_IGNORE
		/** Add a point to the FreeType FT_Outline object
		@param x X-coordinate of the point
		@param y Y-coordinate of the point
		@param tag Tag specifying the type of the point (move, line, conic, cubic) */
		void add(int32_t x, int32_t y, uint32_t tag);

		FT_Outline m_outline;               /**< FreeType outline object */
		uint32_t   m_nb_points      = 0;    /**< Number of points in the outline */
		uint32_t   m_nb_contours    = 0;    /**< Number of contours in the outline */
		bool       m_contour_opened = true; /**< Flag indicating if the contour is open */
		uint32_t   m_zoom           = 1<<6; /**< the zoom ratio for the polygon shifted by 6 bits */
/// @endcond
	};
}

#endif // Outline_hpp_INCLUDED
