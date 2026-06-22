#pragma once

namespace berialdraw
{
	/** The ListStyle class specifies the appearance of list widgets. */
	class ListStyle : public Style
	{
	public:
		/** Constructor */
		ListStyle();

		/** Serialize the content of style into json */
		void serialize(JsonIterator & it) override;

		/** Unserialize the content of style from json */
		void unserialize(JsonIterator & it) override;

		/** Copy operator */
		ListStyle& operator=(const ListStyle& other);

		/** Copy properties from another without invalidation */
		void copy_from(const ListStyle & other);

		/** Set properties with another */
		void set(const ListStyle & other);

		/** Create new list style */
		static Style * create();

		/** Get the property name for this style */
		const char* property_name() const override { return "list"; }


		/** Get the separator color */
		uint32_t separator_color() const;

		/** Set the separator color */
		void separator_color(uint32_t c);


		/** Get the separator thickness in pixels */
		Dim separator_thickness() const { return m_separator_thickness >> 6; }

		/** Get the separator thickness in 64ths of pixels */
		Dim separator_thickness_() const { return m_separator_thickness; }

		/** Set the separator thickness in pixels */
		void separator_thickness(Dim t);

		/** Set the separator thickness in 64ths of pixels */
		void separator_thickness_(Dim t);


		/** Get whether to show arrow indicator (for menus) */
		bool show_arrow() const { return m_show_arrow; }

		/** Set whether to show arrow indicator */
		void show_arrow(bool v);


		/** Get the arrow direction (true = right ">", false = left "<") */
		bool arrow_right() const { return m_arrow_right; }

		/** Set the arrow direction */
		void arrow_right(bool v);


		/** Get the disabled text color */
		uint32_t disabled_text_color() const;

		/** Set the disabled text color */
		void disabled_text_color(uint32_t c);


	protected:
/// @cond DOXYGEN_IGNORE
		uint32_t m_separator_color = Color::LIST_SEPARATOR_COLOR;
		uint32_t m_disabled_text_color = Color::LIST_DISABLED_TEXT_COLOR;
		Dim      m_separator_thickness = 1 << 6;  ///< Default 1 pixel
		bool     m_show_arrow = false;
		bool     m_arrow_right = true;  ///< Default: right arrow ">"
/// @endcond
	};
}
