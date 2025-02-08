#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Init with not root of json */
JsonIterator::JsonIterator(Json & json, JsonIterator * parent, Item * item, int32_t index):
	m_json(json),
	m_index(index),
	m_item(item),
	m_parent(parent),
	m_key(""),
	m_with_index(true),
	m_destroyed(false)
{
	m_accurate = false;
}

/** Init with not root of json */
JsonIterator::JsonIterator(Json & json, JsonIterator * parent, Item * item, const char * key) :
	m_json(json),
	m_index(),
	m_item(item),
	m_parent(parent),
	m_key(key),
	m_with_index(false),
	m_destroyed(false)
{
	m_accurate = is_accurate(key);
}

JsonIterator::JsonIterator(Json & json):
	m_json(json),
	m_destroyed(false),
	m_accurate(false)
{
	m_item = m_json.m_json;
}

bool JsonIterator::has_child()
{
	Item * selected = 0;
	ItemCollection * collection = dynamic_cast<ItemCollection*>(m_item);
	if(collection)
	{
		selected = (*collection)[m_index];
	}
	if (dynamic_cast<ItemObject*>(selected) || dynamic_cast<ItemArray*>(selected))
	{
		return true;
	}
	return false;
}

/** Get the key */
const String & JsonIterator::key() const
{
	ItemObject * obj = dynamic_cast<ItemObject *>(m_item);
	if(obj)
	{
		ItemString * result = obj->key(m_index);
		if(result)
		{
			return result->get();
		}
	}
	return String::empty;
}

JsonIterator JsonIterator::child()
{
	Item * selected = 0;
	ItemCollection * collection = dynamic_cast<ItemCollection*>(m_item);
	if(collection)
	{
		selected = (*collection)[m_index];
	}
	if (dynamic_cast<ItemObject*>(selected) || dynamic_cast<ItemArray*>(selected))
	{
		JsonIterator new_iterator(m_json, this, selected, 0);
		return new_iterator;
	}
	JsonIterator result(m_json, this, 0, 0);
	return result;
}

bool JsonIterator::next()
{
	bool result = false;
	ItemCollection * collection = dynamic_cast<ItemCollection*>(m_item);
	if(collection)
	{
		if(m_index != -1)
		{
			m_index ++;
			if ((*collection)[m_index])
			{
				result = true;
			}
		}
	}
	return result;
}

bool JsonIterator::previous()
{
	bool result = false;
	ItemCollection * collection = dynamic_cast<ItemCollection*>(m_item);
	if(collection)
	{
		if(m_index != 0)
		{
			m_index --;
			if ((*collection)[m_index-1])
			{
				result = true;
			}
		}
	}
	return result;
}

bool JsonIterator::last()
{
	m_index = -1;
	return m_item != 0;
}

bool JsonIterator::first()
{
	m_index = 0;
	return m_item != 0;
}

bool JsonIterator::exist()
{
	bool result = false;
	ItemCollection * collection = dynamic_cast<ItemCollection*>(m_item);
	if(collection)
	{
		if((*collection)[m_index])
		{
			result = true;
		}
	}
	else if (m_item)
	{
		result = true;
	}
	return result;
}

/** Remove current item */
bool JsonIterator::remove()
{
	bool result = false;
	if(m_parent == 0)
	{
		m_item = m_json.m_json;
		delete m_item;
		m_item = 0;
		m_index = 0;
		m_with_index = true;
		m_key = "";
		m_json.m_json = 0;
		result = true;
	}
	else
	{
		if (m_with_index)
		{
			ItemCollection * collection = dynamic_cast<ItemCollection*>(m_parent->m_item);
			if(collection)
			{
				result = (*collection).remove(m_index);
				m_item = 0;
				if (m_index > 0)
				{
					m_index --;
				}
				else if (m_index < 0)
				{
					m_index ++;
				}
				m_key = "";				
			}
		}
		else
		{
			ItemObject * obj = dynamic_cast<ItemObject*>(m_parent->m_item);
			if(obj)
			{
				result = (*obj).remove(m_key);
				m_item = 0;
				m_index = 0;
				m_key = "";
			}
		}
	}
	return result;
}

