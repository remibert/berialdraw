#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
TextBox::TextBox()
{
}

/** Destructor */
TextBox::~TextBox()
{
}

void TextBox::parse(const Area & text_area, Font & font, String & text, 
	uint32_t cursor_pos, uint32_t sel_start, uint32_t sel_end, Align text_align)
{
	uint32_t count = text.count();
	wchar_t character;
	Dim line_height = 0;
	Dim line_width = 0;
	Size space_size;
	uint32_t i;
	LineInfo info;
	bool selection = false;

	line_height = font.real_size().height_();
	space_size = font.char_size(' ');

	m_lines.clear();
	m_lines_size.set(0,0);
	m_cursor_size.set(0,0);
	m_cursor_pos.set(0,0);
	m_cursor_line = 0;

	// Parse all character to search new lines
	for (i=0; i < count; i++)
	{
		character = text.get(i);

		// Save the start position in string of line
		if (info.line_start == UINT32_MAX)
		{
			info.line_start = i;
		}

		if (sel_start != UINT32_MAX)
		{
			// If the current character is selected
			if (i >= sel_start && i <= sel_end && sel_start != sel_end)
			{
				selection = true;
				// If selection start not yet detected
				if (info.selection_start == UINT32_MAX)
				{
					// Set selection start
					info.selection_start = line_width;
				}
				// Set selection end
				info.selection_end = line_width;
			}
		}

		// If new line detected
		if (character == '\n')
		{
			// Set the size of line
			info.size.set_(line_width, line_height);

			if (cursor_pos != UINT32_MAX)
			{
				// If the cursor is at the end of line
				if (i == cursor_pos)
				{
					// Set cursor position
					m_cursor_pos.set_(line_width, m_lines_size.height_());
					m_cursor_size = space_size;
					m_cursor_line = m_lines.size();
				}
			}

			// Save the end of current line
			info.line_end = i;

			// If the line width is greater than previously width found
			if (m_lines_size.width_() < line_width)
			{
				// Keep this width
				m_lines_size.width_(line_width);
			}

			// Save the y position of line
			info.position.y_(m_lines_size.height_());

			// Add the height of line in the line size
			m_lines_size.height_(m_lines_size.height_() + font.real_size().height_());

			// Add this line sizes to the lines list
			m_lines.push_back(info);

			line_width = 0;

			// Clear line informations
			info.selection_end   = UINT32_MAX;
			info.selection_start = UINT32_MAX;
			info.line_start      = UINT32_MAX;
			info.line_end        = UINT32_MAX;
			info.size.set(0,0);
		}
		else
		{
			// Get the size of character
			Size char_size = font.char_size(character);

			// Save the end of current line
			info.line_end = i;

			if (cursor_pos != UINT32_MAX)
			{
				// If the cursor in on the current character
				if (i == cursor_pos)
				{
					// Save cursor position
					m_cursor_pos.set_(line_width, m_lines_size.height_());
					m_cursor_size = char_size;
					m_cursor_line = m_lines.size();
				}
			}

			// Increase the line width with character width
			line_width  += char_size.width_();
		}
	}

	// Handles the special case when the cursor is on the last line and the preceding character is a carriage return
	if (i == cursor_pos && info.line_start == UINT32_MAX && info.line_end == UINT32_MAX)
	{
		m_cursor_pos.set_(line_width, m_lines_size.height_());
		m_cursor_size = space_size;
		m_cursor_line = m_lines.size();
		info.line_end = i;
		info.line_start = i;
		info.position.y_(m_lines_size.height_());
		info.size.set_(line_width, line_height);
		m_lines.push_back(info);
	}

	// If line width measured
	if (line_width > 0)
	{
		// Save the end of current line
		info.line_end = i;

		// If the line width is greater than previously width found
		if (m_lines_size.width_() < line_width)
		{
			// Keep this width
			m_lines_size.width_(line_width);
		}

		// Save the y position of line
		info.position.y_(m_lines_size.height_());

		// Add the height of line in the line size
		m_lines_size.height_(m_lines_size.height_() + font.real_size().height_());

		if (sel_start != UINT32_MAX)
		{
			// If the current character is the end of selection
			if (i == sel_end && info.selection_start != UINT32_MAX)
			{
				info.selection_end = line_width;
			}
		}

		if (cursor_pos != UINT32_MAX)
		{
			// If the cursor is on the current character
			if (i == cursor_pos)
			{
				// Save cursor position
				m_cursor_pos.set_(line_width, m_lines_size.height_() - space_size.height_());
				m_cursor_size = space_size;
				m_cursor_line = m_lines.size();
			}
		}

		// Set line size
		info.size.set_(line_width, line_height);

		// Add the current line into the lines list
		m_lines.push_back(info);
	}
	else
	{
		// Save cursor position
		m_cursor_size = space_size;
	}
	
	Coord movex = 0;

	// If edit field is defined
	if (text_area.width_())
	{
		// If text align is center in edit field
		if ((text_align & ALIGN_HORIZONTAL) == CENTER)
		{
			movex = 0-(((Coord)m_lines_size.width_() - (Coord)text_area.width_())/2);
		}
		// If text align is right in edit field
		else if ((text_align & ALIGN_HORIZONTAL) == ALIGN_RIGHT)
		{
			movex = 0-((Coord)m_lines_size.width_() - (Coord)text_area.width_());
		}
	}
	Coord deltax = 0;

	// Move lines according to alignement
	for (uint32_t i = 0; i < m_lines.size(); i++)
	{
		deltax = 0;

		// If text align is center
		if ((text_align & ALIGN_HORIZONTAL) == CENTER)
		{
			deltax = movex+(m_lines_size.width_() - m_lines[i].size.width_())/2;
		}
		// If text align is right
		else if ((text_align & ALIGN_HORIZONTAL) == ALIGN_RIGHT)
		{
			deltax = movex+m_lines_size.width_() - m_lines[i].size.width_();
		}

		m_lines[i].position.x_(deltax);
		m_lines[i].selection_start += deltax;
		m_lines[i].selection_end   += deltax;

		if (cursor_pos != UINT32_MAX)
		{
			// If the line contains a cursor
			if (cursor_pos >= m_lines[i].line_start && cursor_pos <= m_lines[i].line_end)
			{
				m_cursor_pos.move_(deltax,0);
			}
		}
	}
}

