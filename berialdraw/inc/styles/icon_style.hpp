#ifndef IconStyles_hpp_INCLUDED
#define IconStyles_hpp_INCLUDED

namespace berialdraw
{
	class IconStyle ;
	/** Class defines the behavior of path */
	class Path
	{
	public:
		/** Constructor */
		Path(IconStyle * parent);

		/** Destructor */
		~Path();

		/** Serialize the content of widget into json */
		void serialize(JsonIterator & it);

		/** Unserialize the content of widget from json */
		void unserialize(JsonIterator & it);


		/** Gets the path value */
		const String & path() const;

		/** Sets the path value */
		void path(const String & path_);

		/** Get the back color */
		uint32_t color() const;

		/** Set the back color */
		void color(uint32_t col);

		/** Set the back color with alpha */
		void color(uint32_t col, uint8_t alpha);

	protected:
/// @cond DOXYGEN_IGNORE
		String     m_path;
		IconStyle * m_parent;
		uint32_t   m_color = 0xFF000000;
/// @endcond
	};

	/** The IconStyle class customizes the appearance of Icon widgets. 
	It includes properties for the image size, icon padding and color. */
	class IconStyle  : public Style
	{
	public:
		/** Constructor */
		IconStyle();

		/** Destructor */
		~IconStyle();

		/** Serialize the content of widget into json */
		void serialize(JsonIterator & it);

		/** Unserialize the content of widget from json */
		void unserialize(JsonIterator & it);

		/** Create new paths */
		static Style * create();

		/** Add bezier path */
		void add_path(uint32_t color, const String & path);

		/** Clear all bezier path */
		void clear_paths();



		/** Get the resolution */
		const Size & resolution() const;
		
		/** Set the resolution */
		void resolution(const Size & s);

		/** Set the resolution with width and height in pixels */
		void resolution(Dim w, Dim h=0);

		/** Set the resolution with a precision of 64th of a pixel */
		void resolution_(Dim w, Dim h=0);


		/** Set the zoom ratio for the polygon
		@param z zoom value */
		void zoom(Dim z);
		
		/** Get the zoom ratio for the polygon
		@return zoom zoom value */
		Dim zoom();

		/** Set the zoom ratio for the polygon
		@param z zoom value shifted by 6 bits */
		void zoom_(Dim z);
		
		/** Get the zoom ratio for the polygon
		@return zoom zoom value shifted by 6 bits */
		Dim zoom_();


		/** Get the filename value */
		const String & filename();

		/** Set filename value with string */
		void filename(const String & s);



		/** Get the padding */
		const Margin & icon_padding() const;
		
		/** Set the padding */
		void icon_padding(const Margin & m);
		
		/** Set the padding in pixels */
		void icon_padding(Dim top, Dim left, Dim bottom, Dim right);
		
		/** Set the padding with a precision of 64th of a pixel */
		void icon_padding_(Dim top, Dim left, Dim bottom, Dim right);


		/** Get the back icon color */
		uint32_t icon_color() const;

		/** Set the back icon color */
		void icon_color(uint32_t col);

		/** Set the back icon color with alpha */
		void icon_color(uint32_t col, uint8_t alpha);


		/** Load the icon from filename */
		bool load();

	protected:
/// @cond DOXYGEN_IGNORE
		Vector<Path*> m_paths;
		Size m_resolution;
		Dim  m_zoom = 1<<6;
		String m_filename;
		Margin m_icon_padding;
		uint32_t m_icon_color = Color::TRANSPARENT;
/// @endcond
	};
}

#endif // IconStyles_hpp_INCLUDED
