#include "berialdraw_imp.hpp"

using namespace berialdraw;


/** Init the content of the entry */
Entry::Entry(String * str) : m_input(str)
{
	m_selection_start = 0;
	if (m_input)
	{
		m_selection_end = m_input->count();
	}
	else
	{
		m_selection_end = 0;
	}
	m_cursor_position = 0;
	m_entry_align = Align::ALIGN_LEFT;
	m_typing_mode = (unsigned int)TypingMode::INSERTION;
}

// Copy contructor
Entry::Entry(const Entry & other, String * inp)
{
	m_input = inp;
	if (m_input)
	{
		*m_input = *other.m_input;
	}
	m_selection_start = other.m_selection_start;
	m_selection_end   = other.m_selection_end;
	m_cursor_position = other.m_cursor_position;
	m_entry_align     = other.m_entry_align;
	m_typing_mode     = other.m_typing_mode;
}

/** Copy assignment operator */
Entry& Entry::operator=(const Entry& other)
{
	if (this != &other)
	{
		m_selection_start = other.m_selection_start;
		m_selection_end   = other.m_selection_end;
		m_cursor_position = other.m_cursor_position;
		m_entry_align     = other.m_entry_align;
		m_typing_mode     = other.m_typing_mode;
		m_input          = other.m_input;
	}
	return *this;
}

/** Deinit the content of the entry */
Entry::~Entry()
{
	delete m_mask;
}

/** Insert or replace a char in the entry at the cursor position 
@param character character to add in the input */
void Entry::append_key(wchar_t character)
{
	if (m_input)
	{
		bool selected = (m_selection_start != m_selection_end);
		erase_text_selected();
		if (character)
		{
			if ((TypingMode)m_typing_mode == TypingMode::INSERTION)
			{
				m_input->insert(character, m_cursor_position);
			}
			else
			{
				if (m_cursor_position == m_input->count() || selected)
				{
					m_input->insert(character, m_cursor_position);
				}
				else
				{
					m_input->replace(character, m_cursor_position);
				}
			}
			m_cursor_position++;
			m_selection_end = m_selection_start = m_cursor_position;
		}
	}
}

/** Delete a char a the cursor position
@return true if the new character is accepted */
void Entry::backspace_key()
{
	if (m_input)
	{
		// If nothing selected
		if (m_selection_start == m_selection_end)
		{
			// If position is not the start of entry
			if (m_cursor_position > 0)
			{
				// Remove character before the position
				m_input->remove(m_cursor_position - 1);

				// Move the cursor
				m_cursor_position--;
			}
		}
		else
		{
			// Erase selection
			erase_text_selected();
		}

		// Set the selection at the current position of the cursor
		m_selection_start = m_selection_end = m_cursor_position;
	}
}

/** Delete a character at the cursor position */
void Entry::delete_key()
{
	if (m_input)
	{
		// If nothing selected
		if (m_selection_start == m_selection_end)
		{
			// If the position is correct
			if (m_cursor_position < (int)m_input->count())
			{
				// Remove the character at the position
				m_input->remove(m_cursor_position);
			}
		}
		else
		{
			// Erase selection
			erase_text_selected();
		}

		// Set the selection at the current position of the cursor
		m_selection_start = m_selection_end = m_cursor_position;
	}
}

/** Position of the cursor in line and column */
void Entry::line_column(uint32_t & line, uint32_t & column)
{
	if (m_input)
	{
		uint32_t count = m_input->count();
		line = 0;
		column = 0;
		wchar_t character;
		for (uint32_t i = 0; i < count && i < m_cursor_position; i++)
		{
			character = m_input->get(i);
			if (character == '\n')
			{
				line ++;
				column = 0;
			}
			else
			{
				column ++;
			}
		}
	}
}

/** Move the position of the cursor in line and column */
void Entry::move_cursor(uint32_t line, uint32_t column, bool selected)
{
	if (m_input)
	{
		uint32_t count = m_input->count();
		uint32_t new_line = 0;
		uint32_t new_column = 0;
		wchar_t character;
		uint32_t new_position = 0;
		bool found = false;
		for (new_position = 0; new_position < count; new_position++)
		{
			character = m_input->get(new_position);
			if (character == '\n')
			{
				if (line == new_line)
				{
					found = true;
					break;
				}
				new_line ++;
				new_column = 0;
			}
			else
			{
				if (new_line == line && new_column == column)
				{
					found = true;
					break;
				}
				new_column ++;
			}
		}
		if (found == false)
		{
			if (new_line == line)
			{
				found = true;
				new_position = count;
			}
		}
	
		if (found)
		{
			if (selected)
			{
				if (new_position < m_cursor_position)
				{
					if (m_selection_start == m_cursor_position)
					{
						m_selection_start = new_position;
					}
					else
					{
						m_selection_end = new_position;
					}
				}
				else
				{
					if (m_selection_end == m_cursor_position)
					{
						m_selection_end = new_position;
					}
					else
					{
						m_selection_start = new_position;
					}
				}
				if (m_selection_start > m_selection_end)
				{
					uint32_t tmp = m_selection_start;
					m_selection_start = m_selection_end;
					m_selection_end = tmp;
				}
			}
			else
			{
				m_selection_start = m_selection_end = new_position;
			}
			m_cursor_position = new_position;
		}
	}
}

