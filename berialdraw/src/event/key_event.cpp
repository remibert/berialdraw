#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
KeyEvent::KeyEvent(wchar_t key, enum KeyState state, enum Modifier modifier):
	Event(KeyEvent::type_id()),
	m_key(key),
	m_state(state),
	m_modifier(modifier)
{
}

/** Destructor */
KeyEvent::~KeyEvent()
{
}

/** Return the key state */
enum KeyEvent::KeyState KeyEvent::state() const
{
	return m_state;
}

/** Get the key pressed  */
wchar_t KeyEvent::key() const
{
	return m_key;
}

/** Get type */
uint32_t KeyEvent::type_id()
{
	static uint32_t result = 0;
	if (result == 0)
	{
		result = (uint32_t)typeid(KeyEvent).hash_code();
	}
	return result;
}

/** Get modifier value */
enum KeyEvent::Modifier KeyEvent::modifier() const
{
	return m_modifier;
}

/** To string */
void KeyEvent::to_string(String & str) const
{
	str.print("{'type':'key','key':%3d,'state':'%s,'modifier':'%s,'character':'%c'},",
		m_key, 
		(m_state == KEY_UP ? "up'  " : 
						     "down'"),

		(m_modifier == MODIFIER_NONE    ? "'     " : 
		(m_modifier == MODIFIER_SHIFT   ? "shift'" :
		(m_modifier == MODIFIER_CONTROL ? "ctrl' " : 
										  "alt'  "))),

		(m_key < 0x20  ? m_key + '@' :
		(m_key < 0x7F ? m_key : ' ')));
}
