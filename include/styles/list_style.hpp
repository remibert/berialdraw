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


	protected:
/// @cond DOXYGEN_IGNORE
		uint32_t m_separator_color = Color::LIST_SEPARATOR_COLOR;
		Dim      m_separator_thickness = 1 << 6;  ///< Default 1 pixel
/// @endcond
	};
}
