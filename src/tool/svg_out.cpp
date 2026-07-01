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

// Determine MIME type from filename extension
static const char * mime_type_from_filename(const char * filename)
{
	const char * result = nullptr;
	if (filename)
	{
		// Find last dot
		const char * dot = nullptr;
		for (const char * p = filename; *p; p++)
		{
			if (*p == '.')
			{
				dot = p;
			}
		}
		if (dot)
		{
			dot++; // skip the dot
			// Case-insensitive compare
			if ((dot[0] == 'p' || dot[0] == 'P') && (dot[1] == 'n' || dot[1] == 'N') && (dot[2] == 'g' || dot[2] == 'G') && dot[3] == '\0')
			{
				result = "image/png";
			}
			else if ((dot[0] == 'j' || dot[0] == 'J') && (dot[1] == 'p' || dot[1] == 'P') && (dot[2] == 'g' || dot[2] == 'G') && dot[3] == '\0')
			{
				result = "image/jpeg";
			}
			else if ((dot[0] == 'j' || dot[0] == 'J') && (dot[1] == 'p' || dot[1] == 'P') && (dot[2] == 'e' || dot[2] == 'E') && (dot[3] == 'g' || dot[3] == 'G') && dot[4] == '\0')
			{
				result = "image/jpeg";
			}
		}
	}
	return result;
}

// Add image from original file (PNG, JPEG)
void SvgOut::add_image_file(const char * filename, int32_t x, int32_t y, uint32_t display_width, uint32_t display_height, uint8_t alpha, ImageFitMode fit_mode, uint32_t src_width, uint32_t src_height, Coord angle, int32_t center_x, int32_t center_y)
{
	if (filename && display_width > 0 && display_height > 0 && m_closed == false)
	{
		const char * mime = mime_type_from_filename(filename);
		if (mime)
		{
			// Read original file data
			File file;
			if (file.open(filename, "rb") != -1)
			{
				uint32_t file_size = file.size();
				if (file_size > 0)
				{
					uint8_t * file_data = new uint8_t[file_size];
					if (file.read(file_data, file_size) == (int)file_size)
					{
						// Base64 encode the raw file content
						String base64_data;
						base64_data.base64_encode(file_data, file_size);

						// Compute fitted image dimensions and centering offset
						uint32_t img_w = display_width;
						uint32_t img_h = display_height;
						int32_t img_x = x;
						int32_t img_y = y;
						const char * aspect_ratio = "none";

						if (fit_mode == FIT && src_width > 0 && src_height > 0)
						{
							// Compute fitted size preserving aspect ratio
							ImageProcessor::compute_fit_size(src_width, src_height,
								display_width, display_height, FIT, img_w, img_h);

							// Center the fitted image in the display area
							img_x = x + (int32_t)(display_width - img_w) / 2;
							img_y = y + (int32_t)(display_height - img_h) / 2;
							aspect_ratio = "none";
						}

						// Compute center for rotation
						int32_t cx = x + center_x;
						int32_t cy = y + center_y;

						// Write SVG <image> element
						m_content.write_format("\t<image x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" ",
							img_x, img_y, img_w, img_h);
						m_content.write_format("preserveAspectRatio=\"%s\" ", aspect_ratio);
						if (alpha < 255)
						{
							m_content.write_format("opacity=\"%1d.%02d\" ", (alpha == 255 ? 1 : 0), (alpha * 100) / 255);
						}
						if (angle != 0)
						{
							m_content.write_format("transform=\"rotate(");
							add_value(m_content, -angle);
							m_content.write_format(" %d %d)\" ", cx, cy);
						}
						m_content.write_format("href=\"data:");
						m_content.write_string(mime);
						m_content.write_format(";base64,");
						m_content.write_string(base64_data.c_str());
						m_content.write_format("\" />\r");
					}
					delete[] file_data;
				}
				file.close();
			}
		}
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
		shift = size.height_q6() / line_count;
	}

	for (i = 0; i < count; i++)
	{
		character = text.get(i);
		if (character == '\n')
		{
			part_size.height_q6(size.height_q6());
			if (max_length != 0)
			{
				part_size.width_q6((size.width_q6() * part.count())/max_length);
			}
			else
			{
				part_size.width(0);
			}

			add_text_line(position, part_size, ct, margin, angle, part, font, color);
			ct.move_q6(0,0-shift);
			part.clear();
		}
		else
		{
			part += character;
		}
	}

	if (part.size() > 0)
	{
		part_size.height_q6(size.height_q6());
		if (max_length != 0)
		{
			part_size.width_q6((size.width_q6() * part.count())/max_length);
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
		xx = position.x_q6();
		yy = position.y_q6();

		// Rotation of the text line according to the center of rotation
		if (center.x_q6() != 0 || center.y_q6() != 0)
		{
			vec.x = -center.x_q6();
			vec.y = -center.y_q6();
			FT_Vector_Transform(&vec, &matrix);
			xx += vec.x;
			yy += vec.y;
		}

		// Set the origin of text in top left corner
		vec.x = margin.left_q6();
		vec.y = margin.top_q6() + font.real_size().height_q6();
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

		if (size.height_q6() != 0 && size.width_q6() != 0)
		{
			m_content.write_format("textLength=\"");
			add_value(m_content, size.width_q6());
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


