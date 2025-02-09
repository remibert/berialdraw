#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
Invalidator::Invalidator()
{
}

/** Destructor */
Invalidator::~Invalidator()
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
		bool found = false;
		Widget * widget_with_shape = 0;
		for (uint32_t i = 0; i < m_widgets.size(); i++)
		{
			struct InvalidatorItem item = m_widgets[i];
			if ( (char*)object >= (char*)item.widget               && 
				((char*)object <= (char*)item.widget + item.size))
			{
				result = i;
				break;
			}

			if (item.shape)
			{
				if ( (char*)object >= (char*)item.shape               && 
					((char*)object <= (char*)item.shape + item.size))
				{
					widget_with_shape = m_widgets[i].widget;
					break;
				}
			}
		}
		if (widget_with_shape)
		{
			for (uint32_t i = 0; i < m_widgets.size(); i++)
			{
				if (m_widgets[i].shape == 0)
				{
					if (m_widgets[i].widget == widget_with_shape)
					{
						result = i;
						break;
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
	bool result = false;
	size_t size = m_widgets.size();
	for (uint32_t i = 0; i < size; i++)
	{
		if (m_widgets[i].widget == object)
		{
			if (m_widgets[i].status != NOTHING)
			{
				result = true;
			}
			break;
		}
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
				((char*)item.widget + item.size) <= ((char*)object + size))
			{
				m_widgets[i].status = NOTHING;
			}
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

		if (widget->parent() == 0)
		{
			item.window = 1;
		}
		else
		{
			item.window = 0;
		}
		item.widget = widget;
		item.shape = 0;
		item.size = (uint16_t)size;
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
	}
	else
	{
		uint32_t size = m_widgets.size();
		for (uint32_t i = 0; i < size; i++)
		{
			if (m_widgets[i].widget == widget && m_widgets[i].shape == 0)
			{
				m_widgets.remove(i);
				break;
			}
		}
	}
}
