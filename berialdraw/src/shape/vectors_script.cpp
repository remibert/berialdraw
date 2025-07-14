#include "berialdraw_imp.hpp"

using namespace berialdraw;
//#define TRACE

#define FLOAT_PART(value) ((((abs(value)%64)*1000)/64)+5)/10



VectorsScript::VectorsScript(const char* buffer, Polygon & polygon) :
	m_polygon(polygon)
{
	select(buffer);
}

VectorsScript::VectorsScript(Polygon & polygon) :
	m_polygon(polygon)
{
	select(0);
}

void VectorsScript::select(const char * buffer)
{
	m_pos_error = 0;
	m_error = VectorsScript::SUCCESS;
	m_command = 0;
	m_operator = 0;
	m_separator = 0;
	memset(&m_variables, 0, sizeof(m_variables));
	
	m_pos = 0;
	if(buffer)
	{
		m_buffer = new char[strlen(buffer) + 1];
		strcpy((char*)m_buffer, buffer);
	}
}

VectorsScript::~VectorsScript()
{
	if (m_buffer)
	{
		delete [] m_buffer;
	}
}

uint32_t VectorsScript::to_var(char letter)
{
	return  ((letter >= '0' && letter <= '9') ? letter - '0' :
			((letter >= 'A' && letter <= 'Z') ? letter - 'A' + 10 :
			((letter >= 'a' && letter <= 'z') ? letter - 'a' + 10 + 26 : -1)));
}

Coord VectorsScript::get(char letter)
{
	uint32_t index = to_var(letter);
	if (index < MAX_VARIABLES)
	{
		return m_variables[index];
	}
	else
	{
		set_error(VectorsScript::UNSUPPORTED_VARIABLES);
	}
	return 0;
}

void VectorsScript::set(char letter, Coord value)
{
	set_variable(to_var(letter), value);
}

void VectorsScript::set_variable(uint32_t index, Coord value)
{
	if (index < MAX_VARIABLES)
	{
		m_variables[index] = value;
	}
	else
	{
		set_error(VectorsScript::UNSUPPORTED_VARIABLES);
	}
}

Coord VectorsScript::get_variable(uint32_t index)
{
	if (index < MAX_VARIABLES)
	{
		return m_variables[index];
	}
	else
	{
		set_error(VectorsScript::UNSUPPORTED_VARIABLES);
	}
	return 0;
}

void VectorsScript::clear_params()
{
	m_operator = '+';
	m_separator = 0;
	for (int i = 0; i < 10; i++)
	{
		m_variables[i] = 0;
	}
}

Coord VectorsScript::operation(char m_operator, Coord param1, Coord param2)
{
	Coord result = param1;
	switch (m_operator)
	{
	case '+':
		result += param2;
		break;
	case '-':
		result -= param2;
		break;
	case '*':
		result *= param2;
		break;
	case '/':
		if (param2 != 0)
		{
			result /= param2;
		}
		else
		{
			set_error(VectorsScript::DIVIDE_BY_ZERO);
		}
		break;
	case '%':
		if (param2 != 0)
		{
			result %= param2;
		}
		else
		{
			set_error(VectorsScript::DIVIDE_BY_ZERO);
		}
		break;
	case '<':
		result <<= param2;
		break;
	case '>':
		result >>= param2;
		break;
	}
	return result;
}

char VectorsScript::get_char()
{
	return *m_pos;
}

void VectorsScript::next()
{
	m_pos++;
}

void VectorsScript::first()
{
	m_pos = m_buffer;
}

void VectorsScript::set_error(enum Error err)
{
#ifdef TRACE
	bd_printf("// !!! Error %d\n",err);
#endif

	m_error = err;
	m_pos_error = int32_t(m_pos - m_buffer);
}

