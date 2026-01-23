#pragma once
namespace berialdraw
{

	/** Reserved key for the keyboard */
	enum class ReservedKey
	{
		KEY_LETTER      = 0x0001,
		KEY_NUMERIC     = 0x0002,

		KEY_SHIFT       = 0x2B06,
		KEY_ENTER       = 0x000A,
		KEY_ESCAPE      = 0x001B,
		KEY_BACKSPACE   = 0x0008,
		KEY_TABULATION  = 0x0009,
		KEY_DELETE      = 0x007f,

		KEY_LEFT        = 0x2190,
		KEY_UP          = 0x2191,
		KEY_RIGHT       = 0x2192,
		KEY_DOWN        = 0x2193,

		KEY_HOME        = 0x21E4,
		KEY_END         = 0x21E5,
		KEY_PAUSE       = 0x23F8,
		KEY_EXIT        = 0x23F9,
	};


	/** Class defines the behavior of key */
	class Key : public CommonStyle, public WidgetStyle, public TextStyle, public BorderStyle
	{
	public:
		/** Constructor */
		Key();

		/** Destructor */
		~Key();

		/** Serialize the content of widget into json */
		void serialize(JsonIterator & it);

		/** Unserialize the content of widget from json */
		void unserialize(JsonIterator & it);

		/** Gets the key value */
		wchar_t key() const;

		/** Gets the name of mapping */
		const String & name() const;

		/** Gets the name of the destination mapping */
		const String & to() const;

	protected:
/// @cond DOXYGEN_IGNORE
		wchar_t    m_key = 0;
		String     m_name;
		String     m_to;
/// @endcond
	};
	
	/** Class defines the behavior of specials keys used in widget keyboard */
	class Keys : public Style
	{
	public:
		/** Constructor */
		Keys();

		/** Destructor */
		~Keys();

		/** Gets key informations according to mapping name */
		const Key * key(wchar_t key_value, const String & name) const;

		/** Serialize the content of widget into json */
		void serialize(JsonIterator & it);

		/** Unserialize the content of widget from json */
		void unserialize(JsonIterator & it);

		/** Create new keys */
		static Style * create();

	protected:
/// @cond DOXYGEN_IGNORE
		Vector<Key *> m_keys;
/// @endcond
	};
}
