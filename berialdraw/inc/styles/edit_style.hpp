#ifndef EditStyle_hpp_INCLUDED
#define EditStyle_hpp_INCLUDED

namespace berialdraw
{
	/** The EditStyle class defines the appearance of text input widgets, including max lines and columns,
	place holder text and color. */
	class EditStyle : public Style
	{
	public:
		/** Constructor */
		EditStyle();

		/** Destructor */
		~EditStyle();

		/** Serialize the content of widget into json */
		void serialize(JsonIterator & it);

		/** Unserialize the content of widget from json */
		void unserialize(JsonIterator & it);

		/** Set properties with another */
		void set(const EditStyle & other);


		/** Get the max visible lines in the edit */
		uint16_t max_lines() const;

		/** Get the max visible columns in the edit */
		uint16_t max_columns() const;

		/** Set the max visible lines in the edit */
		void max_lines(uint16_t v);

		/** Set the max visible columns in the edit */
		void max_columns(uint16_t v);


		/** Get the selection color */
		uint32_t select_color() const;

		/** Set the selection color */
		void select_color(uint32_t col);

		/** Set the selection color with alpha */
		void select_color(uint32_t col, uint8_t alpha);


		/** Get the cursor color */
		uint32_t cursor_color() const;

		/** Set the cursor color */
		void cursor_color(uint32_t col);

		/** Set the cursor color with alpha */
		void cursor_color(uint32_t col, uint8_t alpha);


		/** Hides characters for password entry */
		void password(bool state);

		/** Indicates if characters is hidden for password entry */
		bool password();


		/** Get the place holder value */
		const String & place_holder();

		/** Set place holder value with printf format */
		void place_holder(const char * t, ...);

		/** Set place_holder value with string */
		void place_holder(const String & s);


		/** Get the place holder color */
		uint32_t place_holder_color() const;

		/** Set the place holder color */
		void place_holder_color(uint32_t col);

		/** Set the place holder color with alpha */
		void place_holder_color(uint32_t col, uint8_t alpha);


		/** Create new border style */
		static Style * create();
	protected:

/// @cond DOXYGEN_IGNORE
		uint16_t m_max_lines = 1;
		uint16_t m_max_columns = 60;
		uint32_t m_select_color   = 0x3F000000;
		uint32_t m_cursor_color   = 0xFF000000;
		uint32_t m_place_holder_color   = 0xFFC0C4CC;
		bool     m_password = false;
		String * m_place_holder = 0;
/// @endcond
	};
}
#endif
