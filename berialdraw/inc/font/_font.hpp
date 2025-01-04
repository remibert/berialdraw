#ifndef _Font_h_INCLUDED
#define _Font_h_INCLUDED
/// @cond DOXYGEN_IGNORE
namespace berialdraw
{
	/** The _Font class represents a font that can be used for rendering text.
	It encapsulates the font's properties and provides access to the FreeType library. */
	class _Font
	{
	public:
		/** Constructs a _Font object with the specified filename and font.
		@param filename The name of the font file to be loaded.
		@param font A pointer to the Font object that contains glyph information. */
		_Font(FontFacePtr & face, Font * font):
			m_fontface(face),
			m_glyphs(font)
		{
			memset(&m_ft_size, 0, sizeof(m_ft_size));
		}

		/** 	Destructor for the _Font class.
		Cleans up any resources used by the _Font object. */
		~_Font()
		{
		}

		Size              m_pixel_size = {10<<1,10<<1}; ///< The size of the font in pixels.
		Size              m_real_size = {10<<1,10<<1}; ///< The real size of the font in pixels.
		Coord             m_baseline = 0;         ///< The baseline position for the font.
		Coord             m_angle = 0;            ///< The angle of the font for rendering.
		Glyphs            m_glyphs;               ///< The list of glyphs associated with the font.
		
		FontFacePtr      m_fontface;
		FT_Size          m_ft_size; 

		/** Returns the handle to the FreeType library. 
		@return A handle to the FreeType library. */
		static FT_Library load_ft_lib();
		static void unload_ft_lib();
		static FT_Library m_library; ///< The static FreeType library instance.
		static FTC_Manager m_manager;
	};
}
/// @endcond
#endif

