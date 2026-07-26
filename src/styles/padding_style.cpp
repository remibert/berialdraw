#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
PaddingStyle::PaddingStyle()
{
}

/** Serialize the content of widget into json */
void PaddingStyle::serialize(JsonIterator & it)
{
	m_padding.serialize (StyleNames::TEXT_PADDING,it);
}

/** Unserialize the content of widget from json */
void PaddingStyle::unserialize(JsonIterator & it)
{
	m_padding.unserialize (StyleNames::TEXT_PADDING,it);
}

/** Copy operator */
PaddingStyle& PaddingStyle::operator=(const PaddingStyle& other)
{
	set(other);
	return *this;
}

/** Set properties with another */
void PaddingStyle::set(const PaddingStyle & other)
{
	if (this != &other)
	{
		m_padding     = other.m_padding;
		UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	}
}

Style * PaddingStyle::create()
{
	return new PaddingStyle;
}

/** Set the padding */
void PaddingStyle::padding(const Margin & m)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_padding = m;
}

/** Set the padding in pixels */
void PaddingStyle::padding(Dim top, Dim left, Dim bottom, Dim right)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_padding.set(top,left,bottom,right);
}

/** Set the padding with a precision of 64th of a pixel */
void PaddingStyle::padding_q6(Dim top, Dim left, Dim bottom, Dim right)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_padding.set_q6(top,left,bottom,right);
}

/** Set the padding */
void PaddingStyle::padding(Dim horizontal, Dim vertical)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_padding.set(vertical,horizontal,vertical,horizontal);
}

/** Set the padding */
void PaddingStyle::padding(Dim value)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	m_padding.set(value,value,value,value);
}

