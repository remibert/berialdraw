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