void VectorsScript::parse_command(uint32_t max_params)
{
	uint32_t param = 0;
	char letter;
	clear_params();
	letter = get_char();
	while (letter != '\0' && letter  != ';' && m_error == VectorsScript::SUCCESS)
	{
#ifdef TRACE
		bd_printf("%c",letter); if(letter == '>' || letter == '<') bd_printf("%c",letter);
#endif
		// If operators
		if (letter == ',')
		{
			m_operator = '+';
			param++;
		}
		else if (letter == '+' || letter == '-' || letter == '*' ||
				 letter == '/' || letter == '%' || letter == '>' || letter == '<')
		{
			m_operator = letter;
		}
		// If numerical
		else if (letter >= '0' && letter <= '9')
		{
			char* last = 0;
			int32_t val = (int32_t)berialdraw::strtolong(m_pos, &last, 0);

			// If bad integer detected
			if (last == m_pos)
			{
				set_error(VectorsScript::BAD_INTEGER);
				break;
			}
			else
			{
#ifdef TRACE
				char backup = *last;*last = '\0';bd_printf("%s", &m_pos[1]);*last = backup;
#endif
				// If parameters accepted
				if (param < max_params)
				{
					Coord v;
					v = val;
					set_variable(param, operation(m_operator, get_variable(param), v));
					m_pos = last - 1;
				}
				else
				{
					set_error(VectorsScript::TOO_MANY_PARAMETERS);
					break;
				}
			}
		}
		// If variable or parameters
		else if (to_var(letter) > 0)
		{
			// If parameters accepted
			if (param < max_params)
			{
				set_variable(param, operation(m_operator, get_variable(param), get(letter)));
			}
			else
			{
				set_error(VectorsScript::TOO_MANY_PARAMETERS);
				break;
			}
		}
		else
		{
			set_error(VectorsScript::TOO_MANY_PARAMETERS);
			break;
		}
		next();
		letter = get_char();
	}
	next();
}

enum VectorsScript::Error VectorsScript::parse()
{
	m_pos = m_buffer;
	m_polygon.clear();
	if (m_buffer)
	{
		char letter = get_char();
		while (letter != '\0' && m_error == VectorsScript::SUCCESS)
		{
			m_command = letter;

			// If variable set
			if (to_var(letter) != -1)
			{
				next();
				if (get_char() != '=')
				{
					set_error(VectorsScript::BAD_COMMAND);
				}
				else
				{
#ifdef TRACE
					bd_printf("%c = ",letter);
#endif
					next();
					parse_command(1);
					set(letter, get_variable(0));
#ifdef TRACE
					bd_printf(";     // %c = %d.%02d\n", letter, get_variable(0) / 64, FLOAT_PART(get_variable(0)));
#endif
				}
			}
			// Set point
			else if (letter == '.')
			{
#ifdef TRACE
				bd_printf("poly.add_point(");
#endif
				next();
				parse_command(2);
				Point p;
				p.x_(get_variable(0));
				p.y_(get_variable(1));
				m_polygon.add_point(p);
#ifdef TRACE
				bd_printf(");     // %d.%02d, %d.%02d\n", get_variable(0) / 64, FLOAT_PART(get_variable(0)), get_variable(1) / 64, FLOAT_PART(get_variable(1)));
#endif
			}
			// Set conic
			else if (letter == '^')
			{
#ifdef TRACE
				bd_printf("poly.add_conic(");
#endif
				next();
				parse_command(2);
				Point p;
				p.x_(get_variable(0));
				p.y_(get_variable(1));
				m_polygon.add_conic(p);
#ifdef TRACE
				bd_printf(");     // %d.%02d, %d.%02d\n", get_variable(0) / 64, FLOAT_PART(get_variable(0)), get_variable(1) / 64, FLOAT_PART(get_variable(1)));
#endif
			}
			// Set cubic
			else if (letter == '$')
			{
#ifdef TRACE
				bd_printf("poly.add_cubic(");
#endif
				next();
				parse_command(2);
				Point p;
				p.x_(get_variable(0));
				p.y_(get_variable(1));
				m_polygon.add_cubic(p);
#ifdef TRACE
				bd_printf(");     // %d.%02d, %d.%02d\n", get_variable(0) / 64, FLOAT_PART(get_variable(0)), get_variable(1) / 64, FLOAT_PART(get_variable(1)));
#endif
			}
			// Next contour
			else if (letter == '~')
			{
#ifdef TRACE
				bd_printf("poly.next_contour();\n");
#endif
				m_polygon.next_contour();
				next();
				next();
			}
			else
			{
				set_error(VectorsScript::BAD_COMMAND);
			}
			letter = get_char();
		}
	}
	return m_error;
}

uint32_t VectorsScript::count_points()
{
	uint32_t result = 0;
	m_pos = m_buffer;
	char letter = get_char();
	while (letter)
	{
		if (letter == '.' || letter == '^' || letter == '$')
		{
			result ++;
		}
		next();
		letter = get_char();
	}
	return result;
}

uint32_t VectorsScript::count_contours()
{
	uint32_t result = 1;
	m_pos = m_buffer;
	char letter = get_char();
	while (letter)
	{
		if (letter == '~')
		{
			result++;
		}
		next();
		letter = get_char();
	}
	return result;
}

