#pragma once
/// @cond DOXYGEN_IGNORE

namespace berialdraw
{
	class Font;
	class VectorImage;
	class Glyph;

	/** Base glyph interface and shared metrics */
	class Glyph
	{
	public:
		Glyph(Font * font);

		virtual ~Glyph();

		/** Draw character
		@param position The position to draw the character
		@param center The center point of the character
		@param margin The margin around the character
		@param angle The angle to draw the character
		@param color The color of the character */
		virtual void paint(Point & position, const Point & center, const Margin & margin, Coord angle, uint32_t color) = 0;

		/** Current character */
		wchar_t m_character = 0;

		/** Advance direction */
		int32_t m_advancex = 0;
		int32_t m_advancey = 0;

		/** Angle of character */
		Coord m_angle = 0;

		/** Load order, used for cache eviction */
		uint32_t m_stamp = 0;

	protected:
		FT_Face ft_face() const;

		/** Handle on font loaded */
		Font* m_font = 0;
	};

	/** FreeType-backed glyph */
	class FontGlyph : public Glyph
	{
	public:
		FontGlyph(Font * font);
		~FontGlyph() override;

		bool load(wchar_t character, Coord angle);
		void paint(Point & position, const Point & center, const Margin & margin, Coord angle, uint32_t color) override;

		/** Face */
		FT_GlyphSlot m_slot = 0;

		/** Glyph of character */
		FT_Glyph m_glyph = 0;

		/** Bitmap glyph of character */
		FT_Glyph m_bitmap_glyph = 0;

		/** Bitmap of glyph */
		FT_Bitmap* m_bitmap = nullptr;

		/** Character size */
		int32_t m_width = 0;

		int32_t m_baselinex = 0;
		int32_t m_baseliney = 0;

	};

	/** VectorImage-backed glyph definition */
	class VectorGlyph : public Glyph
	{
	public:
		VectorGlyph(Font * font, wchar_t character, VectorImage * image);
		~VectorGlyph() override;

		bool valid() const;
		void paint(Point & position, const Point & center, const Margin & margin, Coord angle, uint32_t color) override;

	protected:
		VectorImage * m_image = 0;
		Dim m_zoom = 0;
		Coord m_height = 0;
		Coord m_draw_width = 0;
		bool m_valid = false;
	};
}
/// @endcond