/** Return the count of wide character detected in selected line */
uint32_t Entry::line_length(uint32_t line)
{
	uint32_t result = 0;
	if (m_input)
	{
		uint32_t column = 0;
		uint32_t max = m_input->count();
		column = 0;

		uint32_t current_line = 0;
		bool found = false;
		for (uint32_t i = 0; i < max; i++)
		{
			wchar_t character = m_input->get(i);
			if (character == '\n')
			{
				if (current_line == line)
				{
					result = column;
					column = 0;
					break;
				}
				current_line ++;
				column = 0;
			}
			else
			{
				column ++;
			}
		}

		result += column;
	}
	return result;
}

/** Move the cursor to the up character  */
void Entry::up_arrow_key(bool selected)
{
	if (m_input)
	{
		uint32_t line = 0;
		uint32_t column = 0;
		line_column(line, column);
		if (line > 0)
		{
			if ((m_entry_align & Align::ALIGN_HORIZONTAL) == Align::ALIGN_LEFT)
			{
				move_cursor(line-1, column, selected);
			}
			else
			{
				move_arrow(line, line-1, column, selected);
			}
		}
	}
}

/** Move the cursor to the down character */
void Entry::down_arrow_key(bool selected)
{
	if (m_input)
	{
		uint32_t line = 0;
		uint32_t column = 0;
		line_column(line, column);
		if ((m_entry_align & Align::ALIGN_HORIZONTAL) == Align::ALIGN_LEFT)
		{
			move_cursor(line+1, column, selected);
		}
		else
		{
			move_arrow(line, line+1, column, selected);
		}
	}
}

/** Move the cursor from the line to another line with the same column */
void Entry::move_arrow(uint32_t from_line, uint32_t to_line, uint32_t column, bool selected)
{
	if (m_input)
	{
		uint32_t length_line_src = line_length(from_line);
		uint32_t length_line_dst = line_length(to_line);

		if ((m_entry_align & Align::ALIGN_HORIZONTAL) == Align::CENTER)
		{
			uint32_t middle_line_src = ((length_line_src * 64)/2 + 33)>>6;
			uint32_t middle_line_dst = ((length_line_dst * 64)/2 + 33)>>6;

			if (column >= middle_line_src)
			{
				uint32_t delta = column - (middle_line_src);
				if (delta + (middle_line_dst) > length_line_dst)
				{
					column = length_line_dst;
				}
				else
				{
					column = delta + (middle_line_dst);
				}
			}
			else
			{
				uint32_t delta = (middle_line_src) - column;

				if (delta > middle_line_dst)
				{
					column = 0;
				}
				else
				{
					column = (middle_line_dst) - delta;
				}
			}
			move_cursor(to_line, column, selected);
		}
		else if ((m_entry_align & Align::ALIGN_HORIZONTAL) == Align::ALIGN_RIGHT)
		{
			if ((length_line_src - column) <= length_line_dst)
			{
				column = length_line_dst - (length_line_src - column);
			}
			else
			{
				column = 0;
			}
			move_cursor(to_line, column, selected);
		}
	}
}

/** Move the cursor to the next character */
void Entry::right_arrow_key(bool selected)
{
	if (m_input)
	{
		// If next with shift pressed
		if (selected)
		{
			// If nothing selected
			if (m_selection_start == m_selection_end)
			{
				// Start selection
				m_selection_start = m_cursor_position;

				// If next char existing
				if (m_cursor_position < (int)m_input->count())
				{
					// Move cursor to the next
					m_cursor_position ++;
					m_selection_end = m_cursor_position;
				}
			}
			// If the cursor is at the end of selection
			else if (m_cursor_position == m_selection_end)
			{
				// If next char existing
				if (m_cursor_position < (int)m_input->count())
				{
					// Move cursor to the next
					m_cursor_position ++;
					m_selection_end = m_cursor_position;
				}
			}
			else
			{
				// If next char existing
				if (m_cursor_position < (int)m_input->count())
				{
					// Move cursor to the next
					m_cursor_position ++;
					m_selection_start = m_cursor_position;
				}
			}
		}
		else
		{
			// If selection opened
			if(m_selection_start != m_selection_end)
			{
				// Set position of cursor at the end of selection
				m_cursor_position = m_selection_start = m_selection_end;
			}
			else
			{
				// If next char existing
				if (m_cursor_position < (int)m_input->count())
				{
					// Move cursor to the next
					m_cursor_position ++;
				}
				// Reset selection
				m_selection_start = m_selection_end = m_cursor_position;
			}
		}
	}
}

