#include "berialdraw_imp.hpp"

using namespace berialdraw;

ComboStyle::ComboStyle()
{
}

void ComboStyle::serialize(JsonIterator & it)
{
	it[StyleNames::COMBO_EDITABLE] = m_editable;
	it[StyleNames::COMBO_BUTTON_WIDTH] = m_button_width;
	it[StyleNames::COMBO_MAX_POPUP_HEIGHT] = m_max_popup_height;
}

void ComboStyle::unserialize(JsonIterator & it)
{
	m_editable = (bool)(it[StyleNames::COMBO_EDITABLE] | m_editable);
	if (it.exists(StyleNames::COMBO_BUTTON_WIDTH))
	{
		m_button_width = (uint16_t)(int)it[StyleNames::COMBO_BUTTON_WIDTH];
	}
	if (it.exists(StyleNames::COMBO_MAX_POPUP_HEIGHT))
	{
		m_max_popup_height = (uint16_t)(int)it[StyleNames::COMBO_MAX_POPUP_HEIGHT];
	}
}

ComboStyle& ComboStyle::operator=(const ComboStyle& other)
{
	set(other);
	return *this;
}

void ComboStyle::set(const ComboStyle & other)
{
	if (this != &other)
	{
		m_editable = other.m_editable;
		m_button_width = other.m_button_width;
		m_max_popup_height = other.m_max_popup_height;
		UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	}
}

Style * ComboStyle::create()
{
	return new ComboStyle;
}

void ComboStyle::editable(bool state)
{
	if (m_editable != state)
	{
		m_editable = state;
		UIManager::invalidator()->dirty(this, Invalidator::REDRAW);
	}
}

void ComboStyle::button_width(uint16_t pixels)
{
	if (m_button_width != pixels)
	{
		m_button_width = pixels;
		UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	}
}

void ComboStyle::max_popup_height(uint16_t pixels)
{
	if (m_max_popup_height != pixels)
	{
		m_max_popup_height = pixels;
		UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	}
}