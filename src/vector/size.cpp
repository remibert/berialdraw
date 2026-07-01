#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Create a size */
Size::Size()
{
	m_width = 0;
	m_height = 0;
	m_width_undefined = 1;
	m_height_undefined = 1;
}

Size::Size(Dim w, Dim h, bool pixel)
{
	m_width = 0;
	m_height = 0;
	if(w == Size::MAX_SIZE)
	{
		m_width = Size::MAX_SIZE;
	}
	else
	{
		m_width  = pixel ? (w << 6) : w;
	}
	m_width_undefined = 0;

	if(h == Size::MAX_SIZE)
	{
		m_height = Size::MAX_SIZE;
	}
	else
	{
		m_height = pixel ? (h << 6) : h;
	}
	m_height_undefined = 0;
}

Size::Size(const Size& p) : 
	m_width(p.m_width), 
	m_height(p.m_height),
	m_width_undefined(p.m_width_undefined),
	m_height_undefined(p.m_height_undefined)
{
}

/** Decrease size with width and height in pixels */
void Size::decrease(const Margin & margin)
{
	decrease_q6(margin.left_q6() + margin.right_q6(), margin.top_q6() + margin.bottom_q6());
}

/** Increase size with width and height in pixels */
void Size::increase(const Margin & margin)
{
	increase_q6(margin.left_q6() + margin.right_q6(), margin.top_q6() + margin.bottom_q6());
}

/** Decrease size with width and height with a precision of 64th of a pixel */
void Size::decrease_q6(Dim w, Dim h)
{
	if(m_width == MAX_SIZE || w == MAX_SIZE)
	{
		m_width = MAX_SIZE;
	}
	else
	{
		if(m_width >= w)
		{
			m_width -= w;
		}
		else
		{
			m_width = 0;
		}
	}
	m_width_undefined = 0;

	if(m_height == MAX_SIZE || h == MAX_SIZE)
	{
		m_height = MAX_SIZE;
	}
	else
	{
		if(m_height >= h)
		{
			m_height -= h;
		}
		else
		{
			m_height = 0;
		}
	}
	m_height_undefined = 0;
}

/** Increase size with width and height with a precision of 64th of a pixel */
void Size::increase_q6(Dim w, Dim h)
{
	if(m_width == MAX_SIZE || w == MAX_SIZE)
	{
		m_width = MAX_SIZE;
	}
	else
	{
		m_width  += w;
	}
	m_width_undefined = 0;

	if(m_height == MAX_SIZE || h == MAX_SIZE)
	{
		m_height = MAX_SIZE;
	}
	else
	{
		m_height += h;
	}
	m_height_undefined = 0;
}

void Size::serialize(const char * name, JsonIterator & it) const
{
	int width  = m_width  != MAX_SIZE  ? m_width  : m_width;
	int height = m_height != MAX_SIZE  ? m_height : m_height;
	it[name]["width_"]  = width ;
	it[name]["height_"] = height;
}

void Size::unserialize(const char * name, JsonIterator & it)
{
	JsonIterator field = it[name];
	if (field.type() == JsonType::INT)
	{
		int value = field;
		m_width = value<<6;
		m_height = value<<6;
		m_width_undefined = 0;
		m_height_undefined = 0;
	}
	else if (field.type() == JsonType::OBJECT)
	{
		Dim width = m_width;
		m_width_undefined = berialdraw::unserialize("width_",field,width);
		m_width = width;

		Dim height = m_height;
		m_height_undefined = berialdraw::unserialize("height_",field,height);
		m_height = height;
	}
}

/** Print content */
void Size::print(const char * name) const
{
	bd_printf("%s w=%s h=%s", name, (const char*)format_dim(m_width), (const char*)format_dim(m_height));
}





