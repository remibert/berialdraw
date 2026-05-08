#include "berialdraw_imp.hpp"

using namespace berialdraw;

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
