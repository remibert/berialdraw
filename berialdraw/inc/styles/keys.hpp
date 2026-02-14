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
		KEY_SPACE       = 0x0020,

		KEY_CTRL_A      = 0x0001,	// A key
		KEY_CTRL_B      = 0x0002,	// B key
		KEY_CTRL_C      = 0x0003,	// C key
		KEY_CTRL_D      = 0x0004,	// D key
		KEY_CTRL_E      = 0x0005,	// E key
		KEY_CTRL_F      = 0x0006,	// F key
		KEY_CTRL_G      = 0x0007,	// G key
		KEY_CTRL_H      = 0x0008,	// H key
		KEY_CTRL_I      = 0x0009,	// I key
		KEY_CTRL_J      = 0x000A,	// J key
		KEY_CTRL_K      = 0x000B,	// K key
		KEY_CTRL_L      = 0x000C,	// L key
		KEY_CTRL_M      = 0x000D,	// M key
		KEY_CTRL_N      = 0x000E,	// N key
		KEY_CTRL_O      = 0x000F,	// O key
		KEY_CTRL_P      = 0x0010,	// P key
		KEY_CTRL_Q      = 0x0011,	// Q key
		KEY_CTRL_R      = 0x0012,	// R key
		KEY_CTRL_S      = 0x0013,	// S key
		KEY_CTRL_T      = 0x0014,	// T key
		KEY_CTRL_U      = 0x0015,	// U key
		KEY_CTRL_V      = 0x0016,	// V key
		KEY_CTRL_W      = 0x0017,	// W key
		KEY_CTRL_X      = 0x0018,	// X key
		KEY_CTRL_Y      = 0x0019,	// Y key
		KEY_CTRL_Z      = 0x001A,	// Z key

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
