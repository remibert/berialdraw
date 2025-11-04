#pragma once
namespace berialdraw
{
	class _Font;
	class Desktop;
	class CommonStyle;
	class Glyph;
	class FontFace;
	typedef SharedPtr<FontFace> FontFacePtr;


	/** Manage the font TTF file */
	class Font
	{
	public:
		friend class Glyphs;
		friend class Glyph;
		friend class Fonts;
		
		/** Font style enumeration */
		enum Style
		{
			UNDEFINED   = -1,           /**< Undefined style */
			NORMAL      = 0,            /**< Normal style */
			ITALIC      = (1<<0),       /**< Italic style */
			BOLD        = (1<<1),       /**< Bold style */
			BOLD_ITALIC = (1<<0)|(1<<1) /**< Bold and Italic style */
		};

		/** Constructor with freetype font face */
		Font(FontFacePtr & font_face, const Size & size);

		/** Destructor that destroys the font instance */
		virtual ~Font();

		/** Get the selected font size
		@return The pixel size of the font */
		const Size & pixel_size() const;

		/** Get the selected font size
		@return The real size of the font */
		const Size & real_size() const;

		/** Get the baseline of the font
		@return The baseline coordinate */
		Coord baseline() const;

		/** Get the baseline of the font with a precision of 64th of a pixel 
		@return The baseline coordinate */
		Coord baseline_() const;

		/** Indicates is this font is monospaced 
		@return true if monospaced else return false */
		bool is_monospaced() const;

		/** Draw text at a specified position, angle, and color
		@param text The text to draw
		@param position The position to draw the text
		@param center The center point of the text
		@param margin The margin around the text
		@param angle The angle to draw the text
		@param color The color of the text */
		void draw(const String & text, const Point & position, const Point & center, const Margin & margin, 
			Coord angle, uint32_t color);

		/** Get the size of the text when drawn
		@param text The text to measure
		@return The size of the text */
		Size text_size(const String & text);

		/** Get the size of the char when drawn at a specific angle
		@param character The character to measure
		@return The size of the text */
		Size char_size(wchar_t character);

		/** Get the family of the font
		@return The family name of the font */
		const String & familly() const;

		/** Get the style of the font
		@return The style of the font */
		enum Style style();

		/** Get the filename of the font
		@return The filename of the font */
		const String & filename();

		/** Clear the glyph cache */
		void clear();

#ifdef _DEBUG
		/** Test the font rendering on a desktop */
		static void test();
#endif

	protected:
#ifndef SWIG
/// @cond DOXYGEN_IGNORE
		/** Set font size in pixels
		@param size The size object containing width and height */
		void size(const Size & size);

		_Font * m_font = 0; /**< Internal font object */
/// @endcond
#endif
	};
}
