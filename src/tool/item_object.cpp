#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Create json object */
ItemObject::ItemObject()
{
}

/** Destroy item json object */
ItemObject::~ItemObject()
{
	clear();
}

/** Serialize to json */
void ItemObject::serialize(TextStream & out, int32_t indent)
{
	uint32_t count = m_keys.size();
	int index;
	
	out.pad('\t', indent);
	out.write_string("{");
	if (indent >= 0)
	{
		out.write_string("\n");
		indent++;
	}
	
	for (index = 0; index < (int)count; index++)
	{
		Item * value = m_values[index];
		ItemString * key = m_keys[index];

		if (value && key)
		{
			if (indent >= 0)
			{
				out.pad('\t', indent);
			}

			// Serialize key
			key->serialize(out, index);

			// If indentation
			if (indent >= 0)
			{
				// Space value
				out.write_string(" : ");
				if (dynamic_cast<ItemArray*>(value)||dynamic_cast<ItemObject*>(value))
				{
					out.write_string("\n");
				}
			}
			else
			{
				out.write_string(":");
			}
	
			// Serialize value
			value->serialize(out, indent);

			// If the last value not reached
			if (index < (int)count-1)
			{
				out.write_string(",");
			}

			// If indentation
			if (indent >= 0)
			{
				out.write_string("\n");
			}
		}
	}

	if(indent >= 0)
	{
		indent--;
		out.pad('\t', indent);
	}
	out.write_string("}");
}

/** Unserialize from json */
bool ItemObject::unserialize(TextStream & content)
{
	wchar_t character = content.read_char();
	bool result = true;

	if(character == '{')
	{
		Item * key=0;
		Item * value=0;
		bool next = true;
		do
		{
			character = Json::skip_blank(content);

			// Case of empty object
			if(character == '}')
			{
				character = content.read_char();
				break;
			}
			else
			{
				// Get the key
				key = Json::choose_type(content);

				// If the key failed to get
				if(key == 0 || dynamic_cast<ItemString*>(key) == 0)
				{
					result = false;
					break;
				}
				else
				{

					character = Json::skip_blank(content);
					if (character == ':')
					{
						character = content.read_char();
						value = Json::choose_type(content);

						// If the value failed to get
						if(value == 0)
						{
							result = false;
							break;
						}
						else
						{
							String key_ = *dynamic_cast<ItemString*>(key);
							character = Json::skip_blank(content);
							set(key_, value);
							delete key;
							key = 0;
							value = 0;
						}
					}
				}
			}
			
			if (character == ',')
			{
				character = content.read_char();
			}
			else if (character == '}')
			{
				character = content.read_char();
				break;
			}
			else
			{
				character = content.read_char();
				result = false;
				break;
			}
		}
		while(character);
		if(character != '}')
		{
			result = false;
		}
		delete key;
		delete value;
	}
	return result;
}

/** Set item into json object */
void ItemObject::set(const char * key, Item * value)
{
	if (key && value)
	{
		for (int index = 0; index < (int)m_keys.size(); index++)
		{
			if (*m_keys[index] == key)
			{
				delete m_values[index];
				m_values[index] = value;
				value = 0;
				break;
			}
		}
		if (value)
		{
			m_keys.push_back(new ItemString(key));
			m_values.push_back(value);
		}
	}
}

/** Set item into json object */
void ItemObject::set(int32_t index, Item * value)
{
	if (m_values.inbound(index))
	{
		delete m_values[index];
		m_values[index] = value;
	}
}

/** Remove selected item */
bool ItemObject::remove(int32_t index)
{
	bool result = false;
	if (m_values.inbound(index))
	{
		delete m_values[index];
		m_values.remove(index);
		delete m_keys[index];
		m_keys.remove(index);
		result = true;
	}
	return result;
}

/** Remove selected item */
bool ItemObject::remove(const char * key)
{
	bool result = false;
	for (int32_t index = 0; index < (int)m_keys.size(); index++)
	{
		if (*m_keys[index] == key)
		{
			result = remove(index);
			break;
		}
	}
	return result;
}

/** Provides read access to element at the given index in the vector. */
Item * ItemObject::operator[](int32_t index)
{
	Item * result = 0;
	if (m_values.inbound(index))
	{
		result = m_values[index];
	}
	return result;
}

/** Provides read access to element at the given index in the vector. */
const Item * ItemObject::operator[](int32_t index) const
{
	Item * result = 0;
	if (m_values.inbound(index))
	{
		result = m_values[index];
	}
	return result;
}

/** Provides read access to element at the given index in the vector. */
Item * ItemObject::operator[](const char * key)
{
	Item * result = 0;
	bool accurate;
	for (int index = 0; index < (int)m_keys.size(); index++)
	{
		if (m_keys[index]->compare(key, '_', accurate))
		{
			result = m_values[index];
			break;
		}
	}
	return result;
}

/** Provides read access to element at the given index in the vector. */
const Item * ItemObject::operator[](const char * key) const
{
	Item * result = 0;
	for (int index = 0; index < (int)m_keys.size(); index++)
	{
		if (*m_keys[index] == key)
		{
			result = m_values[index];
			break;
		}
	}
	return result;
}

/** Clear all items in object */
void ItemObject::clear()
{
	for(int index = 0; index < (int)m_keys.size(); index++)
	{
		delete m_keys[index];
	}
	m_keys.clear();
	for(int index = 0; index < (int)m_values.size(); index++)
	{
		delete m_values[index];
	}
	m_values.clear();
}

/** Item count in object */
int32_t ItemObject::count() const
{
	return (int32_t)m_keys.size();
}

/** Get the child item at index */
Item * ItemObject::child(int32_t index)
{
	if(m_values.inbound(index))
	{
		return m_values[index];
	}
	return 0;
}

/** Get key at index */
ItemString * ItemObject::key(int32_t index) const
{
	if (m_keys.inbound(index))
	{
		return m_keys[index];
	}
	return 0;
}
