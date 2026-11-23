#include "berialdraw_imp.hpp"

using namespace berialdraw;

// Create a text change event.
TextEvent::TextEvent(const String & previous_text, const String & text, Widget * widget) :
	Event(TextEvent::type_id()),
	m_previous_text(previous_text),
	m_text(text),
	m_widget(widget)
{
}

// Destroy the text change event.
TextEvent::~TextEvent()
{
}

// Return the previous text.
const String & TextEvent::previous_text() const
{
	return m_previous_text;
}

// Return the current text.
const String & TextEvent::text() const
{
	return m_text;
}

// Return the event source widget.
Widget * TextEvent::widget() const
{
	return m_widget;
}

// Return the event type identifier.
uint32_t TextEvent::type_id()
{
	static uint32_t result = 0;
	if (result == 0)
	{
		result = (uint32_t)typeid(TextEvent).hash_code();
	}
	return result;
}

// Serialize the event for logging.
void TextEvent::to_string(String & str) const
{
	str.print("{'type':'text_changed','previous_text':'%s','text':'%s'},",
		m_previous_text.c_str(), m_text.c_str());
}
