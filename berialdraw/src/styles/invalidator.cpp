#include "berialdraw_imp.hpp"

using namespace berialdraw;

#define PRIME1 63689
#define PRIME2 378551

/** Generate hash code from a pointer value
@param ptr pointer value
@return hash code */
static inline uint16_t pointer_hash(const void * ptr)
{
	uint16_t hash = PRIME1;
	uint8_t * data = (uint8_t*)&ptr;
	size_t len = sizeof(ptr);
	
	for (size_t i = 0; i < len; i++)
	{
		hash = hash * PRIME2 + data[i];
	}
	
	return hash;
}

/** Helper: check if object is in range of item */
static inline bool is_in_range(const void * object, const void * widget, uint16_t size)
{
	return (char*)object >= (char*)widget && ((char*)object < (char*)widget + size);
}

/** Constructor */
Invalidator::Invalidator()
	: m_last_search_object(0)
	, m_last_search_index(-1)
{
}

/** Add dirty object */
void Invalidator::dirty(Widget * object, enum Status status)
{
	dirty((void*)object,status);
}

/** Add dirty object */
void Invalidator::dirty(Style * object, enum Status status)
{
	dirty((void*)object,status);
}


/** Add dirty object */
void Invalidator::dirty(void * object, enum Status status)
{
	if (object)
	{
		int32_t i = search(object);
		if (i >= 0)
		{
			m_widgets[i].status = (Status)(m_widgets[i].status | status);
		}
	}
}

/** Remove dirty object */
void Invalidator::undirty(Widget * object, enum Status status)
{
	undirty((void*)object,status);
}

/** Return the index in table or -1 if not found */
int32_t Invalidator::search(void * object)
{
	int32_t result = -1;
	if (object)
	{
		// Check cache: if same object, return immediately (fast path for sequential access)
		if (object == m_last_search_object && m_last_search_index >= 0)
		{
			return m_last_search_index;
		}
		
		Widget * widget_with_shape = 0;
		uint32_t search_size = m_widgets.size();
		
		// Start from last position for better locality
		uint32_t start_pos = (m_last_search_index >= 0) ? (uint32_t)m_last_search_index : 0;
		
		// Search from last position to end
		for (uint32_t i = start_pos; i < search_size; i++)
		{
			struct InvalidatorItem item = m_widgets[i];

			// Check if object is in widget range
			if (is_in_range(object, item.widget, item.size))
			{
				if (object != m_widgets[i].widget)
				{
					is_in_range(object, item.widget, item.size);
				}
				
				result = i;
				m_last_search_object = object;
				m_last_search_index = result;
				return result;
			}

			// Check if object is in shape range if shape exists
			if (item.shape && is_in_range(object, item.shape, item.size))
			{
				widget_with_shape = item.widget;
				break;
			}
		}
		
		// If not found after last position, search from beginning
		if (result == -1 && start_pos > 0)
		{
			for (uint32_t i = 0; i < start_pos; i++)
			{
				struct InvalidatorItem item = m_widgets[i];
				// Check if object is in widget range
				if (is_in_range(object, item.widget, item.size))
				{
					result = i;
					m_last_search_object = object;
					m_last_search_index = result;
					return result;
				}

				// Check if object is in shape range if shape exists
				if (item.shape && is_in_range(object, item.shape, item.size))
				{
					widget_with_shape = item.widget;
					break;
				}
			}
		}
		
		// If we found a shape, search for its container widget
		if (widget_with_shape)
		{
			for (uint32_t i = 0; i < search_size; i++)
			{
				if (m_widgets[i].shape == 0 && m_widgets[i].widget == widget_with_shape)
				{
					result = i;
					m_last_search_object = object;
					m_last_search_index = result;
					return result;
				}
			}
		}
	}
	return result;
}

/** Remove dirty object */
void Invalidator::undirty(void * object, enum Status status)
{
	if (object)
	{
		int32_t i = search(object);
		if (i >= 0)
		{
			m_widgets[i].status = (Status)(m_widgets[i].status & ~status);
		}
	}
}

/** Check if object is dirty */
bool Invalidator::is_dirty(Widget * object)
{
	return status(object) != NOTHING;
}

/** Check if object is dirty */
enum Invalidator::Status Invalidator::status(Widget * object)
{
	enum Status result = NOTHING;
	int32_t i = search((void*)object);
	if (i >= 0)
	{
		result = (enum Status)m_widgets[i].status;
	}
	return result;
}