/** Destructor */
JsonIterator::~JsonIterator()
{
	m_destroyed = true;
}

JsonIterator & JsonIterator::operator=(const JsonIterator & other)
{
	if (this != &other)
	{
		m_json      = other.m_json;
		m_index     = other.m_index;
		m_item      = other.m_item; 
		m_item_tmp   = other.m_item_tmp; 
		m_parent    = other.m_parent;
		m_key       = other.m_key;
		m_with_index = other.m_with_index;
		m_accurate  = other.m_accurate;
		m_destroyed = other.m_destroyed;
	}
	return *this;
}

JsonIterator::JsonIterator (const JsonIterator & other): 
	m_json(other.m_json)
{
	m_index     = other.m_index;
	m_item      = other.m_item; 
	m_item_tmp   = other.m_item_tmp; 
	m_parent    = other.m_parent;
	m_key       = other.m_key;
	m_with_index = other.m_with_index;
	m_accurate  = other.m_accurate;
	m_destroyed = other.m_destroyed;
}

int32_t JsonIterator::count()
{
	int32_t result = ~0;
	ItemCollection * collection = dynamic_cast<ItemCollection*>(m_item);
	if (collection)
	{
		result = collection->count();
	}
	return result;
}

Item * JsonIterator::get()
{
	Item * result = 0;
	ItemCollection * collection = dynamic_cast<ItemCollection*>(m_item);
	if (collection)
	{
		result = (*collection)[m_index];
	}
	return result;
}

const Item * JsonIterator::get() const
{
	Item * result = 0;
	ItemCollection * collection = dynamic_cast<ItemCollection*>(m_item);
	if (collection)
	{
		result = (*collection)[m_index];
	}
	return result;
}

/** Return the type of node found in path selected */
enum JsonType JsonIterator::type()
{
	return Item::type(m_item);
}

/** Get variant value in the json */
ItemVariant & JsonIterator::value()
{
	ItemVariant * item = dynamic_cast<ItemVariant*>(get());
	return *item;
}

/** Convert to string */
JsonIterator::operator const String() const
{
	ItemVariant * value = dynamic_cast<ItemVariant *>(m_item);
	if(value)
	{
		return *value;
	}
	return "";
}

/** Convert to boolean */
JsonIterator::operator bool() const
{
	ItemVariant * value = dynamic_cast<ItemVariant *>(m_item);
	if(value)
	{
		return *value;
	}
	return false;
}

long long JsonIterator::to_integer(const ItemVariant * variant) const
{
	const ItemVariant * value = dynamic_cast<const ItemVariant *>(m_item);
	if (value == 0)
	{
		const Item * item = get();
		value = dynamic_cast<const ItemVariant*>(item);
	}

	long long result = 0;
	if(value)
	{
		if (m_accurate)
		{
			if (value->is_accurate())
			{
				result = *value;
			}
			else
			{
				result = *value;
				result <<= 6;
			}
		}
		else
		{
			if (value->is_accurate())
			{
				result = *value;
				result >>= 6;
			}
			else
			{
				result = *value;
			}
		}
	}
	return result;
}

/** Convert to integer */
JsonIterator::operator int() const
{
	return (int)to_integer(dynamic_cast<ItemVariant *>(m_item));
}

/** Convert to integer */
JsonIterator::operator unsigned int() const
{
	return (unsigned int)to_integer(dynamic_cast<ItemVariant *>(m_item));
}

/** Convert to integer */
JsonIterator::operator long long() const
{
	return to_integer(dynamic_cast<ItemVariant *>(m_item));
}

bool JsonIterator::is_null() const
{
	ItemVariant * value = dynamic_cast<ItemVariant *>(m_item);
	if(value)
	{
		return value->is_null();
	}
	return false;
}