/** Move the cursor to the previous character */
void Entry::left_arrow_key(bool selected)
{
	if (m_input)
	{
		// If previous with shift pressed
		if (selected)
		{
			// If nothing selected
			if (m_selection_start == m_selection_end)
			{
				// Selection end at current position
				m_selection_end = m_cursor_position;

				// If previous char existing
				if (m_cursor_position > 0)
				{
					// Move cursor to the previous
					m_cursor_position --;
					m_selection_start = m_cursor_position;
				}
			}
			// If the cursor is at the end of selection
			else if (m_cursor_position == m_selection_end)
			{
				// If previous char existing
				if (m_cursor_position > 0)
				{
					// Move cursor to the previous
					m_cursor_position --;
					m_selection_end = m_cursor_position;
				}
			}
			else
			{
				// If previous char existing
				if (m_cursor_position > 0)
				{
					// Move cursor to the previous
					m_cursor_position --;
					m_selection_start = m_cursor_position;
				}
			}
		}
		else
		{
			// If selection opened
			if(m_selection_start != m_selection_end)
			{
				// Set position of cursor at the start of selection
				m_cursor_position = m_selection_end = m_selection_start;
			}
			else
			{
				// If previous char existing
				if (m_cursor_position > 0)
				{
					// Move cursor to the previous
					m_cursor_position --;
				}
				// Reset selection
				m_selection_start = m_selection_end = m_cursor_position;
			}
		}
	}
}

/** Move the cursor to the first character  */
void Entry::home_key(bool selected)
{
	if (m_input)
	{
		// If home with shift pressed
		if (selected)
		{
			// If nothing selected
			if (m_selection_start == m_selection_end)
			{
				// Selection end at current position
				m_selection_end = m_cursor_position;
			}
			// If the cursor is at the end of selection
			else if (m_cursor_position == m_selection_end)
			{
				// Selection end at start selection
				m_selection_end = m_selection_start;
			}

			// Set the start of selection at the begin of entry
			m_selection_start = m_cursor_position = 0;
		}
		else
		{
			// Move cursor to the first character
			m_selection_start = m_selection_end = m_cursor_position = 0;
		}
	}
}

/** Move the cursor to the last character */
void Entry::end_key(bool selected)
{
	if (m_input)
	{
		// If end with shift pressed
		if (selected)
		{
			// If nothing selected
			if (m_selection_start == m_selection_end)
			{
				// Selection start at current position
				m_selection_start = m_cursor_position;
			}
			// If the cursor is at the begin of selection
			else if (m_cursor_position == m_selection_start)
			{
				// Selection start at end of selection
				m_selection_start = m_selection_end;
			}

			// Set the end of selection at the end of entry
			m_selection_end = m_cursor_position = m_input->count();
		}
		else
		{
			// Move cursor to the last character
			m_selection_start = m_selection_end = m_cursor_position = m_input->count();
		}
	}
}

/** Clear the content of the entry */
void Entry::clear_input()
{
	if (m_input)
	{
		m_input->clear();
		m_selection_start = m_selection_end = m_cursor_position = 0;
	}
}

/** Build the text for the display 
@param display display string */
const String & Entry::input() const
{
	return *m_input;
}

/** Set typing_mode */
void Entry::typing_mode(enum Entry::TypingMode m)
{
	m_typing_mode = (unsigned int)m;
}

/** Set typing_mode */
enum Entry::TypingMode Entry::typing_mode() const
{
	return (TypingMode)m_typing_mode;
}

/** Get the cursor position in entry */
uint32_t Entry::cursor_position() const
{
	return m_cursor_position;
}

/** Get selected text */
void Entry::text_selected(String & text)
{
	if (m_input)
	{
		m_input->slice(m_selection_start,m_selection_end,text);
	}
}

/** Get start of selection */
uint32_t Entry::selection_start() const
{
	return m_selection_start;
}

/** Get end of selection */
uint32_t Entry::selection_end() const
{
	return m_selection_end;
}

