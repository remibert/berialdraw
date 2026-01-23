#pragma once
namespace berialdraw
{
	class Glyph;

	/** Glyphs list management */
	class Glyphs
	{
	public:
		/** Constructor
		@param font Pointer to the font */
		Glyphs(Font * font);

		/** Destructor */
		virtual ~Glyphs();

		/** Clear the glyphs list */
		void clear();

		/** Search for a glyph by character
		@param character The character to search for
		@param angle The angle of the character
		@return Pointer to the glyph if found, otherwise nullptr */
		Glyph* search(wchar_t character, Coord angle);

		/** Load a glyph by character and angle
		@param character The character to load
		@param angle The angle of the character
		@return Pointer to the loaded glyph */
		Glyph* load(wchar_t character, Coord angle);

	protected:
/// @cond DOXYGEN_IGNORE
		Vector<Glyph*> m_glyphs; /**< List of glyphs */
		Font* m_font;            /**< Pointer to the font */
		Coord m_angle = 0;       /**< Angle for the glyphs */
/// @endcond
	};
}
