#include "berialdraw_imp.hpp"

using namespace berialdraw;

Margin::Margin(int v)
{
	set(v,v,v,v);
}

Margin::Margin(Dim t, Dim l, Dim b, Dim r, bool pixel)
{
	if(pixel)
	{
		set(t,l,b,r);
	}
	else
	{
		set_(t,l,b,r);
	}
}

Margin::Margin(const Margin& p) : m_top(p.m_top), m_left(p.m_left),m_bottom(p.m_bottom),m_right(p.m_right)
{
}

Margin::~Margin()
{
}

bool Margin::operator==(const Margin& other) const
{
	return m_top == other.m_top && m_left == other.m_left && m_bottom == other.m_bottom && m_right == other.m_right;
}

bool Margin::operator!=(const Margin& other) const
{
	return m_top != other.m_top || m_left != other.m_left || m_bottom != other.m_bottom || m_right != other.m_right;
}

Margin& Margin::operator=(const Margin& other)
{
	if (this != &other)
	{
		m_top = other.m_top;
		m_left = other.m_left;
		m_bottom = other.m_bottom;
		m_right = other.m_right;
	}
	return *this;
}

void Margin::set(Dim t, Dim l, Dim b, Dim r)
{
	top(t);
	left(l);
	bottom(b);
	right(r);
}

void Margin::top(Dim t)
{
	m_top = t << 6;
}

void Margin::left(Dim l)
{
	m_left = l << 6;
}

void Margin::bottom(Dim b)
{
	m_bottom = b << 6;
}

void Margin::right(Dim r)
{
	m_right = r << 6;
}
	
Dim Margin::top() const
{
	return m_top >> 6;
}

Dim Margin::left() const
{
	return m_left >> 6;
}

Dim Margin::bottom() const
{
	return m_bottom >> 6;
}

Dim Margin::right() const
{
	return m_right >> 6;
}

void Margin::set_(Dim t, Dim l, Dim b, Dim r)
{
	top_(t);
	left_(l);
	bottom_(b);
	right_(r);
}

Dim Margin::top_() const
{
	return m_top;
}

Dim Margin::left_() const
{
	return m_left;
}

Dim Margin::bottom_() const
{
	return m_bottom;
}

Dim Margin::right_() const
{
	return m_right;
}

void Margin::top_(Dim t)
{
	m_top = t;
}

void Margin::left_(Dim l)
{
	m_left = l;
}

void Margin::bottom_(Dim b)
{
	m_bottom = b;
}

void Margin::right_(Dim r)
{
	m_right = r;
}

void Margin::serialize(const char * name, JsonIterator & it) const
{
	it[name]["top_"]    = (int)(m_top);
	it[name]["left_"]   = (int)(m_left);
	it[name]["bottom_"] = (int)(m_bottom);
	it[name]["right_"]  = (int)(m_right);
}

void Margin::unserialize(const char * name, JsonIterator & it)
{
	int top     = it[name]["top_"]    | Size::MAX_SIZE;
	int left    = it[name]["left_"]   | Size::MAX_SIZE;
	int bottom  = it[name]["bottom_"] | Size::MAX_SIZE;
	int right   = it[name]["right_"]  | Size::MAX_SIZE;

	m_top     = (top    == Size::MAX_SIZE) ? m_top    : top   ;
	m_left    = (left   == Size::MAX_SIZE) ? m_left   : left  ;
	m_bottom  = (bottom == Size::MAX_SIZE) ? m_bottom : bottom;
	m_right   = (right  == Size::MAX_SIZE) ? m_right  : right ;
}
