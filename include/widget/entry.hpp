#pragma once
/// @cond DOXYGEN_IGNORE
namespace berialdraw
{
	// Entry field
	class Entry
	{
	public:
		/** Constructor */
		Entry(String * str);

		/** Copy contructor */
		Entry(const Entry & entry, String * inp);

		/** Copy assignment operator
		@param other Other entry to copy from
		@return Reference to this entry */
		Entry& operator=(const Entry& other);

		// Destructor
		virtual ~Entry();

		/** Get the text for the display 
		@param display display string */
		const String & input() const;

		/** Typing  mode enumeration */
		enum class TypingMode
		{
			INSERTION,
			REPLACEMENT,
		};

		/** Set typing mode */
		void typing_mode(enum TypingMode m);

		/** Get typing mode */
		TypingMode typing_mode() const;

		/** Get the cursor position in entry */
		uint32_t cursor_position() const;

		/** Position of the cursor in line and column */
		void line_column(uint32_t & line, uint32_t & column);

		/** Move the position of the cursor in line and column */
		void move_cursor(uint32_t line, uint32_t column, bool selected);

		/** Get selected text */
		void text_selected(String & text);

		/** Select text in entry 
		@param from the position of start the selection
		@param to the position of end the selection and also cursor position */
		void select_text(uint32_t from, uint32_t to);

		/** Select all text in entry */
		void select_all();

		/** No text selected */
		void select_none();

		/** Get start of selection 
		@return the wide character index */
		uint32_t selection_start() const;

		/** Get end of selection 
		@return the wide character index */
		uint32_t selection_end() const;

		/** Insert or replace a char in the entry at the cursor position 
		@param character character to add in the input */
		void append_key(wchar_t character);

		/** Trait the event on key down */
		void on_key_down(wchar_t key, KeyEvent::Modifier modifier);

		/** Set input mask
		@param mask :
		% followed with : 
			- d:decimal (0-9)
			- a:alphanumeric (A-Z,a-z,0-9)
			- A:alphanumeric (A-Z,0-9)
			- s:string (all characters)
			- l:letter (a-z)
			- L:letter (A-Z)
			- X:hexadecimal (A-F,0-9)
			- x:hexadecimal (a-f,0-9)
		 */
		void mask(const String & mask_);

		/** Get input mask */
		const String mask() const;
#ifdef _DEBUG
		// Unitary test
		static void test1();

		// Unitary test
		static void test();
#endif
	protected:
/// @cond DOXYGEN_IGNORE
		/** Set the entry alignement 
		@param alignement */
		void entry_align(Align align);

		/** Erase text selected */
		void erase_text_selected();

		/** Return the count of wide character detected in selected line */
		uint32_t line_length(uint32_t line);

		/** Move the cursor from the line to another line with the same column */
		void move_arrow(uint32_t from_line, uint32_t to_line, uint32_t column, bool selected);

		/** Move the cursor to the up character  */
		void up_arrow_key(bool selected=false);
		
		/** Move the cursor to the down character */
		void down_arrow_key(bool selected=false);

		/** Move the cursor to the next character  */
		void right_arrow_key(bool selected=false);
		
		/** Move the cursor to the previous character */
		void left_arrow_key(bool selected=false);

		/** Backspace a character at the cursor position */
		void backspace_key();

		/** Delete a character at the cursor position */
		void delete_key();

		/** Move the cursor to the first character */
		void home_key(bool selected=false);

		/** Move the cursor to the last character */
		void end_key(bool selected=false);

		/** Clear the content of the entry */
		void clear_input();

		// Input string
		String * m_input = 0;

		// cursor position
		uint32_t m_cursor_position=0;

		// Selection start
		uint32_t m_selection_start=0;

		// Selection end
		uint32_t m_selection_end=0;

		// Input typing_mode
		unsigned int m_typing_mode:2;

		// Text alignement
		unsigned int m_entry_align:4;

		// String mask
		String * m_mask = 0;
/// @endcond
	};
}
/// @endcond
