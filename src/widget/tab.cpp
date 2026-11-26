#include "berialdraw_imp.hpp"

using namespace berialdraw;

// Create a tab button using the dedicated tab style.
Tab::Tab(Widget * parent):
	Button(StyleNames::TYPE_TAB, parent, sizeof(Tab))
{
	focusable(false);
}

// Rotate a margin clockwise for a tab position, preserving Q6 precision.
Margin Tab::rotate_margin(const Margin & margin, TabPosition position)
{
	Margin result;
	switch (position)
	{
	case TabPosition::RIGHT:
		result.set_q6(margin.left_q6(), margin.bottom_q6(), margin.right_q6(), margin.top_q6());
		break;
	case TabPosition::BOTTOM:
		result.set_q6(margin.bottom_q6(), margin.right_q6(), margin.top_q6(), margin.left_q6());
		break;
	case TabPosition::LEFT:
		result.set_q6(margin.right_q6(), margin.top_q6(), margin.left_q6(), margin.bottom_q6());
		break;
	case TabPosition::TOP:
	default:
		result = margin;
		break;
	}
	return result;
}

// Rotate border sides clockwise for a tab position.
Borders Tab::rotate_borders(Borders borders, TabPosition position)
{
	uint16_t rotation = position == TabPosition::RIGHT ? 1 :
		position == TabPosition::BOTTOM ? 2 :
		position == TabPosition::LEFT ? 3 : 0;
	uint16_t value = static_cast<uint16_t>(borders);
	uint16_t sides = value & static_cast<uint16_t>(ALL_BORDERS);
	uint16_t decorations = value & ~static_cast<uint16_t>(ALL_BORDERS);
	for (uint16_t index = 0; index < rotation; index++)
	{
		sides = ((sides << 1) & static_cast<uint16_t>(ALL_BORDERS)) | (sides >> 3);
	}
	return static_cast<Borders>(sides | decorations);
}

// Rotate widget alignment clockwise for a tab position.
Align Tab::rotate_align(Align align, TabPosition position)
{
	uint8_t rotation = position == TabPosition::RIGHT ? 1 :
		position == TabPosition::BOTTOM ? 2 :
		position == TabPosition::LEFT ? 3 : 0;
	uint8_t value = static_cast<uint8_t>(align);
	uint8_t sides = value & static_cast<uint8_t>(Align::ALIGN_DEFAULT);
	uint8_t decorations = value & ~static_cast<uint8_t>(Align::ALIGN_DEFAULT);
	for (uint8_t index = 0; index < rotation; index++)
	{
		uint8_t rotated = 0;
		if (sides & static_cast<uint8_t>(Align::ALIGN_LEFT))
		{
			rotated |= static_cast<uint8_t>(Align::ALIGN_TOP);
		}
		if (sides & static_cast<uint8_t>(Align::ALIGN_TOP))
		{
			rotated |= static_cast<uint8_t>(Align::ALIGN_RIGHT);
		}
		if (sides & static_cast<uint8_t>(Align::ALIGN_RIGHT))
		{
			rotated |= static_cast<uint8_t>(Align::ALIGN_BOTTOM);
		}
		if (sides & static_cast<uint8_t>(Align::ALIGN_BOTTOM))
		{
			rotated |= static_cast<uint8_t>(Align::ALIGN_LEFT);
		}
		sides = rotated;
	}
	return static_cast<Align>(sides | decorations);
}

// Reapply the base tab style and orient its margin and borders.
void Tab::position(TabPosition position)
{
	if (m_position != position)
	{
		String title = text();
		UIManager::styles()->apply(this, (CommonStyle *)this);
		UIManager::styles()->apply(this, (BorderStyle *)this);
		UIManager::styles()->apply(this, (TextStyle *)this);
		UIManager::styles()->apply(this, (PaddingStyle *)this);
		text(title);
		m_position = position;
		margin(rotate_margin(margin(), position));
		borders(rotate_borders((Borders)borders(), position));
		align(rotate_align(align(), position));
		UIManager::invalidator()->dirty(this, Invalidator::ALL);
	}
}