bool JsonIterator::is_accurate(const char * key)
{
	bool result = false;
	const char * current = key;

	if (key)
	{
		while(*current)
		{
			if (*current == '_')
			{
				if (current[1] == '\0')
				{
					result = true;
					break;
				}
			}
			current ++;
		}
	}
	return result;
}

/** Provides read access to element at the given index in the vector. */
JsonIterator JsonIterator::operator[](int index)
{
	if (m_destroyed)
	{
		throw JsonIteratorError();
	}
	if(m_parent == 0)
	{
		m_item = m_json.m_json;
	}
	ItemCollection * collection =dynamic_cast<ItemCollection*>(m_item);
	if(collection)
	{
		JsonIterator new_iterator(m_json, this, (*collection)[index], index);
		return new_iterator;
	}

	JsonIterator result(m_json, this, 0, index);
	return result;
}

/** Provides read access to element at the given index in the vector. */
JsonIterator JsonIterator::operator[](const char * key)
{
	if (m_destroyed)
	{
		throw JsonIteratorError();
	}
	if(m_parent == 0)
	{
		m_item = m_json.m_json;
	}
	ItemObject * object =dynamic_cast<ItemObject*>(m_item);
	if(object)
	{
		JsonIterator new_iterator(m_json, this, (*object)[key], key);
		return new_iterator;
	}

	JsonIterator result(m_json, this, 0, key);
	return result;
}

void JsonIterator::set(Item * newone)
{
	Item * node = newone;
	JsonIterator * current = this;
	do
	{
		if(current->m_parent)
		{
			ItemArray  * parent_arr = dynamic_cast<ItemArray*> (current->m_parent->m_item);
			ItemObject * parent_obj = dynamic_cast<ItemObject*>(current->m_parent->m_item);

			if (current->m_destroyed)
			{
				delete newone;
				throw JsonIteratorError();
			}

			// If the current item already existing
			if (current->m_item)
			{
				// If the parent is an array
				if (parent_arr)
				{
					parent_arr->set(current->m_index, node);
					node = 0;
				}
				// If the parent is an object
				else if (parent_obj)
				{
					parent_obj->set(current->m_key, node);
					node = 0;
				}
			}
			// If the current item not existing
			else
			{
				// If index detected for new node
				if(current->m_with_index)
				{
					// If parent existing
					if (parent_arr)
					{
						// Set the new node in parent
						parent_arr->set(current->m_index, node);
						node = 0;
					}
					else
					{
						// Create an array with the new node
						ItemArray * newarr = new ItemArray;
						current->m_item = node;
						newarr->set(0,node);
						node = newarr;
						if(current->m_parent)
						{
							current->m_parent->m_item_tmp = newarr;
						}
					}
				}
				// Else the key is detected for new node
				else
				{
					// If parent is an object
					if (parent_obj)
					{
						// Add the new node in parent
						parent_obj->set(current->m_key, node);
						node = 0;
					}
					else
					{
						// Create new object with new node
						ItemObject * newobj = new ItemObject;
						newobj->set(current->m_key, node);
						node = newobj;
						current->m_item = node;
						if(current->m_parent)
						{
							current->m_parent->m_item_tmp = newobj;
						}
					}
				}
			}
		}
		current = current->m_parent;
	}
	while (current && node != 0);

	current = this;
	do
	{
		current->m_item = current->m_item_tmp;
		current = current->m_parent;
	}
	while (current);

	// If json tree not yet initialized
	if(m_json.m_json == 0)
	{
		// Set the new tree
		m_json.m_json = dynamic_cast<ItemCollection*>(node);
	}
	else if (node)
	{
		delete node;
	}
}

JsonIterator & JsonIterator::operator=(const String & value)
{
	set(new ItemString(value));
	return *this;
}

JsonIterator & JsonIterator::operator=(const char * value)
{
	set(new ItemString(value));
	return *this;
}

