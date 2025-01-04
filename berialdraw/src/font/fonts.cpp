#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Create fonts */
Fonts::Fonts()
{
	m_fonts = new Vector<FontPtr>;
	m_fonts_faces = new Vector<FontFacePtr>;
	m_familly = "Berial";
	load_directory("resources/fonts");
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

/** Select the instance of font, if the font familly not found return the first font, increase reference counter */
FontPtr Fonts::select(const String & familly, const Size & size, enum Font::Style style)
{
	FontPtr result;

	std::lock_guard<std::recursive_mutex> lock(m_mutex);
	if (m_fonts && m_fonts_faces)
	{
		FontPtr selected;
		const String * searched_familly = &familly;

		if (familly == "")
		{
			searched_familly = &m_familly;
		}
		
		// Find the font that matches exactly
		for (uint32_t i = 0; i < m_fonts->size(); i++)
		{
			// If font is found and style found and size found
			if (((*m_fonts)[i]->familly()    == *searched_familly || *searched_familly == "") && 
				((*m_fonts)[i]->style()      == style   || style   == Font::UNDEFINED) && 
				((*m_fonts)[i]->pixel_size() == size))
			{
				selected = (*m_fonts)[i];
				break;
			}
		}

		// Exact font not found
		if (selected.get() == 0)
		{
			FontFacePtr font_face;
			FontFacePtr first_face;

			// Search the font face equal to font choosed
			for(uint32_t i = 0; i < m_fonts_faces->size(); i++)
			{
				if ((*m_fonts_faces)[i]->familly() == *searched_familly&& 
				  (((*m_fonts_faces)[i]->style()   == style) || style == Font::UNDEFINED))
				{
					font_face = (*m_fonts_faces)[i];
					break;
				}
				if (first_face.get() == 0)
				{
					first_face = (*m_fonts_faces)[i];
				}
			}

			// If font corresponding exactly
			if (font_face.get())
			{
				result = SharedPtr<Font>(new Font(font_face, size));
				m_fonts->push_back(result);
			}
			// Else take the first font found
			else if (first_face.get())
			{
				result = SharedPtr<Font>(new Font(first_face, size));
				m_fonts->push_back(result);
			}

			// Clean up font
			for(uint32_t i = 0; i < m_fonts->size(); )
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
	std::lock_guard<std::recursive_mutex> lock(m_mutex);
	Directory directory; 
	if (directory.open(dir))
	{
		if (directory.first())
		{
			do
			{
				if (directory.match("*.ttf",true))
				{
					if (load(directory.full_path()) == false)
					{
						result = false;
					}
				}
			}
			while(directory.next());
		}
	}
	else
	{
		result = false;
	}
	return result;
}