/** Get the cursor position according to the click location */
uint32_t TextBox::cursor_position(const Point & click_location, const Area & text_area, const Point & text_shift, Font & font, const String & text, Align text_align)
{
	Point text_origin(text_area.position());
	text_origin.move(text_shift);
	Point current_line(text_origin);
	uint32_t line_detected = UINT32_MAX;
	uint32_t result = UINT32_MAX;
	Coord last_line = 0;

	// Search the line clicked
	for (uint32_t i = 0; i < m_lines.size(); i++)
	{
		if (click_location.y_() >= current_line.y_() && 
			click_location.y_() <= current_line.y_() + (Coord)m_lines[i].size.height_())
		{
			line_detected = i;
			break;
		}
		last_line = current_line.y_() + (Coord)m_lines[i].size.height_();
		current_line.move_(0, m_lines[i].size.height_());
	}


	if (line_detected == UINT32_MAX && m_lines.size() > 0)
	{
		if (click_location.y_() < current_line.y_())
		{
			line_detected = 0;
		}
		else if (click_location.y_() > last_line)
		{
			line_detected = m_lines.size() -1;
		}
	}

	if (line_detected != UINT32_MAX)
	{
		Point line_position;

		line_position.move(text_area.position());
		line_position.move(text_shift);
		line_position.move(m_lines[line_detected].position);

		uint32_t char_start = m_lines[line_detected].line_start;
		uint32_t char_end   = m_lines[line_detected].line_end;

		// If click is before the text
		if (click_location.x_() < line_position.x_())
		{
			// Cursor is on the start of line
			result = m_lines[line_detected].line_start;
		}
		// If click is after the text
		else if (click_location.x_() > (line_position.x_() + (Coord)m_lines[line_detected].size.width_()))
		{
			// Cursor is on the end of line
			result = m_lines[line_detected].line_end;
		}
		else
		{
			Point coord_char;

			// Search in line where is the cursor position
			for(uint32_t i = char_start ; i < char_end; i++)
			{
				wchar_t character = text.get(i);
				Size char_size = font.char_size(character);

				// If cursor position found
				if (click_location.x_() >= (line_position.x_() + coord_char.x_()) &&
					click_location.x_() <= (line_position.x_() + coord_char.x_() + (Coord)char_size.width_()))
				{
					result = i;
					// If the click past the half of char
					if (click_location.x_() >= (line_position.x_() + coord_char.x_() + ((Coord)char_size.width_()/2)))
					{
						// If not the last character
						if (i < char_end)
						{
							// Select the next character
							result = i+1;
						}
					}
					break;
				}
				coord_char.move_(char_size.width_(),0);
			}
		}
	}
	return result;
}

