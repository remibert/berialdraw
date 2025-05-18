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

Size::~Size()
{
}

Size& Size::operator=(const Size& other)
{
	if (this != &other)
	{
		this->m_width = other.m_width;
		this->m_height = other.m_height;
		this->m_width_undefined = other.m_width_undefined;
		this->m_height_undefined = other.m_height_undefined;
	}
	return *this;
}

void Size::set(Dim w, Dim h)
{
	width(w);
	height(h);
}

void Size::set_(Dim w, Dim h)
{
	m_width  = w;
	m_height = h;
	m_width_undefined = 0;
	m_height_undefined = 0;
}

Dim Size::height() const
{
	return (m_height+63) >> 6;
}

Dim Size::width() const
{
	return (m_width+63) >> 6;
}

void Size::height(Dim h)
{
	if (h != Size::MAX_SIZE)
	{
		m_height = h << 6;
	}
	else
	{
		m_height = h;
	}
	m_height_undefined = 0;
}

void Size::width(Dim w)
{
	if (w != Size::MAX_SIZE)
	{
		m_width  = w << 6;
	}
	else
	{
		m_width = w;
	}
	m_width_undefined = 0;
}

Dim Size::height_() const
{
	return m_height;
}

Dim Size::width_() const
{
	return m_width;
}

void Size::height_(Dim h)
{
	m_height = h;
	m_height_undefined = 0;
}

void Size::width_(Dim w)
{
	m_width = w;
	m_width_undefined = 0;
}

Size Size::middle() const
{
	Size  m(m_width>>1, m_height>>1, false);
	return m;
}

bool Size::operator==(const Size& other) const
{
	return m_width == other.m_width && m_height == other.m_height;
}

bool Size::operator!=(const Size& other) const
{
	return m_width != other.m_width || m_height != other.m_height;
}


/** Decrease size with width and height in pixels */
void Size::decrease(const Margin & margin)
{
	decrease_(margin.left_() + margin.right_(), margin.top_() + margin.bottom_());
}

/** Increase size with width and height in pixels */
void Size::increase(const Margin & margin)
{
	increase_(margin.left_() + margin.right_(), margin.top_() + margin.bottom_());
}

/** Decrease size with width and height in pixels */
void Size::decrease(const Size & size)
{
	decrease_(size.width_(), size.height_());
}

/** Increase size with width and height in pixels */
void Size::increase(const Size & size)
{
	increase(size.width_(), size.height_());
}

/** Decrease size with width and height in pixels */
void Size::decrease(Dim w, Dim h)
{
	decrease_(w<<6, h<<6);
}

/** Increase size with width and height in pixels */
void Size::increase(Dim w, Dim h)
{
	increase_(w<<6, h<<6);
}

/** Decrease size with width and height with a precision of 64th of a pixel */
void Size::decrease_(Dim w, Dim h)
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
void Size::increase_(Dim w, Dim h)
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
	Dim width = m_width;
	m_width_undefined = berialdraw::unserialize("width_",field,width);
	m_width = width;

	Dim height = m_height;
	m_height_undefined = berialdraw::unserialize("height_",field,height);
	m_height = height;
}

/** Resizes itself on the nearest pixel */
void Size::nearest_pixel()
{
	m_width  = ((m_width  + 32) >> 6) << 6;
	m_height = ((m_height + 32) >> 6) << 6;
	m_width_undefined = 0;
	m_height_undefined = 0;
}

/** Indicates if width is not defined */
bool Size::is_width_undefined() const
{
	return m_width_undefined;
}

/** Indicates if height is not defined */
bool Size::is_height_undefined() const
{
	return m_height_undefined;
}

/** Clean the size and set to undefined */
void Size::clean()
{
	m_width = 0;
	m_height = 0;
	m_width_undefined = 1;
	m_height_undefined = 1;
}

/** Print content */
void Size::print(const char * name) const
{
#define FLOAT_PART(value) (((((value)%64)*1000)/64)+5)/10
	bd_printf("%s w=%d.%02d h=%d.%02d\n",name, m_width/64, FLOAT_PART(m_width), m_height/64, FLOAT_PART(m_height));
}

/** Substract b to a and avoid negative value */
Dim Size::subs(Dim a, Dim b)
{
	if (b >= a)
	{
		return 0;
	}
	return a-b;
}
