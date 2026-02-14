#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
Key::Key()
{
}

/** Gets the key value */
wchar_t Key::key() const
{
	return m_key;
}

/** Gets the name of mapping */
const String & Key::name() const
{
	return m_name;
}

/** Gets the name of the destination mapping */
const String & Key::to() const
{
	return m_to;
}

/** Serialize the content of widget into json */
void Key::serialize(JsonIterator & it)
{
	it["key"]        = m_key;
	it["name"]       = m_name;
	it["to"]         = m_to;
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
	TextStyle::serialize(it);
	BorderStyle::serialize(it);
}

/** Unserialize the content of widget from json */
void Key::unserialize(JsonIterator & it)
{
	m_key        = it["key"]        | 0;
	m_name       = it["name"]       | "";
	m_to         = it["to"]         | "";
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
	TextStyle::unserialize(it);
	BorderStyle::unserialize(it);
}


/** Constructor */
Keys::Keys()
{
}

/** Destructor */
Keys::~Keys()
{
	for(int index = 0; index < (int)m_keys.size(); index++)
	{
		delete m_keys[index];
	}
	m_keys.clear();
}

/** Gets key informations according to mapping id */
const Key * Keys::key(wchar_t key_value, const String & name) const
{
	const Key * result = 0;
	const Key * other = 0;
	for(int index = 0; index < (int)m_keys.size(); index++)
	{
		if (m_keys[index]->name() == "" && m_keys[index]->key() == key_value)
		{
			other = m_keys[index];
		}
		if (m_keys[index]->name() == name && m_keys[index]->key() == key_value)
		{
			result = m_keys[index];
		}
	}

	if (result == 0 && other != 0)
	{
		result = other;
	}
	return result;
}

/** Serialize the content of widget into json */
void Keys::serialize(JsonIterator & it)
{
}

/** Unserialize the content of widget from json */
void Keys::unserialize(JsonIterator & parent)
{
	JsonIterator it = parent["keys"];

	Key default_key;
	default_key.unserialize(parent);
	for(it.first(); it.exist(); it.next())
	{
		Key * key = new Key;

		key->CommonStyle::set(default_key);
		key->WidgetStyle::set(default_key);
		key->BorderStyle::set(default_key);
		key->TextStyle::set(default_key);

		JsonIterator child = it.child();
		key->unserialize(child);
		m_keys.push_back(key);
	}
}

/** Create new keys */
Style * Keys::create()
{
	return new Keys;
}
