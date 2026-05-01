#pragma once
namespace berialdraw
{
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
		
	protected:
/// @cond DOXYGEN_IGNORE
		/** Fonts list */
		Vector<FontPtr> * m_fonts=0;
		Vector<FontFacePtr> * m_fonts_faces=0;
		String m_familly;
		bool m_loaded = false;

		/** Mutex for thread-safety */
		std::recursive_mutex m_mutex;  
/// @endcond
	};
}
