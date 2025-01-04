#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
MaskValidator::MaskValidator(String & mask) : m_mask(mask)
{
}

/** Destructor */
MaskValidator::~MaskValidator()
{
}

Validator::ValidResult MaskValidator::check_char(wchar_t mask_char, wchar_t text_char)
{
	Validator::ValidResult result = Validator::ACCEPTED;
	switch(mask_char)
	{
	case 'd':
		if (!isdigit(text_char)) 
		{
			result = ValidResult::REFUSED;
		}
		break;

	case 'a':
		if ((isalnum(text_char) && isupper(text_char)) || !isalnum(text_char))
		{
			result = ValidResult::REFUSED;
		}
		break;

	case 'A':
		if ((isalnum(text_char) && islower(text_char)) || !isalnum(text_char))
		{
			result = ValidResult::REFUSED;
		}
		break;

	case 'x':
		if ((isxdigit(text_char) && isupper(text_char)) || !isxdigit(text_char))
		{
			result = ValidResult::REFUSED;
		}
		break;

	case 'X':
		if ((isxdigit(text_char) && islower(text_char)) || !isxdigit(text_char))
		{
			result = ValidResult::REFUSED;
		}
		break;

	case 'l':
		if (islower(text_char) == false)
		{
			result = ValidResult::REFUSED;
		}
		break;

	case 'L':
		if (isupper(text_char) == false)
		{
			result = ValidResult::REFUSED;
		}
		break;
				
	default:
		result = ValidResult::REFUSED;
		break;
	}
	return result;
}

Validator::ValidResult MaskValidator::get_mask_char(uint32_t & pos)
{
	Validator::ValidResult result = Validator::ACCEPTED;
	m_significant_zero = false;
	m_length = UINT32_MAX;
	m_mask_char = m_mask.get(pos);
	m_min = INT32_MIN;
	m_max = INT32_MAX;
	
	// If field detected
	if (m_mask_char == '%')
	{
		pos++;

		wchar_t character = m_mask.get(pos);

		// If the % must be treated
		if (character == '%')
		{
			m_mask_char = '%';
			pos++;
		}
		else if (character == '\0')
		{
			// Field not correctly ended
			result = ValidResult::REFUSED;
		}
		else
		{
			String len;
			String min;
			String max;
			uint32_t i = pos;
			int state = 0;
			bool loop = true;
			m_mask_char = '\0';

			while(loop)
			{
				switch(state)
				{
				// Read length
				case 0:
					// If length of field
					if (isdigit(character))
					{
						len += character;
					}
					// If the field is ended
					else if (isalpha(character))
					{
						m_mask_char = character;
						loop = false;
					}
					// If min and max specified
					else if (character == '[')
					{
						state = 1;
					}
					else
					{
						result = ValidResult::REFUSED;
						loop = false;
					}
					break;

				// Read min
				case 1:
					// If min detected
					if (isdigit(character) || (character == '-' && min.size() == 0))
					{
						min += character;
					}
					// If max specified
					else if (character == '.')
					{
						// Read other dot
						state = 2;
					}
					else
					{
						result = ValidResult::REFUSED;
						loop = false;
					}
					break;

				// Read other point
				case 2:
					if (character == '.')
					{
						// Start the read of max
						state = 3;
					}
					else
					{
						result = ValidResult::REFUSED;
						loop = false;
					}
					break;

				// Read max
				case 3:
					// If min detected
					if (isdigit(character) || (character == '-' && max.size() == 0))
					{
						max += character;
					}
					// If slice terminated
					else if (character == ']')
					{
						// Start the type of field
						state = 4;
					}
					else
					{
						result = ValidResult::REFUSED;
						loop = false;
					}
					break;

				// Read the type of field
				case 4:
					// If the field is ended
					if (isalpha(character))
					{
						m_mask_char = character;
						loop = false;
					}
					else
					{
						result = ValidResult::REFUSED;
						loop = false;
					}
				}
				if (loop)
				{
					pos++;
					character = m_mask.get(pos);
				}
			}
			
			if (result == ValidResult::ACCEPTED)
			{
				// If digit not empty
				if (len.size() > 0)
				{
					// If the zero is significant
					if (len.get(0) == '0')
					{
						m_significant_zero = true;
					}

					// Get the length of field
					int32_t l;
					len.to_int(l);
					m_length = (uint32_t)l;
				}

				// If min size detected
				if (min.size() > 0)
				{
					min.to_int(m_min);
				}

				// If max size detected
				if (max.size() > 0)
				{
					max.to_int(m_max);
				}

				if (m_max < m_min)
				{
					int32_t tmp;
					tmp   = m_max;
					m_max = m_min;
					m_min = tmp;
				}
				pos++;
			}
			if (result == ValidResult::ACCEPTED)
			{
				if (m_mask_char < 0xFF)
				{
					m_mask_char |= 0xF800;
				}
			}
		}
	}
	else
	{
		pos++;
	}

	// If the mask is ended but text entered not yet finished
	if (m_mask_char == '\0')
	{
		result =  ValidResult::REFUSED;
	}
	return result;
}

