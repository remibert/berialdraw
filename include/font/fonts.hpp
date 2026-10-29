#pragma once
namespace berialdraw
{
	class VectorImage;
	class Glyphs;
	class VectorGlyphDefinition;

	typedef berialdraw::SharedPtr<Font> FontPtr;

	/** Manage all fonts TTF file */
	class Fonts
	{
	public:
		/** Create fonts */
		Fonts();

		/** Destroy fonts */
		~Fonts();

		/** Load font and add into the list
		@param filename The filename of the font to load
		@return True if the font was successfully loaded */
		bool load(const String & filename);

		/** Unload font and remove from the list
		@param filename The filename of the font to unload */
		void unload(const String & filename);

		/** Clear glyphs cache */
		void clear_cach();

		/** Define or replace a vector glyph available to every font
		@param character Character mapped to the vector glyph
		@param image Vector image used to draw the glyph
		@return True if the glyph was successfully defined */
		bool define_vector_glyph(wchar_t character, const VectorImage & image);

		/** Define or replace a vector glyph from a file, available to every font
		@param character Character mapped to the vector glyph
		@param filename Vector image filename
		@return True if the file was loaded and the glyph was successfully defined */
		bool define_vector_glyph(wchar_t character, const String & filename);

		/** Remove a vector glyph definition available to every font
		@param character Character whose vector glyph definition is removed
		@return True if a definition was found and removed */
		bool remove_vector_glyph(wchar_t character);

		/** Remove all vector glyph definitions */
		void clear_vector_glyphs();

		/** Select the instance of font, if the font family not found return the first font, increase reference counter
		@param familly The family name of the font
		@param size size of font
		@param style The style of the font (default is Font::UNDEFINED)
		@return A shared pointer to the selected font */
		FontPtr select(const String & familly, const Size & size, enum Font::Style style=Font::UNDEFINED);

		/** Get default familly font name
		@return familly */
		const String & familly();

		/** Set default familly for font
		@param name familly font name */
		void familly(const String & name);

		/** Load all fonts from the selected directory 
		@param directory directory with fonts */
		bool load_directory(const String & directory);

	private:
		/** Find best font face by family name (or any family if nullptr) and style priority
		@param family_name The family name to search in (nullptr searches all families)
		@param style The style priority to match
		@return The best matching font face, or nullptr if none found */
		FontFacePtr find_best_font_face(const String * family_name, enum Font::Style style);

		/** Copy a registered vector glyph image for a per-font cache */
		VectorImage* copy_vector_glyph(wchar_t character);

		/** Find the first vector glyph not lower than the character */
		uint32_t lower_bound_vector_glyph(wchar_t character);

		/** Invalidate cached glyphs for all managed font instances */
		void clear_font_glyph_caches();

		friend class Glyphs;
		
	protected:
/// @cond DOXYGEN_IGNORE
		/** Fonts list */
		Vector<FontPtr> * m_fonts=0;
		Vector<FontFacePtr> * m_fonts_faces=0;
		Vector<VectorGlyphDefinition*> m_vector_glyphs;
		String m_familly;
		bool m_loaded = false;

		/** Mutex for thread-safety */
		std::recursive_mutex m_mutex;  
/// @endcond
	};
}
