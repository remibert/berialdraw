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
void Invalidator::dirty(void * object)
{
	bool found = false;
	Widget * obj = dynamic_cast<Widget*>((Widget*)object);
	for (uint32_t i = 0; i < m_dirties.size(); i++)
	{
		if (m_dirties[i] == object)
		{
			found = true;
			break;
		}
	}

	if (found == false)
	{
		m_dirties.push_back(object);
	}
}

/** Check if object is dirty */
bool Invalidator::is_dirty(void * object)
{
	bool result = false;
	if (object)
	{
		for (uint32_t i = 0; i < m_dirties.size(); i++)
		{
			void * dirty = m_dirties[i];
			if (dirty == object)
			{
				result = true;
				break;
			}
		}
	}
	else
	{
		if (m_dirties.size()  > 0)
		{
			result = true;
		}
	}
	return result;
}

/** Clear all dirties object */
void Invalidator::clear(void * object)
{
	if (object == 0)
	{
		m_dirties.clear();
	}
	else
	{
		for (uint32_t i = 0; i < m_dirties.size(); i++)
		{
			if (m_dirties[i] == object)
			{
				m_dirties.remove(i);
				break;
			}
		}
	}
}