Validator::ValidResult MaskValidator::check_text(const String & text, uint32_t & pos)
{
	Validator::ValidResult result = Validator::ACCEPTED;
	uint32_t count = text.count();
	uint32_t len = m_length;
	wchar_t text_char;
	uint32_t i;
	String field;
	m_field_overflow = false;

	// If the mask is a letter
	if ((m_mask_char & 0xFF00) == 0xF800)
	{
		wchar_t mask_char = m_mask_char & 0xFF;

		// Extra field from text entered
		for (i = pos; i < count; i++)
		{
			text_char = text.get(i);

			// Check if character is correct according to the mask
			if (check_char(mask_char, text_char) || (mask_char == 'd' && m_min < 0 && field.size() == 0 && text_char == '-'))
			{
				field += text_char;
			}
			else
			{
				// Stop the fill of field
				i -= 1;
				break;
			}
		}

		// Check if field contains zero but at the start
		if (m_significant_zero == false)
		{
			if (field.size() > 2)
			{
				if (field.get(0) == '-' && field.get(1) == '0')
				{
					result = ValidResult::REFUSED;
				}
			}
			if (field.size() > 1)
			{
				if (field.get(0) == '0')
				{
					m_field_overflow = true;
					result = ValidResult::REFUSED;
				}
			}
		}

		// If the min specified
		if (m_min != INT32_MIN)
		{
			if (mask_char == 'd')
			{
				int32_t value;
				field.to_int(value);
				if (value < m_min)
				{
					m_field_overflow = true;
					result = ValidResult::REFUSED;
				}
			}
		}

		// If the max specified
		if (m_max != INT32_MAX)
		{
			if (mask_char == 'd')
			{
				int32_t value;
				field.to_int(value);
				if (value > m_max)
				{
					m_field_overflow = true;
					result = ValidResult::REFUSED;
				}
			}
		}

		// If length of field setted
		if (m_length != UINT32_MAX)
		{
			uint32_t field_count = field.count();

			// If field too long
			if ((int32_t)field_count > m_length)
			{
				m_field_overflow = true;
				result = ValidResult::REFUSED;
			}
			// If field is empty
			else if (field_count == 0)
			{
				result = ValidResult::REFUSED;
			}
		}
		pos = i;
	}
	else
	{
		// Check the content of text with the mask
		if (m_mask_char != text.get(pos))
		{
			result = ValidResult::REFUSED;
		}
	}
	return result;
}

/** Abstract to get the display string */
void MaskValidator::display(const String & text, String & display)
{
	ValidResult result = ValidResult::ACCEPTED;
	uint32_t pos_mask = 0;
	uint32_t pos_text = 0;
	uint32_t count = text.count();

	display = text;

	for (pos_text = 0; pos_text < count && result == ValidResult::ACCEPTED; pos_text++)
	{
		result = get_mask_char(pos_mask);
		if (result == ValidResult::ACCEPTED)
		{
			result = check_text(text, pos_text);
		}
	}

	if (pos_mask < m_mask.count() && result == ValidResult::ACCEPTED)
	{
		count = m_mask.count();
		
		display = text;
		for (;pos_mask < count;)
		{
			if (get_mask_char(pos_mask) == ValidResult::ACCEPTED)
			{
				if ((m_mask_char & 0xFF00) == 0xF800)
				{
					if (m_length != UINT32_MAX)
					{
						for (uint32_t i = 0; i < m_length; i++)
						{
							display.write_char('_');
						}
					}
					else
					{
						display.write_char('_');
					}
				}
				else
				{
					display.write_char(m_mask_char);
				}
			}
		}
	}
}

/** Get the current separator or null if the field is not fully filled */
wchar_t MaskValidator::separator(const String & text)
{
	wchar_t result = '\0';
	ValidResult state = ValidResult::ACCEPTED;
	uint32_t pos_mask = 0;
	uint32_t pos_text = 0;
	uint32_t count = text.count();
	for (pos_text = 0; pos_text < count && state == ValidResult::ACCEPTED; pos_text++)
	{
		state = get_mask_char(pos_mask);
		if (state == ValidResult::ACCEPTED)
		{
			state = check_text(text, pos_text);
		}
	}

	if (m_field_overflow)
	{
		get_mask_char(pos_mask);
		if (m_mask_char < 0xF800)
		{
			result = m_mask_char;
		}
	}
	else
	{
		if (m_mask_char < 0xF800)
		{
			result = m_mask_char;
		}
	}
	return result;
}

