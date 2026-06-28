#pragma once
namespace berialdraw
{
	/** The list item */
	class ListItemStyle : public Style
	{
	public:
		/** Constructor */
		ListItemStyle();

		/** Serialize the content of widget into json */
		void serialize(JsonIterator & it) override;

		/** Unserialize the content of widget from json */
		void unserialize(JsonIterator & it) override;

		/** Copy operator */
		ListItemStyle& operator=(const ListItemStyle& other);

		/** Set properties with another */
		void set(const ListItemStyle & other);

		/** Create new text style */
		static Style * create();


		/** Get the leading text */
		const String & leading();

		/** Set leading text with printf format */
		void leading(const char * t, ...);

		/** Set leading text with string */
		void leading(const String & s);

		/** Set leading text with unicode value */
		void leading(wchar_t value);


		/** Get the trailing text */
		const String& trailing();

		/** Set trailing text with printf format */
		void trailing(const char* t, ...);

		/** Set trailing text with string */
		void trailing(const String& s);

		/** Set trailing text with unicode value */
		void trailing(wchar_t value);


		/** Get the property name for this style */
		const char* property_name() const override { return "list-item"; }

	protected:
/// @cond DOXYGEN_IGNORE
		String  m_leading;
		String  m_trailing;
		Size m_leading_size;
		Size m_trailing_size;
		unsigned int m_leading_modified:1;
		unsigned int m_trailing_modified : 1;
/// @endcond 
	};
}
