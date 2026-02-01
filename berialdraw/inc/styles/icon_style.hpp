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

		/** Create new paths */
		static Style * create();

		/** Serialize the content of widget into json */
		virtual void serialize(JsonIterator & it);

		/** Unserialize the content of widget from json */
		virtual void unserialize(JsonIterator & it);

		// Zoom properties

		/** Set the zoom ratio for the polygon
		@param z zoom value */
		void zoom(Dim z);

		/** Get the zoom ratio for the polygon
		@return zoom zoom value */
		Dim zoom() const;

		/** Set the zoom ratio for the polygon
		@param z zoom value shifted by 6 bits */
		void zoom_(Dim z);

		/** Get the zoom ratio for the polygon
		@return zoom zoom value shifted by 6 bits */
		Dim zoom_() const;

		// Filename properties

		/** Get the filename value */
		const String & filename() const;

		/** Set filename value with string */
		void filename(const String & s);

		// Icon padding properties

		/** Get the padding */
		const Margin & icon_padding() const;

		/** Set the padding */
		void icon_padding(const Margin & m);

		/** Set the padding in pixels */
		void icon_padding(Dim top, Dim left, Dim bottom, Dim right);

		/** Set the padding with a precision of 64th of a pixel */
		void icon_padding_(Dim top, Dim left, Dim bottom, Dim right);

		// Icon color properties

		/** Get the back icon color */
		uint32_t icon_color() const;

		/** Set the back icon color */
		void icon_color(uint32_t col);

		/** Set the back icon color with alpha */
		void icon_color(uint32_t col, uint8_t alpha);

	protected:
/// @cond DOXYGEN_IGNORE
		Margin m_icon_padding;
		Size m_icon_size;
		uint32_t m_icon_color = Color::TRANSPARENT;
		Dim m_zoom = 1<<6;
		String m_filename;
		bool m_icon_modified = true;
/// @endcond
	};
}