/** Abstract function to check if the input field is correct */
Validator::ValidResult MaskValidator::validate(const String & text)
{
	ValidResult result = ValidResult::ACCEPTED;
	uint32_t pos_mask = 0;
	uint32_t pos_text = 0;
	uint32_t count = text.count();

	for (pos_text = 0; pos_text < count && result == ValidResult::ACCEPTED; pos_text++)
	{
		result = get_mask_char(pos_mask);
		if (result == ValidResult::ACCEPTED)
		{
			result = check_text(text, pos_text);
		}
	}
	return result;
}

#ifdef _DEBUG
// Unitary test
void MaskValidator::test1()
{
	{
		String mask("%d.%d");
		MaskValidator valid(mask);
		assert(valid.validate("3") == ValidResult::ACCEPTED);
	}
	{
		String mask("%z,,");
		MaskValidator valid(mask);
		assert(valid.validate("33,,") == ValidResult::REFUSED);
	}
	{
		String mask("%z,,");
		MaskValidator valid(mask);
		assert(valid.validate("33,,") == ValidResult::REFUSED);
	}
	{
		String mask("%d,,");
		MaskValidator valid(mask);
		assert(valid.validate("33,.") == ValidResult::REFUSED);
	}
	{
		String mask("%[6..3]d");
		MaskValidator valid(mask);
		assert(valid.validate("3") == ValidResult::ACCEPTED);
	}
	{
		String mask("%[3..6]2");
		MaskValidator valid(mask);
		assert(valid.validate("3") == ValidResult::REFUSED);
	}
	{
		String mask("%[3..d");
		MaskValidator valid(mask);
		assert(valid.validate("3") == ValidResult::REFUSED);
	}
	{
		String mask("%(3.d");
		MaskValidator valid(mask);
		assert(valid.validate("3") == ValidResult::REFUSED);
	}
	{
		String mask("%[3.d");
		MaskValidator valid(mask);
		assert(valid.validate("3") == ValidResult::REFUSED);
	}
	{
		String mask("%z,,");
		MaskValidator valid(mask);
		assert(valid.validate("3") == ValidResult::REFUSED);
	}
	{
		String mask("%[d");
		MaskValidator valid(mask);
		assert(valid.validate("3") == ValidResult::REFUSED);
	}
	{
		String mask("%%%d");
		MaskValidator valid(mask);
		assert(valid.validate("%3") == ValidResult::ACCEPTED);
	}
	{
		String mask("%d%%");
		MaskValidator valid(mask);
		assert(valid.validate("3%") == ValidResult::ACCEPTED);
	}
	{
		String mask("%[-5..3]d");
		MaskValidator valid(mask);
		assert(valid.validate("5-") == ValidResult::REFUSED);
		assert(valid.validate("-5") == ValidResult::ACCEPTED);
		assert(valid.validate("-6") == ValidResult::REFUSED);
		assert(valid.validate("3") == ValidResult::ACCEPTED);
		assert(valid.validate("4") == ValidResult::REFUSED);
		assert(valid.validate("0") == ValidResult::ACCEPTED);
		assert(valid.validate("-0") == ValidResult::ACCEPTED);
	}
	{
		String mask("%d%");
		MaskValidator valid(mask);
		assert(valid.validate("1.") == ValidResult::REFUSED);
	}
	{
		String mask("%L");
		MaskValidator valid(mask);
		assert(valid.validate("abcz") == ValidResult::REFUSED);
		assert(valid.validate("ABCZ") == ValidResult::ACCEPTED);
		assert(valid.validate("13") == ValidResult::REFUSED);
	}
	{
		String mask("%l");
		MaskValidator valid(mask);
		assert(valid.validate("abcz") == ValidResult::ACCEPTED);
		assert(valid.validate("ABCZ") == ValidResult::REFUSED);
		assert(valid.validate("13") == ValidResult::REFUSED);
	}
	{
		String mask("%2x-%2x");
		MaskValidator valid(mask);
		assert(valid.validate("12.12") == ValidResult::REFUSED);
	}
	{
		String mask("%x");
		MaskValidator valid(mask);
		assert(valid.validate("123abc256") == ValidResult::ACCEPTED);
		assert(valid.validate("123ABC256") == ValidResult::REFUSED);
	}
	{
		String mask("%X");
		MaskValidator valid(mask);
		assert(valid.validate("123abc256") == ValidResult::REFUSED);
		assert(valid.validate("123ABC256") == ValidResult::ACCEPTED);
	}
	{
		String mask("%A");
		MaskValidator valid(mask);
		assert(valid.validate("123abcz256") == ValidResult::REFUSED);
		assert(valid.validate("123ABCZ256") == ValidResult::ACCEPTED);
	}
	{
		String mask("%a");
		MaskValidator valid(mask);
		assert(valid.validate("123abcz256") == ValidResult::ACCEPTED);
		assert(valid.validate("123AbcZ256") == ValidResult::REFUSED);
	}
	{
		String mask("%3[0..255]d.%3[0..255]d.%3[0..255]d.%3[0..255]d");
		MaskValidator valid(mask);
		assert(valid.validate("256") == ValidResult::REFUSED);
	}
	{
		String mask("%02d");
		MaskValidator valid(mask);
		assert(valid.validate("0") == ValidResult::ACCEPTED);
		assert(valid.validate("02") == ValidResult::ACCEPTED);
		assert(valid.validate("1") == ValidResult::ACCEPTED);
		assert(valid.validate("12") == ValidResult::ACCEPTED);
		assert(valid.validate("023") == ValidResult::REFUSED);
	}
	{
		String mask("%3d.%3d.%3d.%3d");
		MaskValidator valid(mask);
		assert(valid.validate(".123.0") == ValidResult::REFUSED);
		assert(valid.validate("a") == ValidResult::REFUSED);
		assert(valid.validate("123.0") == ValidResult::ACCEPTED);
		assert(valid.validate("123.10") == ValidResult::ACCEPTED);
		assert(valid.validate("123.110") == ValidResult::ACCEPTED);
		assert(valid.validate("123.1101") == ValidResult::REFUSED);
		assert(valid.validate("123.011") == ValidResult::REFUSED);

		assert(valid.validate("123.110.") == ValidResult::ACCEPTED);
		assert(valid.validate("123.110.0") == ValidResult::ACCEPTED);
		assert(valid.validate("123.110.01") == ValidResult::REFUSED);
		assert(valid.validate("123.110.10") == ValidResult::ACCEPTED);
		assert(valid.validate("123.110.101") == ValidResult::ACCEPTED);
		assert(valid.validate("123.110.1012") == ValidResult::REFUSED);

		assert(valid.validate("123.110.101.1") == ValidResult::ACCEPTED);
		assert(valid.validate("123.110.101.12") == ValidResult::ACCEPTED);
		assert(valid.validate("123.110.101.123") == ValidResult::ACCEPTED);
		assert(valid.validate("123.110.101.1123") == ValidResult::REFUSED);
		assert(valid.validate("123.110.101.112.") == ValidResult::REFUSED);
		assert(valid.validate("123.110.1012") == ValidResult::REFUSED);

		assert(valid.validate("123.110.0") == ValidResult::ACCEPTED);
		assert(valid.validate("1") == ValidResult::ACCEPTED);
		assert(valid.validate("01") == ValidResult::REFUSED);
		assert(valid.validate("12") == ValidResult::ACCEPTED);
		assert(valid.validate("123") == ValidResult::ACCEPTED);
		assert(valid.validate("1234") == ValidResult::REFUSED);
		assert(valid.validate("123.") == ValidResult::ACCEPTED);
		
		assert(valid.validate("123.10") == ValidResult::ACCEPTED);
		assert(valid.validate("123.01") == ValidResult::REFUSED);
	}
	{
		String mask("%2d");
		MaskValidator valid(mask);
		assert(valid.validate("") == ValidResult::ACCEPTED);
		assert(valid.validate("1") == ValidResult::ACCEPTED);
		assert(valid.validate("01") == ValidResult::REFUSED);
		assert(valid.validate("12") == ValidResult::ACCEPTED);
		assert(valid.validate("123") == ValidResult::REFUSED);
	}
	{
		String mask("%d");
		MaskValidator valid(mask);
		assert(valid.validate("-01") == ValidResult::REFUSED);
		assert(valid.validate("0") == ValidResult::ACCEPTED);
		assert(valid.validate("01") == ValidResult::REFUSED);
		assert(valid.validate("012") == ValidResult::REFUSED);
		assert(valid.validate("1020") == ValidResult::ACCEPTED);
	}
}

// Unitary test
void MaskValidator::test()
{
	test1();
}
#endif


