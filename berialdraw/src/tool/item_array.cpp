#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Create json array */
ItemArray::ItemArray()
{
}

/** Destroy item json array */
ItemArray::~ItemArray()
{
	clear();
}

/** Serialize to json */
void ItemArray::serialize(TextStream & out, int32_t indent)
{
	uint32_t count = m_array.size();
	int index;
	out.pad('\t', indent);
	out.write_string("[");
	if (indent >= 0)
	{
		out.write_string("\n");
		indent++;
	}
	for (index = 0; index < (int)count; index++)
	{
		Item * item = m_array[index];

		if (item)
		{
			if(dynamic_cast<ItemObject *>(item) == 0)
			{
				out.pad('\t', indent);
			}
			item->serialize(out, indent);
			if (index < (int)count-1)
			{
				out.write_string(",");
			}
			if (indent >= 0)
			{
				out.write_string("\n");
			}
		}
	}
	if(indent >= 0)
	{
		indent--;
	}
	out.pad('\t', indent);
	out.write_string("]");
}

/** Unserialize from json */
bool ItemArray::unserialize(TextStream & content)
{
	wchar_t character = content.read_char();
	bool result = true;
	if(character == '[')
	{
		Item * item;
		bool next = true;
		do
		{
			character = Json::skip_blank(content);

			// Case of empty array
			if(character == ']')
			{
				character = content.read_char();
				break;
			}
			else
			{
				// Get next item of array
				item = Json::choose_type(content);

				// If the item failed to get
				if(item == 0)
				{
					result = false;
					break;
				}
				else
				{
					append(item);
				}
				character = Json::skip_blank(content);
			}
			
			if (character == ',')
			{
				character = content.read_char();
			}
			else if (character == ']')
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
		if(character != ']')
		{
			result = false;
		}
	}
	return result;
}

/** Append item into json array */
void ItemArray::append(Item * item)
{
	if (item)
	{
		m_array.push_back(item);
	}
}

/** Provides read access to element at the given index in the vector. */
Item * ItemArray::operator[](int32_t index)
{
	Item * result = 0;
	if (m_array.inbound(index))
	{
		result = m_array[index];
	}
	return result;
}

/** Set item into json object */
void ItemArray::set(int32_t index, Item * value)
{
	if (value)
	{
		//value->set_parent(this);
		if (m_array.inbound(index))
		{
			Item * previous = m_array.operator[](index);
			m_array.operator[](index) = value;
			delete previous;
		}
		else
		{
			if (index < 0)
			{
				m_array.insert(0, value);
			}
			else
			{
				m_array.push_back(value);
			}
		}
	}
}

/** Provides read access to element at the given index in the vector. */
const Item * ItemArray::operator[](int32_t index) const
{
	Item * result = 0;
	if (m_array.inbound(index))
	{
		result = m_array[index];
	}
	return result;
}

/** Remove item at the index position */
bool ItemArray::remove(int32_t index)
{
	bool result = false;
	if (m_array.inbound(index))
	{
		Item * old = m_array[index];
		delete old;
		m_array.remove(index);
		result = true;
	}
	return result;
}

/** Replace the item at the index position by the new item */
void ItemArray::replace(int32_t index, Item * item)
{
	Item * old = 0;
	if (m_array.inbound(index))
	{
		old = m_array[index];
		delete old;
		m_array[index] = item;
	}
}

/** Insert item in the array */
void ItemArray::insert(int32_t index, Item * value)
{
	if(value)
	{
		m_array.insert(index, value);
	}
}

/** Clear all items in array */
void ItemArray::clear()
{
	for (uint32_t index = 0; index < m_array.size(); index++)
	{
		delete m_array[index];
	}
	m_array.clear();
}

/** Item count in array */
int32_t ItemArray::count() const
{
	return (int32_t)m_array.size();
}

/** Get the child item at index */
Item * ItemArray::child(int32_t index)
{
	if(index < (int)m_array.size())
	{
		return m_array[index];
	}
	return 0;
}
