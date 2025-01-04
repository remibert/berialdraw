#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
Mapping::Mapping()
{
}

/** Destructor */
Mapping::~Mapping()
{
}

/** Get id of mapping */
const String & Mapping::name() const
{
	return m_name;
}

/** Get mapping */
const String & Mapping::mapping() const
{
	return m_mapping;
}

/** Serialize the content of widget into json */
void Mapping::serialize(JsonIterator & it)
{
	it["name"]    = m_name;
	it["mapping"] = m_mapping;
}

/** Unserialize the content of widget from json */
void Mapping::unserialize(JsonIterator & it)
{
	m_name    = it["name"]    | "";
	m_mapping = it["mapping"] | "";
}




/** Constructor */
Mappings::Mappings()
{
}

/** Destructor */
Mappings::~Mappings()
{
	for(int index = 0; index < (int)m_mappings.size(); index++)
	{
		delete m_mappings[index];
	}
	m_mappings.clear();
}

/** Select mapping according to its mapping id */
const Mapping * Mappings::select(const String & id) const
{
	const Mapping * result = 0;
	if (m_mappings.size() > 0)
	{
		if (id == "")
		{
			result = m_mappings[0];
		}
		else
		{
			for(int index = 0; index < (int)m_mappings.size(); index++)
			{
				if (m_mappings[index]->name() == id)
				{
					result = m_mappings[index];
					break;
				}
			}
		}
	}
	return result;
}

/** Serialize the content of widget into json */
void Mappings::serialize(JsonIterator & it)
{
}

/** Unserialize the content of widget from json */
void Mappings::unserialize(JsonIterator & parent)
{
	JsonIterator it = parent["mappings"];
	for(it.first(); it.exist(); it.next())
	{
		Mapping * mapping = new Mapping;
		JsonIterator child = it.child();
		mapping->unserialize(child);
		m_mappings.push_back(mapping);
	}
}

/** Create new mappings */
Style * Mappings::create()
{
	return new Mappings;
}

/** Indicates if the window must be refreshed */
bool Mappings::is_dirty()
{
	return UIManager::invalidator()->is_dirty(this);
}
