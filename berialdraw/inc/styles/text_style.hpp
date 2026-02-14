#pragma once
namespace berialdraw
{
	/** The TextStyle class defines the font, size, color, and alignment of text displayed within widgets. 
	It ensures consistent text rendering across the UI. */
	class TextStyle : public Style
	{
	public:
		/** Constructor */
		TextStyle();

		/** Serialize the content of widget into json */
		void serialize(JsonIterator & it);

		/** Unserialize the content of widget from json */
		void unserialize(JsonIterator & it);

		/** Copy operator */
		TextStyle& operator=(const TextStyle& other);

		/** Set properties with another */
		void set(const TextStyle & other);

		/** Create new text style */
		static Style * create();


		/** Get the text color */
		uint32_t text_color() const { return m_text_color; }

		/** Set the text color */
		void text_color(uint32_t col);

		/** Set the text color with alpha */
		void text_color(uint32_t col, uint8_t alpha);


		/** Get the font familly selected */
		const String & font_familly();

		/** Set the font familly */
		void font_familly(const char * f);

		/** Get the font size */
		const Size & font_size() const { return m_font_size; }
		
		/** Set the font size */
		void font_size(const Size & s);

		/** Set the font size with width and height */
		void font_size(Dim w, Dim h=0);

		/** Set the font size with a precision of 64th of a pixel */
		void font_size_(Dim w, Dim h=0);

		/** Select font */
		void select_font();


		/** Get the text value */
		const String & text();

		/** Set text value with printf format */
		void text(const char * t, ...);

		/** Set text value with string */
		void text(const String & s);

		/** Set text value with unicode value */
		void text(wchar_t value);



		/** Get the padding */
		const Margin & padding() const { return m_padding; }
		
		/** Set the padding */
		void padding(const Margin & m);
		
		/** Set the padding in pixels */
		void padding(Dim top, Dim left, Dim bottom, Dim right);
		
		/** Set the padding with a precision of 64th of a pixel */
		void padding_(Dim top, Dim left, Dim bottom, Dim right);

		/** Set the padding */
		void padding(Dim horizontal, Dim vertical);

		/** Set the padding */
		void padding(Dim value);




		/** Get the text align */
		Align text_align() const { return (Align)m_text_align; }

		/** Set the text align */
		void text_align(Align v);

	protected:
/// @cond DOXYGEN_IGNORE
		uint32_t m_text_color   = 0xFF000000;
		String  m_font_familly;
		Size    m_font_size = {0,10};
		FontPtr m_font;
		String  m_text;
		Size    m_text_size = {0,0};

		Margin m_padding = {0,0,0,0};
		unsigned int m_font_modified:1;
		unsigned int m_text_modified:1;
		unsigned int m_text_align:4;
/// @endcond 
	};
}
