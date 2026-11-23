#include "berialdraw_imp.hpp"

using namespace berialdraw;

// Create a selection change event.
SelectionEvent::SelectionEvent(const Vector<int32_t> & previous_indices, const Vector<int32_t> & indices,
	Widget * widget) :
	Event(SelectionEvent::type_id()),
	m_previous_indices(previous_indices),
	m_indices(indices),
	m_widget(widget)
{
}

// Destroy the selection change event.
SelectionEvent::~SelectionEvent()
{
}

// Return the previously selected indices.
const Vector<int32_t> & SelectionEvent::previous_indices() const
{
	return m_previous_indices;
}

// Return the current selected indices.
const Vector<int32_t> & SelectionEvent::indices() const
{
	return m_indices;
}

// Return the event source widget.
Widget * SelectionEvent::widget() const
{
	return m_widget;
}

// Return the event type identifier.
uint32_t SelectionEvent::type_id()
{
	static uint32_t result = 0;
	if (result == 0)
	{
		result = (uint32_t)typeid(SelectionEvent).hash_code();
	}
	return result;
}

// Serialize the event for logging.
void SelectionEvent::to_string(String & str) const
{
	str += "{'type':'selection_changed','previous_indices':[";
	for (uint32_t index = 0; index < m_previous_indices.size(); index++)
	{
		String value;
		value.print("%d", m_previous_indices[index]);
		str += value;
		if (index + 1 < m_previous_indices.size())
		{
			str += ",";
		}
	}
	str += "],'indices':[";
	for (uint32_t index = 0; index < m_indices.size(); index++)
	{
		String value;
		value.print("%d", m_indices[index]);
		str += value;
		if (index + 1 < m_indices.size())
		{
			str += ",";
		}
	}
	str += "]},";
}
