#include "berialdraw_imp.hpp"

using namespace berialdraw;

namespace berialdraw
{
	class VectorGlyphDefinition
	{
	public:
		VectorGlyphDefinition(wchar_t character, const VectorImage & image) :
			m_character(character),
			m_image(new VectorImage(image, 0))
		{
			if (m_image && m_image->path_count() == 0)
			{
				m_image->load();
			}
			m_valid = m_image && m_image->path_count() > 0;
		}

		~VectorGlyphDefinition()
		{
			delete m_image;
		}

		VectorImage* copy_image() const
		{
			return m_valid ? new VectorImage(*m_image, 0) : 0;
		}

		wchar_t m_character;
		VectorImage * m_image;
		bool m_valid = false;
	};
}

/** Create fonts */
Fonts::Fonts()
{
	m_fonts = new Vector<FontPtr>;
	m_fonts_faces = new Vector<FontFacePtr>;
	m_familly = "Cerial";
}

/** Destroy fonts */
Fonts::~Fonts()
{
	delete m_fonts;
	m_fonts = 0;
	delete m_fonts_faces;
	m_fonts_faces = 0;
	for (uint32_t i = 0; i < m_vector_glyphs.size(); i++)
	{
		delete m_vector_glyphs[i];
	}
	m_vector_glyphs.clear();
	_Font::unload_ft_lib();
}

/** Open font and add into the list */
bool Fonts::load(const String & filename)
{
	bool result = false;
	bool found = false;

	std::lock_guard<std::recursive_mutex> lock(m_mutex);
	if (m_fonts_faces)
	{
		for (uint32_t i = 0; i < m_fonts_faces->size(); i++)
		{	
			if((*m_fonts_faces)[i]->filename() == filename)
			{
				found = true;
				break;
			}
		}

		if(found == false)
		{
			if (File::exists(filename.c_str()))
			{
				result = true;
				m_fonts_faces->push_back(SharedPtr<FontFace>(new FontFace(filename)));
			}
		}
	}
	return result;
}

/** Close font and remove from the list */
void Fonts::unload(const String & filename)
{
	std::lock_guard<std::recursive_mutex> lock(m_mutex);
	if (m_fonts)
	{
		// Clean up font
		for(uint32_t i = 0; i < m_fonts->size(); )
		{
			if((*m_fonts)[i]->filename() == filename)
			{
				m_fonts->remove(i);
			}
			else
			{
				i++;
			}
		}
	}
}

/** Find best font face by family name (or any family if nullptr) and style priority */
FontFacePtr Fonts::find_best_font_face(const String * family_name, enum Font::Style style)
{
	FontFacePtr result;
	FontFacePtr normal_face;
	FontFacePtr bold_face;
	FontFacePtr italic_face;
	FontFacePtr first_face;

	// Search font faces with optional family filter, ordered by style priority
	for (uint32_t i = 0; i < m_fonts_faces->size(); i++)
	{
		// Skip if family filter is set and doesn't match
		if (family_name != NULL && (*m_fonts_faces)[i]->familly() != *family_name)
		{
			continue;
		}

		// Store first face as ultimate fallback
		if (first_face.get() == NULL)
		{
			first_face = (*m_fonts_faces)[i];
		}

		// Priority hierarchy:
		// 1. Exact style match (if requested)
		if ((*m_fonts_faces)[i]->style() == style && style != Font::UNDEFINED)
		{
			result = (*m_fonts_faces)[i];
			break;
		}
		// 2. NORMAL style
		else if ((*m_fonts_faces)[i]->style() == Font::NORMAL && normal_face.get() == NULL)
		{
			normal_face = (*m_fonts_faces)[i];
		}
		// 3. BOLD style
		else if ((*m_fonts_faces)[i]->style() == Font::BOLD && bold_face.get() == NULL)
		{
			bold_face = (*m_fonts_faces)[i];
		}
		// 4. ITALIC style
		else if ((*m_fonts_faces)[i]->style() == Font::ITALIC && italic_face.get() == NULL)
		{
			italic_face = (*m_fonts_faces)[i];
		}
	}

	// Select best face according to hierarchy if exact match not found
	if (result.get() == NULL)
	{
		if (normal_face.get() != NULL)
		{
			result = normal_face;
		}
		else if (bold_face.get() != NULL)
		{
			result = bold_face;
		}
		else if (italic_face.get() != NULL)
		{
			result = italic_face;
		}
		else if (first_face.get() != NULL)
		{
			result = first_face;
		}
	}

	return result;
}

/** Select the instance of font, if the font familly not found return the first font, increase reference counter */
FontPtr Fonts::select(const String & familly, const Size & size, enum Font::Style style)
{
	FontPtr result;

	if (m_loaded == false)
	{
		m_loaded = true;
		load_directory("$(ui.fonts)");
	}

	std::lock_guard<std::recursive_mutex> lock(m_mutex);
	if (m_fonts && m_fonts_faces)
	{
		FontPtr selected;
		const String * searched_familly = &familly;

		if (familly == "")
		{
			searched_familly = &m_familly;
		}
		
		// Find the font that matches exactly (same family, style and size)
		for (uint32_t i = 0; i < m_fonts->size(); i++)
		{
			if (((*m_fonts)[i]->familly()    == *searched_familly || *searched_familly == "") && 
				((*m_fonts)[i]->style()      == style   || style   == Font::UNDEFINED) && 
				((*m_fonts)[i]->pixel_size() == size))
			{
				selected = (*m_fonts)[i];
				break;
			}
		}

		// If exact font not found, search for a font face and create one
		if (selected.get() == NULL)
		{
			// First pass: Search font faces with requested family
			FontFacePtr best_face = find_best_font_face(searched_familly, style);

			// Second pass: If no font found with requested family, search any available font
			if (best_face.get() == NULL)
			{
				best_face = find_best_font_face(NULL, style);
			}

			// Create font from best face found
			if (best_face.get() != NULL)
			{
				result = SharedPtr<Font>(new Font(best_face, size));
				m_fonts->push_back(result);
			}

			// Clean up unused fonts (those with reference count == 1, meaning only held by m_fonts)
			for (uint32_t i = 0; i < m_fonts->size(); )
			{
				if ((*m_fonts)[i].count() == 1)
				{
					m_fonts->remove(i);
				}
				else
				{
					i++;
				}
			}
		}
		else
		{
			result = selected;
		}
	}
	return result;
}