JsonIterator & JsonIterator::operator=(int value)
{
	set(new ItemInt(value, m_accurate));
	return *this;
}

JsonIterator & JsonIterator::operator=(unsigned int value)
{
	set(new ItemInt(value, m_accurate));
	return *this;
}

JsonIterator & JsonIterator::operator=(long long value)
{
	set(new ItemInt(value, m_accurate));
	return *this;
}

JsonIterator & JsonIterator::operator=(bool value)
{
	set(new ItemBool(value));
	return *this;
}

// Compare if equal
bool JsonIterator::operator==(const char * other) const
{
	bool result = false;
	const ItemVariant * item = dynamic_cast<const ItemVariant*>(m_item);
	if(item)
	{
		if (*item == other)
		{
			result = true;
		}
	}
	return result;
}

bool JsonIterator::operator==(const String & other) const
{
	bool result = false;
	const ItemVariant * item = dynamic_cast<const ItemVariant*>(m_item);
	if(item)
	{
		if (*item == other)
		{
			result = true;
		}
	}
	return result;
}

bool JsonIterator::operator==(int other) const
{
	bool result = false;
	const ItemVariant * item = dynamic_cast<const ItemVariant*>(m_item);
	if(item)
	{
		if (*item == other)
		{
			result = true;
		}
	}
	return result;
}

bool JsonIterator::operator==(unsigned int other) const
{
	bool result = false;
	const ItemVariant * item = dynamic_cast<const ItemVariant*>(m_item);
	if(item)
	{
		if (*item == other)
		{
			result = true;
		}
	}
	return result;
}

bool JsonIterator::operator==(long long other) const
{
	bool result = false;
	const ItemVariant * item = dynamic_cast<const ItemVariant*>(m_item);
	if(item)
	{
		if (*item == other)
		{
			result = true;
		}
	}
	return result;
}

bool JsonIterator::operator==(bool other) const
{
	bool result = false;
	const ItemVariant * item = dynamic_cast<const ItemVariant*>(m_item);
	if(item)
	{
		if (*item == other)
		{
			result = true;
		}
	}
	return result;
}

// Compare if different
bool JsonIterator::operator!=(const char * other) const
{
	return !(*this == other);
}

bool JsonIterator::operator!=(const String & other) const
{
	return !(*this == other);
}

bool JsonIterator::operator!=(int other) const
{
	return !(*this == other);
}

bool JsonIterator::operator!=(unsigned int other) const
{
	return !(*this == other);
}

bool JsonIterator::operator!=(long long other) const
{
	return !(*this == other);
}

bool JsonIterator::operator!=(bool other) const
{
	return !(*this == other);
}

String JsonIterator::operator|(const char * other) const
{
	const ItemVariant * item = dynamic_cast<const ItemVariant*>(m_item);
	if(item)
	{
		return *item;
	}
	return other;
}

String JsonIterator::operator|(const String & other) const
{
	const ItemVariant * item = dynamic_cast<const ItemVariant*>(m_item);
	if(item)
	{
		return *item;
	}
	return other;
}

int JsonIterator::operator|(int other) const
{
	const ItemVariant * item = dynamic_cast<const ItemVariant*>(m_item);
	if(item)
	{
		// return *item;
		return (int)to_integer(item);
	}
	return other;
}

int JsonIterator::operator|(unsigned int other) const
{
	const ItemVariant * item = dynamic_cast<const ItemVariant*>(m_item);
	if(item)
	{
		// return (int)*item;
		return (unsigned int)to_integer(item);
	}
	return other;
}

long long JsonIterator::operator|(long long other) const
{
	const ItemVariant * item = dynamic_cast<const ItemVariant*>(m_item);
	if(item)
	{
		// return *item;
		return to_integer(item);
	}
	return other;
}

bool JsonIterator::operator|(bool other) const
{
	const ItemVariant * item = dynamic_cast<const ItemVariant*>(m_item);
	if(item)
	{
		return *item;
	}
	return other;
}
