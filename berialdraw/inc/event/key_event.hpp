#pragma once
namespace berialdraw
{
	/** Key event */
	class KeyEvent : public Event
	{
	public:
		/** Key state */
		enum KeyState
		{
			KEY_UP   = 0,
			KEY_DOWN = 1,
		};

		/** Key modifier */
		enum Modifier
		{
			MODIFIER_NONE    = 0, ///< No key modifier
			MODIFIER_SHIFT   = 1, ///< Shift pressed
			MODIFIER_CONTROL = 2, ///< Control pressed
			MODIFIER_ALT     = 4, ///< Alt pressed
		};

		/** Constructor */
		KeyEvent(wchar_t key, enum KeyState state, enum Modifier modifier=MODIFIER_NONE);

		/** Destructor */
		virtual ~KeyEvent();

		/** To string */
		virtual void to_string(String & str) const;

		/** Return the widget */
		virtual Widget * widget() const {return 0;}

		/** Return the key state */
		enum KeyState state() const;

		/** Get the key pressed  */
		wchar_t key() const;

		/** Get modifier value */
		enum Modifier modifier() const;

		/** Get type */
		static uint32_t type_id();

	private:
/// @cond DOXYGEN_IGNORE
		wchar_t m_key = 0;
		enum KeyState m_state = KEY_UP;
		enum Modifier m_modifier = MODIFIER_NONE;
/// @endcond
	};
};
