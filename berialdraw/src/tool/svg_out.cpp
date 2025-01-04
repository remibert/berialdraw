#include "berialdraw_imp.hpp"

using namespace berialdraw;

SvgOut::SvgOut(const char * filename)
{
	if(filename)
	{
		m_filename = filename;
	}
}

// Destructor
SvgOut::~SvgOut()
{
}

void SvgOut::add_value(String & str, int32_t value)
{
	uint32_t integer = abs(value) >> 6;
	uint32_t decimal;
	decimal = abs(value) % 64;
	decimal = (((decimal*1000)/64)+5)/10;

	if (value < 0)
	{
		str.write_string("-");
	}
	if (decimal)
	{
		str.write_format("%d.%02d",integer, decimal);
	}
	else
	{
		str.write_format("%d",integer);
	}
}

void SvgOut::add_vector(String & str, const FT_Vector* vector)
{
	add_value(str, vector->x);
	str.write_string(",");
	add_value(str, vector->y);
}

int SvgOut::move_to(const FT_Vector*  to, void* user)
{
	SvgOut * this_ = (SvgOut *)user;
	this_->m_content.write_format("M ");
	add_vector(this_->m_content, to);
	this_->m_content.write_format(" ");
	return 0;
}

int SvgOut::line_to(const FT_Vector* to, void* user)
{
	SvgOut * this_ = (SvgOut *)user;
	this_->m_content.write_format("L ");
	add_vector(this_->m_content, to);
	this_->m_content.write_format(" ");
	return 0;
}

int SvgOut::conic_to(const FT_Vector* control, const FT_Vector* to, void* user)
{
	SvgOut * this_ = (SvgOut *)user;
	this_->m_content.write_format("Q ");
	add_vector(this_->m_content, control);
	this_->m_content.write_format(" ");
	add_vector(this_->m_content, to);
	this_->m_content.write_format(" ");
	return 0;
}

int SvgOut::cubic_to(const FT_Vector* control1, const FT_Vector* control2, const FT_Vector* to, void* user)
{
	SvgOut * this_ = (SvgOut *)user;
	this_->m_content.write_format("C ");
	add_vector(this_->m_content, control1);
	this_->m_content.write_format(" ");
	add_vector(this_->m_content, control2);
	this_->m_content.write_format(" ");
	add_vector(this_->m_content, to);
	this_->m_content.write_format(" ");
	return 0;
}

// Add path 
void SvgOut::add_path(Outline & outline_, uint32_t color)
{
	FT_Outline & outline = outline_.get();
	if (color && outline.n_contours > 0 && outline.n_points > 0  && m_closed == false)
	{
		FT_Outline_Funcs decompose_ctx =
		{
			move_to,
			line_to,
			conic_to,
			cubic_to,
			0,
			0
		}; 

		uint32_t alpha = (color & 0xFF000000) >> 24;
		m_content.write_format("	<path style=\"fill:#%06X;",(color & 0xFFFFFF));
		if (alpha != 255) m_content.write_format("fill-opacity:%1d.%02d",(alpha == 255 ? 1 : 0), (alpha == 255 ? 0 : (alpha * 100)/255));
		m_content.write_format("\" d=\"");
		FT_Outline_Decompose(&outline, &decompose_ctx, this);
		m_content.write_format("Z\" />\r");
	}
}

/** Add text in svg 
@param position Position of the text
@param size Size of the text
@param center Center point of the text
@param margin Margin around the text
@param angle Angle of the text
@param text Text to be added
@param font Font of the text
@param color Color of the text */
void SvgOut::add_text(const Point & position, const Size & size, const Point & center, const Margin & margin, Coord angle, const String & text, const Font & font, uint32_t color)
{
	uint32_t i;
	uint32_t count = text.count();
	String part;
	Size part_size;
	wchar_t character;
	uint32_t max_length = 0;
	uint32_t length = 0;
	uint32_t line_count = 1;
	Point ct(center);

	for (i = 0; i < count; i++)
	{
		character = text.get(i);
		if (character == '\n')
		{
			line_count ++;
			if (length > max_length)
			{
				max_length = length;
			}
			length = 0;
		}
		else
		{
			length ++;
		}
	}
	if (length > max_length)
	{
		max_length = length;
	}

	Coord shift = 0; 

	if (line_count != 0)
	{
		shift = size.height_() / line_count;
	}

	for (i = 0; i < count; i++)
	{
		character = text.get(i);
		if (character == '\n')
		{
			part_size.height_(size.height_());
			if (max_length != 0)
			{
				part_size.width_((size.width_() * part.count())/max_length);
			}
			else
			{
				part_size.width(0);
			}

			add_text_line(position, part_size, ct, margin, angle, part, font, color);
			ct.move_(0,0-shift);
			part.clear();
		}
		else
		{
			part += character;
		}
	}

	if (part.size() > 0)
	{
		part_size.height_(size.height_());
		if (max_length != 0)
		{
			part_size.width_((size.width_() * part.count())/max_length);
		}
		else
		{
			part_size.width(0);
		}
		add_text_line(position, part_size, ct, margin, angle, part, font, color);
	}
}