/** Select text in entry */
void Entry::select_text(uint32_t from, uint32_t to)
{
	if (m_input)
	{
		uint32_t count = m_input->count();

		// If the start of selection is before the end
		if (from != to )
		{
			uint32_t start;
			uint32_t end;
			if (from < to)
			{
				start = from;
				end   = to;
			}
			else
			{
				start = to;
				end   = from;
			}
			// If the start not exceed the entry
			if (start <= count)
			{
				// Set the selection start
				m_selection_start = start;
			}
			else
			{
				// Set the selection start at the end entry
				m_selection_start = count;
			}

			// If the not exceed the entry
			if (end <= count)
			{
				// Set the selection end
				m_selection_end = end;
			}
			else
			{
				// Set the selection end at the end of entry
				m_selection_end = count;
			}

			if (to <= count)
			{
				m_cursor_position = to;
			}
			else
			{
				m_cursor_position = count;
			}
		}
		// If the start is equal to end (move the cursor only)
		else
		{
			// If the start not exceed the entry
			if (to <= count)
			{
				m_selection_start = m_selection_end = m_cursor_position = to;
			}
			else
			{
				// Set the selection start at the end entry
				m_selection_start = m_selection_end = m_cursor_position = count;
			}
		}
	}
}

/** Select all text in entry */
void Entry::select_all()
{
	if (m_input)
	{
		select_text(m_input->count(),0);
	}
}

/** No text selected */
void Entry::select_none()
{
	if (m_input)
	{
		select_text(0,0);
	}
}

/** Erase text selected */
void Entry::erase_text_selected()
{
	if (m_input)
	{
		// If text selected
		if (m_selection_start != m_selection_end)
		{
			// Remove this text
			m_input->remove(m_selection_start,m_selection_end);

			// Set the position at the start of selection
			m_cursor_position = m_selection_end = m_selection_start;
		}
	}
}

/** Set the entry alignement 
@param alignement */
void Entry::entry_align(Align align)
{
	m_entry_align = align;
}

void Entry::on_key_down(wchar_t key, KeyEvent::Modifier modifier)
{
	bool accept = true;
	if (m_input)
	{
		switch(key)
		{
		case (wchar_t)ReservedKey::KEY_TABULATION  :
			break;

		case (wchar_t)ReservedKey::KEY_SHIFT      :
			break;

		case (wchar_t)ReservedKey::KEY_ENTER      :
			append_key((wchar_t)ReservedKey::KEY_ENTER);
			break;

		case (wchar_t)ReservedKey::KEY_HOME:
			home_key(modifier & KeyEvent::MODIFIER_SHIFT);
			break;

		case (wchar_t)ReservedKey::KEY_END:
			end_key(modifier & KeyEvent::MODIFIER_SHIFT);
			break;

		case (wchar_t)ReservedKey::KEY_LEFT :
			left_arrow_key(modifier & KeyEvent::MODIFIER_SHIFT);
			break;

		case (wchar_t)ReservedKey::KEY_RIGHT:
			right_arrow_key(modifier & KeyEvent::MODIFIER_SHIFT);
			break;

		case (wchar_t)ReservedKey::KEY_UP   :
			up_arrow_key(modifier & KeyEvent::MODIFIER_SHIFT);
			break;
				
		case (wchar_t)ReservedKey::KEY_DOWN :
			down_arrow_key(modifier & KeyEvent::MODIFIER_SHIFT);
			break;

		case (wchar_t)ReservedKey::KEY_PAUSE:
			break;

		case (wchar_t)ReservedKey::KEY_DELETE    :
			// If a validator existing
			if (m_mask)
			{
				String text;
				MaskValidator validator(*m_mask);
				Entry in_validation(*this,&text);

				// Remove char in entry to check if the field is still valid
				in_validation.delete_key();

				// If the field is invalid
				if (validator.validate(text) == Validator::REFUSED)
				{
					// Reject key pressed
					accept = false;
				}
			}
			// If key accepted
			if (accept)
			{
				// Delete character in entry
				delete_key();
			}
			break;

		case (wchar_t)ReservedKey::KEY_BACKSPACE  :
			// If a validator existing
			if (m_mask)
			{
				String text;
				MaskValidator validator(*m_mask);
				Entry in_validation(*this,&text);

				// Remove char in entry to check if the field is still valid
				in_validation.backspace_key();

				// If the field is invalid
				if (validator.validate(text) == Validator::REFUSED)
				{
					// Reject key pressed
					accept = false;
				}
			}
			// If key accepted
			if (accept)
			{
				// Backspace character in entry
				backspace_key();
			}
			break;

		default:
			// If a validator existing
			if (m_mask)
			{
				String text = *m_input;
				MaskValidator validator(*m_mask);
				Entry in_validation(*this, &text);

				// Adds the keypress to check if the field is still valid
				in_validation.append_key(key);

				// If the field is invalid
				if (validator.validate(text) == Validator::REFUSED)
				{
					// Search the next separator
					wchar_t  separator = validator.separator(text);
					
					// If next separator found
					if (separator != '\0')
					{
						// Remove key pressed
						in_validation.backspace_key();

						// Add separator
						in_validation.append_key(separator);

						// Add key
						in_validation.append_key(key);

						// If the field is invalid
						if (validator.validate(text) == Validator::REFUSED)
						{
							// Reject the key
							accept = false;
						}
						else
						{
							// Add separator
							append_key(separator);
						}
					}
					else
					{
						// Reject key
						accept = false;
					}
				}
			}
			// If key accepted
			if (accept)
			{
				// Add key in the entry
				append_key(key);
			}
			break;
		}
	}
}


