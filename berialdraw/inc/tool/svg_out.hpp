struct FT_Outline_;
typedef struct FT_Outline_ FT_Outline;

struct FT_Vector_;
typedef struct FT_Vector_ FT_Vector;

namespace berialdraw
{
	/** Class used to serialize bezier path in svg format */
	class SvgOut : public Exporter
	{
	public:
		/** Create svg export 
		@param filename Filename for the SVG export (optional) */
		SvgOut(const char * filename = 0);

		/** Destructor */
		virtual ~SvgOut();

		/** Set the svg size 
		@param size Size of the SVG */
		virtual void size(const Size & size);

		/** Add path in svg 
		@param outline Outline of the path
		@param color Color of the path */
		virtual void add_path(Outline & outline, uint32_t color);

		/** Add text in svg 
		@param position Position of the text
		@param size Size of the text
		@param center Center point of the text
		@param margin Margin around the text
		@param angle Angle of the text
		@param text Text to be added
		@param font Font of the text
		@param color Color of the text */
		virtual void add_text(const Point & position, const Size & size, const Point & center, const Margin & margin, Coord angle, const String & text, const Font & font, uint32_t color);

		/** Open clipping and group 
		@param position Position of the group
		@param size Size of the group */
		virtual void open_group(const Point & position, const Size & size);

		/** Close clipping and group */
		virtual void close_group();

		/** Get the content of svg generated 
		@return Content of the SVG as a String */
		const String & get();

		/** Close svg 
		@param crc32 CRC32 checksum for the SVG */
		virtual void close(uint32_t crc32);

	protected:
		/** Add text in svg 
		@param position Position of the text
		@param size Size of the text
		@param center Center point of the text
		@param margin Margin around the text
		@param angle Angle of the text
		@param text Text to be added
		@param font Font of the text
		@param color Color of the text */
		virtual void add_text_line(const Point & position, const Size & size, const Point & center, const Margin & margin, Coord angle, const String & text, const Font & font, uint32_t color);


		/** Add value to the string 
		@param str String to add value to
		@param value Value to be added */
		static void add_value(String & str, int32_t value);

		/** Add vector to the string 
		@param str String to add vector to
		@param vector Vector to be added */
		static void add_vector(String & str, const FT_Vector* vector);

		/** Move to a point 
		@param to Point to move to
		@param user User data
		@return Status of the operation */
		static int move_to(const FT_Vector* to, void* user);

		/** Draw a line to a point 
		@param to Point to draw line to
		@param user User data
		@return Status of the operation */
		static int line_to(const FT_Vector* to, void* user);

		/** Draw a conic curve to a point 
		@param control Control point for the curve
		@param to Point to draw curve to
		@param user User data
		@return Status of the operation */
		static int conic_to(const FT_Vector* control, const FT_Vector* to, void* user);

		/** Draw a cubic curve to a point 
		@param control1 First control point for the curve
		@param control2 Second control point for the curve
		@param to Point to draw curve to
		@param user User data
		@return Status of the operation */
		static int cubic_to(const FT_Vector* control1, const FT_Vector* control2, const FT_Vector* to, void* user);

	protected:
/// @cond DOXYGEN_IGNORE
		String  m_content;        /**< Content of the SVG */
		bool    m_closed = false; /**< Flag to indicate if SVG is closed */
		String  m_filename;       /**< Filename for the SVG export */
		int32_t m_id = 0;         /**< ID counter for elements in the SVG */
/// @endcond
	};
}