// Add text
void SvgOut::add_text_line(const Point & position, const Size & size, const Point & center, const Margin & margin, Coord angle, const String & text, const Font & font, uint32_t color)
{
	if(text.size() > 0 && m_closed == false)
	{
		uint32_t alpha = (color & 0xFF000000) >> 24;

		Coord xx;
		Coord yy;

		FT_Vector      vec;
		FT_Matrix      matrix = vector_matrix((angle << 10));
		String transform;

		// Compute the position of text
		xx = position.x_();
		yy = position.y_();

		// Rotation of the text line according to the center of rotation
		if (center.x_() != 0 || center.y_() != 0)
		{
			vec.x = -center.x_();
			vec.y = -center.y_();
			FT_Vector_Transform(&vec, &matrix);
			xx += vec.x;
			yy += vec.y;
		}

		// Set the origin of text in top left corner
		vec.x = margin.left_();
		vec.y = margin.top_() + font.real_size().height_();
		FT_Vector_Transform(&vec, &matrix);

		xx += vec.x;
		yy += vec.y;

		m_content.write_format("\t<text x=\"%d\" y=\"%d\" ", xx>>6, yy>>6);
		m_content.write_format("xml:space=\"preserve\" ");
		m_content.write_format("style=\"");
			m_content.write_format("font-size:%dpx;", font.pixel_size().height());
			m_content.write_format("font-family:%s", font.familly().c_str());
			if (font.is_monospaced())
			{
				m_content.write_format(",monospace");
			}
			m_content.write_format(";");
			m_content.write_format("fill:#%06X;",(color & 0xFFFFFF));
			if (alpha != 255) m_content.write_format("opacity:%1d.%02d;",(alpha == 255 ? 1 : 0), (alpha == 255 ? 0 : (alpha * 100)/255));
		m_content.write_format("\" ");

		if (size.height_() != 0 && size.width_() != 0)
		{
			m_content.write_format("textLength=\"");
			add_value(m_content, size.width_());
			m_content.write_format("\" lengthAdjust=\"spacingAndGlyphs\" ");
		}

		if (angle != 0) 
		{
			m_content.write_format("transform=\"rotate(");
			add_value(m_content, -angle);
			m_content.write_char(' ');
			add_value(m_content, xx);
			m_content.write_char(' ');
			add_value(m_content, yy);
			m_content.write_format(")\" ");
		}
		m_content.write_format("");
		m_content.write_format(">");

		for(int i = 0; i < (int)text.size(); i++)
		{
			wchar_t character = text.get(i);
			switch(character)
			{
			case '<' : m_content.write_format("&lt;"  ); break;
			case '>' : m_content.write_format("&gt;"  ); break;
			case '"' : m_content.write_format("&quot;"); break;
			case '&' : m_content.write_format("&amp;" ); break;
			default  : m_content += character; break;
			}
		}
		m_content.write_format("</text>\r");
	}
}

// Open clipping and group
void SvgOut::open_group(const Point & position, const Size & size)
{
	if(m_closed == false)
	{
		m_content.write_format("<defs><clipPath id=\"clip_%d\"><polygon points=\"%d %d,%d %d,%d %d,%d %d\" /></clipPath></defs>\r",
			m_id,
			position.x()               , position.y(),
			position.x() + size.width(), position.y(),
			position.x() + size.width(), position.y() + size.height(),
			position.x()               , position.y() + size.height());
		m_content.write_format("<g clip-path=\"url(#clip_%d)\">\r",m_id++);
	}
}

// Close clipping and group
void SvgOut::close_group()
{
	if(m_closed == false)
	{
		m_content.write_format("</g>\r");
	}
}

// Get the content of svg generated
const String & SvgOut::get()
{
	close(0);
	return m_content;
}

// Close svg
void SvgOut::close(uint32_t crc32)
{
	if(m_closed == false)
	{
		if(m_content.size() > 0)
		{
			if (crc32)
			{
				UIManager::screen_crc()->add(m_filename, crc32);
			}
			m_content.write_format("</svg>\r");
		}

		if(m_filename.size() != 0)
		{
			File file;
			if (file.open(m_filename.c_str(), "w") != -1)
			{
				file.write((const void*)m_content.c_str(), m_content.size());
			}
		}
		m_closed = true;
	}
}

// Set the svg size
void SvgOut::size(const Size & size)
{
	m_content.clear();
	m_content.write_format("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\r");
	m_content.write_format(""
		"<svg xmlns=\"http://www.w3.org/2000/svg\" "
		"version=\"1.1\" "
		"viewBox=\"0 0 %d %d\" "
		">\r", size.width(), size.height());
}

