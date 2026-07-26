#pragma once
namespace berialdraw
{
	/** The IconStyle class customizes the appearance of Icon widgets. 
	It includes properties for the image size, icon padding and color. */
	class IconStyle : public Style
	{
	public:
		/** Constructor */
		IconStyle();

		/** Destructor */
		~IconStyle();

		/** Copy operator */
		IconStyle& operator=(const IconStyle& other);

		/** Set properties with another */
		void set(const IconStyle& other);

		/** Create new paths */
		static Style * create();

		/** Serialize the content of widget into json */
		void serialize(JsonIterator & it) override;

		/** Unserialize the content of widget from json */
		void unserialize(JsonIterator & it) override;


		/** Get the icon frame size */
		const Size & icon_frame_size() const { return m_icon_frame_size; }
		
		/** Set the icon frame size */
		void icon_frame_size(const Size & s);

		/** Set the icon frame size with width and height in pixels */
		void icon_frame_size(Dim w, Dim h=0);

		/** Set the icon frame size with a precision of 64th of a pixel */
		void icon_frame_size_q6(Dim w, Dim h=0);

		// Filename properties

		/** Get the filename value */
		const String & filename() const { return m_filename; }

		/** Set filename value with string */
		void filename(const String & s);

		// Icon padding properties

		/** Get the padding */
		const Margin & icon_padding() const { return m_icon_padding; }

		/** Set the padding */
		void icon_padding(const Margin & m);

		/** Set the padding in pixels */
		void icon_padding(Dim top, Dim left, Dim bottom, Dim right);

		/** Set the padding with a precision of 64th of a pixel */
		void icon_padding_q6(Dim top, Dim left, Dim bottom, Dim right);

		// Icon color properties

		/** Get the back icon color */
		uint32_t icon_color() const { return m_icon_color; }

		/** Set the back icon color */
		void icon_color(uint32_t col);

		/** Set the back icon color with alpha */
		void icon_color(uint32_t col, uint8_t alpha);

		/** Get the text padding in pixels */
		inline Dim text_padding() const
		{
			return m_text_padding >> 6;
		}

		/** Set the text padding in pixels */
		void text_padding(Dim pad);

		/** Get the property name for this style */
		const char* property_name() const override { return "icon"; }

	protected:
/// @cond DOXYGEN_IGNORE
		Margin m_icon_padding;
		uint32_t m_icon_color = Color::TRANSPARENT;
		String m_filename;
		bool m_icon_modified = true;
		Dim m_text_padding = 2 << 6;
		Size m_icon_size;
		Size m_icon_frame_size;
/// @endcond
	};
}