/** Moves the text to make the area containing the cursor visible. */
void TextBox::text_shift(Point & cursor_shift, const Area & text_area)
{
	Area cursor_clip(m_cursor_pos,m_cursor_size);
	cursor_clip.move(text_area.position());

	Coord movex = horizontal_shift(cursor_clip, cursor_shift, text_area);
	Coord movey = vertical_shift(cursor_clip, cursor_shift, text_area);

	// If text must be moved
	if (movex != 0 || movey != 0)
	{
		cursor_shift.move(movex, movey);
	}
}

/** Moves the text to make the area containing the cursor visible. */
Coord TextBox::vertical_shift(Area & cursor_clip, Point & cursor_shift, const Area & text_area)
{
	Coord movey = 0;

	// If cursor is before text edit field in y
	if (cursor_clip.position().y() + cursor_shift.y() < text_area.position().y())
	{
		movey = text_area.position().y() - (cursor_clip.position().y() + cursor_shift.y());

		// If the height of edit field is smaller than cursor height
		if (text_area.size().height() < cursor_clip.size().height())
		{
			movey -= (cursor_clip.size().height() - text_area.size().height())/2;
		}
	}
	// If cursor is after text edit field in y
	else if (cursor_clip.position().y() + cursor_shift.y() + cursor_clip.size().height() > text_area.position().y() + text_area.size().height())
	{
		movey -= (cursor_clip.position().y() + cursor_shift.y() + cursor_clip.size().height()) - (text_area.position().y() + text_area.size().height());

		if ((text_area.position().y() - (cursor_clip.position().y() + cursor_shift.y())) == 0)
		{
			if (movey == 1 || movey == -1)
			{
				// Avoid text jittering during mouse selection
				movey = 0;
			} 
		}

		// If the height of edit field is smaller than cursor height
		if (text_area.size().height() < cursor_clip.size().height())
		{
			movey += (cursor_clip.size().height() - text_area.size().height())/2;
		}
	}
	return movey;
}

/** Moves the text to make the area containing the cursor visible. */
Coord TextBox::horizontal_shift(Area & cursor_clip, Point & cursor_shift, const Area & text_area)
{
	Coord movex = 0;
	if (m_cursor_line < m_lines.size())
	{
		Coord start_of_line   = m_lines[m_cursor_line].position.x();
		Coord end_of_line     = ((Coord)m_lines[m_cursor_line].size.width()) + start_of_line;
		Coord dist_from_end   = end_of_line - m_cursor_pos.x();
		Coord dist_from_start = m_cursor_pos.x() - start_of_line;

		// If cursor is before text edit field in x
		if (cursor_clip.position().x() + cursor_shift.x() < text_area.position().x())
		{
			if (dist_from_start > (Coord)(text_area.size().width()/4))
			{
				// Move the text to see the cursor
				movex = text_area.position().x() - (cursor_clip.position().x() + cursor_shift.x());

				// Add offset in move
				movex += text_area.size().width ()/4;
			}
			else
			{
				// Remove the cursor offset to go to the start of field
				cursor_shift.x(0-m_lines[m_cursor_line].position.x());
				movex = 0;
			}
		}
		// If cursor is after text edit field in x
		else if (cursor_clip.position().x() + cursor_shift.x() + cursor_clip.size().width() > text_area.position().x() + text_area.size().width ())
		{
			// Move the text to see the cursor
			movex -= (cursor_clip.position().x() + cursor_shift.x() + cursor_clip.size().width ()) - (text_area.position().x() + text_area.size().width ());

			// If the cursor is far from the end of the edited line
			if (dist_from_end > (Coord)(text_area.size().width()/4))
			{
				// Add offset in move
				movex -= text_area.size().width ()/4;
			}
			else
			{
				movex -= dist_from_end;
			}
		}
	}
	else
	{
		cursor_shift.set(0,0);
	}
	return movex;
}