/** Set input mask
@param mask :
% followed with : 
	- d:decimal (0-9)
	- a:alphanumeric (A-Z,a-z,0-9)
	- A:alphanumeric (A-Z,0-9)
	- s:string (all characters)
	- l:letter (a-z)
	- L:letter (A-Z)
	- X:hexadecimal (A-F,0-9)
	- x:hexadecimal (a-f,0-9)
	*/
void Entry::mask(const String & mask_)
{
	if (m_mask)
	{
		delete m_mask;
		m_mask = 0;
	}
	if (mask_.size() > 0)
	{
		m_mask = new String(mask_);
	}
}

/** Get input mask */
const String Entry::mask() const
{
	if (m_mask)
	{
		return *m_mask;
	}
	return String::empty;
}


#ifdef _DEBUG
#include <assert.h>
#define check(str,ref) assert(strcmp(str.input().c_str(), ref) == 0);

void Entry::test1()
{
	{
		String s("Hello  World\nHW\n");
		Entry entry(&s);
		entry.entry_align(Align::ALIGN_RIGHT);
		entry.left_arrow_key();
		entry.down_arrow_key();
		entry.append_key('0');
		check(entry,"Hello  World\n0HW\n");
	}

	{
		String s("Hello  World\nHW\n");
		Entry entry(&s);
		entry.entry_align(Align::ALIGN_LEFT);
		entry.left_arrow_key();
		entry.move_cursor(1,2,true);
		entry.move_cursor(1,1,true);
		entry.erase_text_selected();
		check(entry,"W\n");
	}

	{
		String s("Hello  World\nHW\n");
		Entry entry(&s);
		entry.entry_align(Align::ALIGN_LEFT);
		entry.right_arrow_key();
		entry.move_cursor(0,2,true);
		entry.erase_text_selected();
		check(entry,"He");
	}
	{
		String s("Hello  World\nHW\n");
		Entry entry(&s);
		entry.entry_align(Align::ALIGN_LEFT);
		entry.left_arrow_key();
		entry.move_cursor(1,5,true);
		entry.erase_text_selected();
		check(entry,"\n");
	}
	{
		String s("Hello  World\nHW\n");
		Entry entry(&s);
		entry.entry_align(Align::ALIGN_LEFT);
		entry.left_arrow_key();
		entry.down_arrow_key(true);
		entry.erase_text_selected();
		check(entry,"HW\n");
	}
	{
		String s("Hello  World\nHW\n");
		Entry entry(&s);
		entry.entry_align(Align::ALIGN_LEFT);
		entry.delete_key();
		check(entry,"");
	}
	{
		String s("Hello  World\nHW\n");
		Entry entry(&s);
		entry.entry_align(Align::ALIGN_LEFT);
		entry.backspace_key();
		check(entry,"");
	}
	{
		String s("Hello  World\nHW\n");
		Entry entry(&s);
		entry.entry_align(Align::ALIGN_LEFT);
		entry.left_arrow_key();
		entry.right_arrow_key(true);
		entry.right_arrow_key(true);
		entry.right_arrow_key(true);
		entry.right_arrow_key(true);
		entry.erase_text_selected();
		check(entry,"o  World\nHW\n");
		entry.up_arrow_key();
	}
	{
		String s("Hello  World\nHW\n");
		Entry entry(&s);
		entry.entry_align(Align::ALIGN_LEFT);
		entry.left_arrow_key();

		entry.down_arrow_key();
		entry.append_key('1');
		check(entry,"Hello  World\n1HW\n");
		entry.up_arrow_key();

		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();

		entry.down_arrow_key();
		entry.append_key('0');
		check(entry,"Hello  World\n1HW0\n");
	}
	{
		String s("Hello  World\nHW\n");
		Entry entry(&s);
		entry.entry_align(Align::ALIGN_RIGHT);
		entry.left_arrow_key();

		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();

		entry.down_arrow_key();
		entry.append_key('0');
		check(entry,"Hello  World\nH0W\n");
	}

	{
		String s("Hello  World\nHW\n");
		Entry entry(&s);
		entry.entry_align(Align::CENTER);
		entry.left_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.down_arrow_key();
		entry.append_key('0');
		check(entry,"Hello  World\nH0W\n");
		
		entry.right_arrow_key();
		entry.up_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.down_arrow_key();
		entry.append_key('1');
		check(entry,"Hello  World\nH0W1\n");


		entry.left_arrow_key();
		entry.left_arrow_key();
		entry.left_arrow_key();
		entry.left_arrow_key();
		entry.up_arrow_key();
		entry.left_arrow_key();
		entry.left_arrow_key();
		entry.down_arrow_key();
		entry.append_key('2');
		check(entry,"Hello  World\n2H0W1\n");

		entry.home_key();
		entry.down_arrow_key();
		entry.append_key('3');
		check(entry,"Hello  World\n32H0W1\n");
		
	}
	{
		String s("Hello World\nsalut\n");
		Entry entry(&s);
		entry.select_none();
		entry.append_key('1');
		entry.append_key('2');
		entry.append_key('3');
		check(entry,"123Hello World\nsalut\n");
		entry.select_all();
		entry.append_key('1');
		check(entry,"1");
	}
	{
		String s("Hello World\nsalut\n");
		Entry entry(&s);
		uint32_t count = entry.line_length(0);
		assert(count == 11);
		count = entry.line_length(1);
		assert(count == 5);
		count = entry.line_length(2);
		assert(count == 0);
		count = entry.line_length(3);
		assert(count == 0);
	}

	{
		String s("Hello World");
		Entry entry(&s);
		entry.home_key(); assert(entry.cursor_position() == 0);
		entry.right_arrow_key(); assert(entry.cursor_position() == 1);
		entry.right_arrow_key(); assert(entry.cursor_position() == 2);
		entry.right_arrow_key(); assert(entry.cursor_position() == 3);
		entry.right_arrow_key(); assert(entry.cursor_position() == 4);

		entry.right_arrow_key(true); assert(entry.cursor_position() ==  5); assert(entry.selection_start() == 4); assert(entry.selection_end() == 5);
		entry.right_arrow_key(true); assert(entry.cursor_position() ==  6); assert(entry.selection_start() == 4); assert(entry.selection_end() == 6);
		entry.right_arrow_key(true); assert(entry.cursor_position() ==  7); assert(entry.selection_start() == 4); assert(entry.selection_end() == 7);
		entry.right_arrow_key(true); assert(entry.cursor_position() ==  8); assert(entry.selection_start() == 4); assert(entry.selection_end() == 8);
		entry.right_arrow_key(true); assert(entry.cursor_position() ==  9); assert(entry.selection_start() == 4); assert(entry.selection_end() == 9);
		entry.right_arrow_key(true); assert(entry.cursor_position() == 10); assert(entry.selection_start() == 4); assert(entry.selection_end() == 10);
		entry.right_arrow_key(true); assert(entry.cursor_position() == 11); assert(entry.selection_start() == 4); assert(entry.selection_end() == 11);
		entry.right_arrow_key(true); assert(entry.cursor_position() == 11); assert(entry.selection_start() == 4); assert(entry.selection_end() == 11);
		entry.right_arrow_key(true); assert(entry.cursor_position() == 11); assert(entry.selection_start() == 4); assert(entry.selection_end() == 11);

		entry.left_arrow_key(true); assert(entry.cursor_position() == 10); assert(entry.selection_start() == 4); assert(entry.selection_end() == 10);
		entry.left_arrow_key(true); assert(entry.cursor_position() ==  9); assert(entry.selection_start() == 4); assert(entry.selection_end() ==  9);
		entry.left_arrow_key(true); assert(entry.cursor_position() ==  8); assert(entry.selection_start() == 4); assert(entry.selection_end() ==  8);
		entry.left_arrow_key(true); assert(entry.cursor_position() ==  7); assert(entry.selection_start() == 4); assert(entry.selection_end() ==  7);
		entry.left_arrow_key(true); assert(entry.cursor_position() ==  6); assert(entry.selection_start() == 4); assert(entry.selection_end() ==  6);
		entry.left_arrow_key(true); assert(entry.cursor_position() ==  5); assert(entry.selection_start() == 4); assert(entry.selection_end() ==  5);
		entry.left_arrow_key(true); assert(entry.cursor_position() ==  4); assert(entry.selection_start() == 4); assert(entry.selection_end() ==  4);
		entry.left_arrow_key(true); assert(entry.cursor_position() ==  3); assert(entry.selection_start() == 3); assert(entry.selection_end() ==  4);
		entry.left_arrow_key(true); assert(entry.cursor_position() ==  2); assert(entry.selection_start() == 2); assert(entry.selection_end() ==  4);
		entry.left_arrow_key(true); assert(entry.cursor_position() ==  1); assert(entry.selection_start() == 1); assert(entry.selection_end() ==  4);
		entry.left_arrow_key(true); assert(entry.cursor_position() ==  0); assert(entry.selection_start() == 0); assert(entry.selection_end() ==  4);
		entry.left_arrow_key(true); assert(entry.cursor_position() ==  0); assert(entry.selection_start() == 0); assert(entry.selection_end() ==  4);

		entry.right_arrow_key(true); assert(entry.cursor_position() ==  1); assert(entry.selection_start() == 1); assert(entry.selection_end() == 4);
		entry.right_arrow_key(true); assert(entry.cursor_position() ==  2); assert(entry.selection_start() == 2); assert(entry.selection_end() == 4);
		entry.right_arrow_key(true); assert(entry.cursor_position() ==  3); assert(entry.selection_start() == 3); assert(entry.selection_end() == 4);
		entry.right_arrow_key(true); assert(entry.cursor_position() ==  4); assert(entry.selection_start() == 4); assert(entry.selection_end() == 4);

		entry.home_key(true); assert(entry.cursor_position() ==  0); assert(entry.selection_start() == 0); assert(entry.selection_end() == 4);
		entry.end_key(true);  assert(entry.cursor_position() == 11); assert(entry.selection_start() == 4); assert(entry.selection_end() == 11);

		entry.left_arrow_key(true); assert(entry.cursor_position() == 10); assert(entry.selection_start() == 4); assert(entry.selection_end() == 10);
		entry.left_arrow_key(true); assert(entry.cursor_position() ==  9); assert(entry.selection_start() == 4); assert(entry.selection_end() ==  9);
		entry.left_arrow_key(true); assert(entry.cursor_position() ==  8); assert(entry.selection_start() == 4); assert(entry.selection_end() ==  8);

		entry.home_key(true); assert(entry.cursor_position() ==  0); assert(entry.selection_start() == 0); assert(entry.selection_end() == 4);

		entry.right_arrow_key(true); assert(entry.cursor_position() ==  1); assert(entry.selection_start() == 1); assert(entry.selection_end() == 4);
		entry.right_arrow_key(true); assert(entry.cursor_position() ==  2); assert(entry.selection_start() == 2); assert(entry.selection_end() == 4);
		entry.right_arrow_key(true); assert(entry.cursor_position() ==  3); assert(entry.selection_start() == 3); assert(entry.selection_end() == 4);
		entry.right_arrow_key(true); assert(entry.cursor_position() ==  4); assert(entry.selection_start() == 4); assert(entry.selection_end() == 4);

		entry.end_key(true);  assert(entry.cursor_position() == 11); assert(entry.selection_start() == 4); assert(entry.selection_end() == 11);

		entry.select_text(0, 11); assert(entry.cursor_position() == 11); assert(entry.selection_start() == 0); assert(entry.selection_end() == 11);

		entry.select_text(0, 20); assert(entry.cursor_position() == 11); assert(entry.selection_start() == 0); assert(entry.selection_end() == 11);

		entry.select_text(5, 5); assert(entry.cursor_position() == 5); assert(entry.selection_start() == 5); assert(entry.selection_end() == 5);
		entry.select_text(20, 20); assert(entry.cursor_position() == 11); assert(entry.selection_start() == 11); assert(entry.selection_end() == 11);

		entry.select_text(19, 20); assert(entry.cursor_position() == 11); assert(entry.selection_start() == 11); assert(entry.selection_end() == 11);
	}

	{
		String s("Default value");
		Entry entry(&s);
		//entry.input(s);
		assert(entry.cursor_position() == 0);
		entry.append_key('1');
		assert(entry.cursor_position() == 1);
		entry.append_key('2');
		assert(entry.cursor_position() == 2);
		entry.append_key('3');
		assert(entry.cursor_position() == 3);
		check(entry,"123");
		entry.clear_input();
		assert(entry.cursor_position() == 0);
	}

	{
		String s("Default value");
		Entry entry(&s);
		entry.append_key('1');
		entry.append_key('2');
		entry.append_key('3');
		check(entry,"123");
	}

	{
		String s("Default value");
		Entry entry(&s);
		entry.end_key();
		entry.append_key('1');
		entry.append_key('2');
		entry.append_key('3');
		check(entry,"Default value123");
	}
	{
		String s("Default value");
		Entry entry(&s);
		entry.home_key();
		entry.append_key('1'); 
		entry.append_key('2');
		entry.append_key('3');
		check(entry,"123Default value");
	}
	{
		String s("Default value");
		Entry entry(&s);
		entry.right_arrow_key();
		entry.append_key('1');
		entry.append_key('2');
		entry.append_key('3');
		check(entry,"Default value123");
	}
	{
		String s("Default value");
		Entry entry(&s);
		entry.left_arrow_key();
		entry.append_key('1');
		entry.append_key('2');
		entry.append_key('3');
		check(entry,"123Default value");
	}

	{
		String s("");
		Entry entry(&s);
		entry.typing_mode(TypingMode::REPLACEMENT);
		entry.append_key('1');
		entry.append_key('2');
		entry.append_key('3');
		check(entry,"123");
		entry.home_key();
		entry.append_key('4');
		check(entry,"423");
		entry.append_key('5');
		check(entry,"453");
		entry.append_key('6');
		check(entry,"456");
		entry.append_key('7');
		check(entry,"4567");
	}

	{
		String s("burp");
		Entry entry(&s);
		assert(entry.typing_mode() == TypingMode::INSERTION);

		entry.typing_mode(TypingMode::REPLACEMENT);
		assert(entry.typing_mode() == TypingMode::REPLACEMENT);
		entry.append_key('a');
		entry.append_key('b');
		entry.append_key('c');
		entry.append_key('d');
		entry.append_key('e');
		entry.append_key('f');
		check(entry,"abcdef");
		entry.home_key();
		entry.append_key('g');
		entry.append_key('h');
		entry.append_key('i');
		entry.append_key('j');
		entry.append_key('k');
		entry.append_key('l');
		entry.append_key('m');
		check(entry,"ghijklm");
	}

	{
		String s("burp");
		Entry entry(&s);
		assert(entry.typing_mode() == TypingMode::INSERTION);
		entry.typing_mode(TypingMode::REPLACEMENT);
		assert(entry.typing_mode() == TypingMode::REPLACEMENT);
	}
	{
		String s("rémi à la plage");
		Entry entry(&s);
		entry.select_text(0,0);
		check(entry,"rémi à la plage");
		entry.end_key();
		entry.backspace_key();
		entry.backspace_key();
		entry.backspace_key();
		entry.backspace_key();
		entry.backspace_key();
		entry.backspace_key();
		entry.backspace_key();
		entry.append_key('\'');
		entry.append_key('e');
		entry.append_key('a');
		entry.append_key('u');
		check(entry,"rémi à l'eau");
		entry.left_arrow_key();
		entry.left_arrow_key();
		entry.left_arrow_key();
		entry.left_arrow_key();
		entry.left_arrow_key();
		entry.left_arrow_key();
		entry.backspace_key();
		entry.append_key('d');
		entry.append_key('a');
		entry.append_key('n');
		entry.append_key('s');
		check(entry,"rémi dans l'eau");

		entry.home_key();
		entry.backspace_key();
		entry.backspace_key();
		entry.backspace_key();
		entry.backspace_key();
		check(entry,"rémi dans l'eau");

		entry.delete_key();
		entry.delete_key();
		entry.delete_key();
		entry.delete_key();
		check(entry," dans l'eau");

		entry.end_key();
		entry.delete_key();
		entry.delete_key();
		entry.delete_key();
		entry.delete_key();
		check(entry," dans l'eau");
		entry.left_arrow_key();
		entry.delete_key();
		check(entry," dans l'ea");
	}
	{
		String s;
		Entry entry(&s);
		entry.append_key('a');
		entry.append_key('b');
		entry.append_key('c');
		check(entry,"abc");
		entry.left_arrow_key();
		entry.append_key('d');
		entry.left_arrow_key();
		entry.append_key('e');
		entry.left_arrow_key();
		entry.append_key('f');
		entry.left_arrow_key();
		entry.append_key('g');
		check(entry,"abgfedc");
		entry.left_arrow_key();
		entry.left_arrow_key();
		entry.left_arrow_key();
		entry.left_arrow_key();
		entry.left_arrow_key();
		entry.append_key(0xE9);
		check(entry,"éabgfedc");

		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.append_key(0xE0);
		check(entry,"éabgfedcà");
	}

	{

		String s("Hello");
		Entry hello(&s);
		hello.left_arrow_key();
		hello.left_arrow_key();
		hello.right_arrow_key();
		hello.right_arrow_key();
		hello.right_arrow_key();
		hello.right_arrow_key();
		hello.right_arrow_key();
	}
}

// Unitary test
void Entry::test()
{
	static bool done = false;
	if (done == false)
	{
		done = true;
		test1();
	}
}
#endif
