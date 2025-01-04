#ifndef Glyph_hpp_INCLUDED
#define Glyph_hpp_INCLUDED
/// @cond DOXYGEN_IGNORE

namespace berialdraw
{
	class Font;
	class Glyph;

	/** Glyph informations */
	class Glyph
	{
	public:
		/** Constructor
		@param font Pointer to the font */
		Glyph(Font * font);

		/** Destructor */
		~Glyph();

		/** Load character
		@param character The character to load
		@param angle The angle of the character
		@return True if the character was successfully loaded */
		bool load(wchar_t character, Coord angle);

		/** Draw character
		@param position The position to draw the character
		@param center The center point of the character
		@param margin The margin around the character
		@param angle The angle to draw the character
		@param color The color of the character */
		void paint(Point & position, const Point & center, const Margin & margin, Coord angle, uint32_t color);

		/** Handle on font loaded */
		Font* m_font = 0;

		/** Face */
		FT_GlyphSlot m_slot = 0;

		/** Current character */
		wchar_t m_character = 0;

		/** Glyph of character */
		FT_Glyph m_glyph = 0;

		/** Bitmap of glyph */
		FT_Bitmap* m_bitmap = nullptr;

		/** Advance direction */
		int32_t m_advancex = 0;
		int32_t m_advancey = 0;

		/** Character size */
		int32_t m_width = 0;

		int32_t m_baselinex = 0;
		int32_t m_baseliney = 0;

		/** Angle of character */
		Coord m_angle = 0;
	};
}
/// @endcond

#endif // Glyph_hpp_INCLUDED