/** Check if something must be refreshed */
bool Invalidator::is_dirty()
{
	bool result = false;
	size_t size = m_widgets.size();

	for (uint32_t i = 0; i < size; i++)
	{
		if (m_widgets[i].status != NOTHING)
		{
			update();
			result = true;
			break;
		}
	}
	return result;
}

/** Clear all dirties object */
void Invalidator::clear(void * object)
{
	if (object == 0)
	{
		uint32_t size = m_widgets.size();
		for (uint32_t i = 0; i < size; i++)
		{
			m_widgets[i].status = NOTHING;
		}
	}
	else
	{
		uint32_t size = m_widgets.size();
		for (uint32_t i = 0; i < size; i++)
		{
			struct InvalidatorItem item = m_widgets[i];
			if ((char*)item.widget >= (char*)object && 
				((char*)item.widget + item.size) <= ((char*)object + item.size))
			{
				m_widgets[i].status = NOTHING;
				break;
			}
		}
	}
}

/** Clear all dirties object for a specific window */
void Invalidator::window_clean_all(void * window)
{
	uint16_t window_id = pointer_hash(window);
	uint32_t size = m_widgets.size();
	for (uint32_t i = 0; i < size; i++)
	{
		if (m_widgets[i].window_id == window_id)
		{
			m_widgets[i].status = NOTHING;
		}
	}
}

/** Updates parents that a child widget is invalidate */
void Invalidator::update()
{
	uint32_t size = m_widgets.size();
	bool geometry_window = false;
	for (uint32_t i = 0; i < size; i++)
	{
		if (m_widgets[i].status != NOTHING)
		{
			if (m_widgets[i].status & GEOMETRY)
			{
				dirty(m_widgets[i].widget->root(),GEOMETRY);
				geometry_window = true;
			}
		}
	}

	if (geometry_window)
	{
		uint32_t size = m_widgets.size();
		for (uint32_t i = 0; i < size; i++)
		{
			if ((m_widgets[i].status & GEOMETRY) && (m_widgets[i].window == 1))
			{
				m_widgets[i].widget->dirty_children(GEOMETRY);
			}
		}
	}
}

/** Add widget in the list */
void Invalidator::add(Widget * widget, size_t size)
{
	if (widget && size)
	{
		struct InvalidatorItem item;
		Widget * window = widget->root();
		if (window == widget)
		{
			window = widget;
			item.window = 1;
		}
		else
		{
			item.window = 0;
		}
		item.widget = widget;
		item.shape = 0;
		item.size = (uint16_t)size;
		item.window_id = pointer_hash(window);
		item.status = Invalidator::ALL;
		m_widgets.push_back(item);
	}
}

/** Add Shape in the list */
void Invalidator::add(Widget * widget, Shape * shape, size_t size)
{
	if (widget && size)
	{
		struct InvalidatorItem item;
		item.widget = widget;
		item.shape = shape;
		item.window = 0;
		item.size = (uint16_t)size;
		item.status = Invalidator::ALL;
		m_widgets.push_back(item);
	}
}

/** Remove shape from the list */
void Invalidator::remove(Shape * shape)
{
	if (shape)
	{
		uint32_t size = m_widgets.size();
		for (uint32_t i = 0; i < size; i++)
		{
			if (m_widgets[i].shape == shape)
			{
				m_widgets.remove(i);
				break;
			}
		}
	}
}

/** Remove widget from the list */
void Invalidator::remove(Widget * widget)
{
	if (widget == 0)
	{
		m_widgets.clear();
		m_last_search_object = 0;
		m_last_search_index = -1;
	}
	else
	{
		uint32_t size = m_widgets.size();
		for (uint32_t i = 0; i < size; i++)
		{
			if (m_widgets[i].widget == widget && m_widgets[i].shape == 0)
			{
				// Optimize cache invalidation: only reset if necessary
				if (i < (uint32_t)m_last_search_index)
				{
					// Removed something before cache position - adjust index
					m_last_search_index--;
				}
				else if (i == (uint32_t)m_last_search_index)
				{
					// Removed the cached widget - full reset
					m_last_search_object = 0;
					m_last_search_index = -1;
				}

				m_widgets.remove(i);
				break;
			}
		}
	}
}