/** Get the size of the TextBox content.
@return Size object representing the content size of the TextBox */
Size TextBox::content_size()
{
	return m_lines_size;
}

void TextBox::paint(const Point & cursor_shift, Font & font, 
	const String & text, const Point & position, const Area & area, uint32_t text_color, 
	uint32_t cursor_color, uint32_t select_color, bool insertion)
{
	Point  center;
	Margin  margin;
	Exporter * exporter = UIManager::exporter();

	if (exporter)
	{
		exporter->open_group(area.position(), area.size());
	}

	paint(cursor_shift, font, text, position, center, margin, 0, text_color, cursor_color, select_color, insertion);

	if (exporter)
	{
		exporter->close_group();
	}
}

void TextBox::paint(const Point & cursor_shift, Font & font, const String & text, 
			const Point & position, const Point & center, const Margin & margin,
			Coord angle, uint32_t text_color, uint32_t cursor_color, uint32_t select_color, bool insertion)
{
	Point shift;
	Point line_center(center);

	// Parse all character to search new lines
	for (uint32_t i = 0; i < m_lines.size(); i++)
	{
		// Display selection
		if (m_lines[i].selection_start != m_lines[i].selection_end && select_color != 0)
		{
			// Compute selection position according to the line and center
			Point select_pos;
				select_pos = position;
				select_pos.move(cursor_shift);
				select_pos.move_(0, m_lines[i].position.y_());
				select_pos.move_(0,0-i*font.real_size().height_());

			// Compute the center
			Point select_center(line_center);
				select_center.move_(0-m_lines[i].selection_start,0);

			// Draw rectangle
			Rect rect(0);
				rect.size_((m_lines[i].selection_end-m_lines[i].selection_start), m_lines[i].size.height_());
				rect.position(select_pos);
				rect.center(select_center);
				rect.thickness_(0);
				rect.radius_(0);
				rect.angle((angle>>6) + (90<<6));
				rect.color(select_color);
				rect.paint(shift);
		}

		// Display text line
		{
			// Compute text position according to the line and center
			Point line_pos(position);
				line_pos.move(cursor_shift);
				line_pos.move(m_lines[i].position);

			String text_line;
			// Get text line from full buffer
			text.slice(m_lines[i].line_start, m_lines[i].line_end, text_line);

			// Move the position to take into account the rotation center
			line_pos.move_(0,0-i*font.real_size().height_());

			// Draw text line
			font.draw(text_line, line_pos, line_center, margin, angle, text_color);
		}

		// If the current line has a cursor
		if (i == m_cursor_line && cursor_color != 0)
		{
			// If cursor is visible
			if (m_cursor_size.width_() > 0 && m_cursor_size.height_() > 0)
			{
				// Compute cursor position according to the line and center
				Point cursor_pos;
					cursor_pos = position;
					cursor_pos.move(cursor_shift);
					cursor_pos.move_(0, m_lines[i].position.y_());
					cursor_pos.move_(0,0-i*font.real_size().height_());

				// Compute the center
				Point cursor_center(line_center);
				cursor_center.move_(0-m_cursor_pos.x_(),0);

				// Draw cursor
				Rect rect(0);
					if (insertion)
					{
						rect.size_(2<<6, m_cursor_size.height_());
						cursor_center.move(1,0);
					}
					else
					{
						cursor_center.move_(0, 0-(m_cursor_size.height_()-(3<<6)));
						rect.size_(m_cursor_size.width_(), 3<<6);
					}

					rect.position(cursor_pos);
					rect.center(cursor_center);
					rect.thickness_(0);
					rect.radius_(0);
					rect.angle((angle>>6) + (90<<6));
					rect.color(cursor_color);
					rect.paint(shift);
			}
		}

		// Move rotation center to the next line
		line_center.y_(line_center.y_() - font.real_size().height_());
	}
}