/** Clear glyphs cach */
void Fonts::clear_cach()
{
	std::lock_guard<std::recursive_mutex> lock(m_mutex);
	for(uint32_t i = 0; i < m_fonts->size(); i++)
	{
		(*m_fonts)[i]->clear();
	}
	m_loaded = false;
}

// Find the first vector glyph not lower than the character
uint32_t Fonts::lower_bound_vector_glyph(wchar_t character)
{
	uint32_t low = 0;
	uint32_t high = m_vector_glyphs.size();
	while (low < high)
	{
		uint32_t mid = (low + high) / 2;
		if (m_vector_glyphs[mid]->m_character < character)
		{
			low = mid + 1;
		}
		else
		{
			high = mid;
		}
	}
	return low;
}

// Invalidate cached glyphs for all managed font instances
void Fonts::clear_font_glyph_caches()
{
	if (m_fonts)
	{
		for (uint32_t i = 0; i < m_fonts->size(); i++)
		{
			(*m_fonts)[i]->clear();
		}
	}
}

// Define or replace a vector glyph available to every font
bool Fonts::define_vector_glyph(wchar_t character, const VectorImage & image)
{
	bool result = false;
	if (character != 0)
	{
		if (character < 0x20)
		{
			character = 0x20;
		}
		VectorGlyphDefinition * definition = new VectorGlyphDefinition(character, image);
		if (definition && definition->m_valid)
		{
			std::lock_guard<std::recursive_mutex> lock(m_mutex);
			uint32_t index = lower_bound_vector_glyph(character);
			if (index < m_vector_glyphs.size() && m_vector_glyphs[index]->m_character == character)
			{
				delete m_vector_glyphs[index];
				m_vector_glyphs[index] = definition;
			}
			else
			{
				m_vector_glyphs.insert(index, definition);
			}
			clear_font_glyph_caches();
			result = true;
		}
		else
		{
			delete definition;
		}
	}
	return result;
}

// Define or replace a vector glyph by loading its image from a file
bool Fonts::define_vector_glyph(wchar_t character, const String & filename)
{
	bool result = false;
	VectorImage image(0);
	image.filename(filename);
	if (image.load())
	{
		result = define_vector_glyph(character, image);
	}
	return result;
}

// Remove a vector glyph definition available to every font
bool Fonts::remove_vector_glyph(wchar_t character)
{
	bool result = false;
	if (character < 0x20)
	{
		character = 0x20;
	}
	std::lock_guard<std::recursive_mutex> lock(m_mutex);
	uint32_t index = lower_bound_vector_glyph(character);
	if (index < m_vector_glyphs.size() && m_vector_glyphs[index]->m_character == character)
	{
		delete m_vector_glyphs[index];
		m_vector_glyphs.remove(index);
		clear_font_glyph_caches();
		result = true;
	}
	return result;
}

// Remove all vector glyph definitions
void Fonts::clear_vector_glyphs()
{
	std::lock_guard<std::recursive_mutex> lock(m_mutex);
	for (uint32_t i = 0; i < m_vector_glyphs.size(); i++)
	{
		delete m_vector_glyphs[i];
	}
	m_vector_glyphs.clear();
	clear_font_glyph_caches();
}

// Copy a registered vector glyph image for a per-font cache
VectorImage* Fonts::copy_vector_glyph(wchar_t character)
{
	VectorImage * result = 0;
	std::lock_guard<std::recursive_mutex> lock(m_mutex);
	uint32_t index = lower_bound_vector_glyph(character);
	if (index < m_vector_glyphs.size() && m_vector_glyphs[index]->m_character == character)
	{
		result = m_vector_glyphs[index]->copy_image();
	}
	return result;
}

/** Get default familly font name */
const String & Fonts::familly()
{
	std::lock_guard<std::recursive_mutex> lock(m_mutex);
	return m_familly;
}

/** Set default familly for font */
void Fonts::familly(const String & name)
{
	std::lock_guard<std::recursive_mutex> lock(m_mutex);
	m_familly = name;
}

/** Load all fonts from the selected directory  */
bool Fonts::load_directory(const String & dir)
{
	bool result = true;
	bool found = false;
	std::lock_guard<std::recursive_mutex> lock(m_mutex);
	Directory directory; 

	if (directory.open(dir))
	{
		if (directory.first())
		{
			do
			{
				String full_path = directory.full_path();
				if (File::match_pattern("*.ttf", full_path.c_str(), false) == true && 
					File::match_pattern("*/._*.ttf", full_path.c_str(), false) == false)
				{
					if (load(full_path) == false)
					{
						result = false;
					}
					else
					{
						found = true;
					}
				}
			}
			while(directory.next());
		}
	}
	if (found == false)
	{
		bd_printf("No font found, you need to provide an existing directory in UIManager::init\n");
		result = false;
	}
	return result;
}
