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
		bool found = false;
		Widget * widget_with_shape = 0;
		for (uint32_t i = 0; i < m_widgets.size(); i++)
		{
			struct InvalidatorItem item = m_widgets[i];
			if ( (char*)object >= (char*)item.widget               && 
				((char*)object <= (char*)item.widget + item.size))
			{
				m_widgets[i].status = (Status)(m_widgets[i].status | status);
				found = true;
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
						m_widgets[i].status = (Status)(m_widgets[i].status | status);
						found = true;
						break;
					}
				}
			}
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
	for (uint32_t i = 0; i < size; i++)
	{
		if (m_widgets[i].status != NOTHING)
		{
			Widget * parent = m_widgets[i].widget->parent();
			while (parent)
			{
				dirty(parent, m_widgets[i].status);
				parent = parent->parent();
			}
		}
	}
}


/** Add widget in the list */
void Invalidator::add(Widget * widget, size_t size)
{
	struct InvalidatorItem item;
	item.widget = widget;
	item.shape = 0;
	item.size = size;
	m_widgets.push_back(item);
}

/** Add Shape in the list */
void Invalidator::add(Widget * widget, Shape * shape, size_t size)
{
	struct InvalidatorItem item;
	item.widget = widget;
	item.shape = shape;
	item.size = size;
	m_widgets.push_back(item);
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
