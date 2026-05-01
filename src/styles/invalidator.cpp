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
	, m_deleted_count(0)
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
			if (m_widgets[m_last_search_index].widget != nullptr)  // Verify not deleted
			{
				result = m_last_search_index;
			}
			else
			{
				m_last_search_object = 0;
				m_last_search_index = -1;
			}
		}
		else
		{
			Widget * widget_with_shape = nullptr;
			uint32_t search_size = m_widgets.size();
			
			// Start from last position for better locality
			uint32_t start_pos = (m_last_search_index >= 0) ? (uint32_t)m_last_search_index : 0;
			
			// Search from last position to end
			for (uint32_t i = start_pos; i < search_size && result == -1; i++)
			{
				struct InvalidatorItem item = m_widgets[i];

				// Skip deleted entries
				if (item.widget == nullptr)
					continue;

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
				}
				// Check if object is in shape range if shape exists
				else if (item.shape && is_in_range(object, item.shape, item.size))
				{
					widget_with_shape = item.widget;
				}
			}
			
			// If not found after last position, search from beginning
			if (result == -1 && start_pos > 0)
			{
				for (uint32_t i = 0; i < start_pos && result == -1; i++)
				{
					struct InvalidatorItem item = m_widgets[i];
					
					// Skip deleted entries
					if (item.widget == nullptr)
						continue;
					
					// Check if object is in widget range
					if (is_in_range(object, item.widget, item.size))
					{
						result = i;
						m_last_search_object = object;
						m_last_search_index = result;
					}
					// Check if object is in shape range if shape exists
					else if (item.shape && is_in_range(object, item.shape, item.size))
					{
						widget_with_shape = item.widget;
					}
				}
			}
			
			// If we found a shape, search for its container widget
			if (result == -1 && widget_with_shape)
			{
				for (uint32_t i = 0; i < search_size && result == -1; i++)
				{
					if (m_widgets[i].shape == nullptr && m_widgets[i].widget == widget_with_shape)
					{
						result = i;
						m_last_search_object = object;
						m_last_search_index = result;
					}
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
		if (m_widgets[i].widget != nullptr && m_widgets[i].status != NOTHING)
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
			if (m_widgets[i].widget != nullptr)
			{
				m_widgets[i].status = NOTHING;
			}
		}
	}
	else
	{
		uint32_t size = m_widgets.size();
		for (uint32_t i = 0; i < size; i++)
		{
			struct InvalidatorItem item = m_widgets[i];
			if (item.widget != nullptr &&
				(char*)item.widget >= (char*)object && 
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
		if (m_widgets[i].widget != nullptr && m_widgets[i].window_id == window_id)
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
	bool scroll = false;
	for (uint32_t i = 0; i < size; i++)
	{
		if (m_widgets[i].widget != nullptr && m_widgets[i].status != NOTHING)
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
			if (m_widgets[i].widget != nullptr &&
				(m_widgets[i].status & GEOMETRY) && (m_widgets[i].window == 1))
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
			if (m_widgets[i].widget != nullptr && m_widgets[i].shape == shape)
			{
				// Mark as deleted instead of removing
				m_widgets[i].widget = nullptr;
				m_deleted_count++;
				
				// Compact when too many deletions
				if (m_deleted_count >= 1000)
				{
					compact();
				}
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
		m_deleted_count = 0;
	}
	else
	{
		uint32_t size = m_widgets.size();
		for (uint32_t i = 0; i < size; i++)
		{
			if (m_widgets[i].widget == widget && m_widgets[i].shape == 0)
			{
				// Invalidate cache if necessary
				if (i < (uint32_t)m_last_search_index)
				{
					m_last_search_index--;
				}
				else if (i == (uint32_t)m_last_search_index)
				{
					m_last_search_object = 0;
					m_last_search_index = -1;
				}

				// Mark as deleted instead of removing
				m_widgets[i].widget = nullptr;
				m_deleted_count++;
				

				// Compact when too many deletions
				if (m_deleted_count >= 5000)
				{
					compact();
				}
				break;
			}
		}
	}
}

/** Compact the vector by removing null entries */
void Invalidator::compact()
{
	// In-place compaction: move non-null elements forward, overwriting nulls
	uint32_t write_pos = 0;  // Position où écrire le prochain élément valide
	uint32_t size = m_widgets.size();
	
	for (uint32_t read_pos = 0; read_pos < size; read_pos++)
	{
		// Si élément non-null, le déplacer à write_pos
		if (m_widgets[read_pos].widget != nullptr)
		{
			if (write_pos != read_pos)
			{
				m_widgets[write_pos] = m_widgets[read_pos];
			}
			write_pos++;
		}
	}
	
	// Réduire la taille du vector
	m_widgets.resize(write_pos);
	m_deleted_count = 0;
	
	// Reset cache since indices changed
	m_last_search_object = 0;
	m_last_search_index = -1;
}

