#pragma once
namespace berialdraw
{
	/** Manage all fonts TTF file */
	class FontFace
	{
	public:
		/** Constructor that opens a font file.
		    @param fontfilename The filename of the font to open. Default is an empty string. */
		FontFace(const String & fontfilename="");

		/** Destructor that destroys the font instance */
		~FontFace();

		/** Get the family of the font
		@return The family name of the font */
		const String & familly() const;

		/** Get the style of the font
		@return The style of the font */
		enum Font::Style style();

		/** Get the filename of the font
		@return The filename of the font */
		const String & filename();

		/** Indicates is this font is monospaced 
		@return true if monospaced else return false */
		bool is_monospaced() const;

		/** Get font face true type */
		FT_Face face();

	protected:
/// @cond DOXYGEN_IGNORE
		FT_Face           m_face = 0;             ///< The FreeType face object for the font.
		String            m_familly;              ///< The family name of the font.
		String            m_filename;             ///< The filename of the font file.
/// @endcond
	};
}
