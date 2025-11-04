#pragma once
namespace berialdraw
{
	class Widget;
	/** Class defines the behavior of path */
	class Path
	{
	public:
		/** Constructor */
		Path(void * parent);

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
#ifndef SWIG
/// @cond DOXYGEN_IGNORE
		String     m_path;
		void * m_parent;
		uint32_t   m_color = 0xFF000000;
/// @endcond
#endif
	};

	/**  */
	class Sketch : public Shape
	{
	public:
		/** Constructor for sketch
		@param canvas Pointer to the canvas on which the sketch is drawn */
		Sketch(Canvas * canvas);

		/** Constructor for sketch
		@param widget Pointer to the widget on which the sketch is drawn */
		Sketch(Widget * widget);

		/** Copy constructor for sketch
		@param other Other sketch object to copy from */
		Sketch(const Sketch & other);

		/** Destructor */
		virtual ~Sketch();

		/** Paint the sketch on the desktop
		@param shift Point object representing the shift in position */
		virtual void paint(const Point & shift);

		/** Get the polygon representing the sketch
		@return Reference to the polygon of the sketch */
		virtual const Polygon & polygon() const
		{
			return m_polygon;
		}

		/** Serialize the content of widget into json */
		void serialize(JsonIterator & it);

		/** Unserialize the content of widget from json */
		void unserialize(JsonIterator & it);

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
		Dim zoom() const;

		/** Set the zoom ratio for the polygon
		@param z zoom value shifted by 6 bits */
		void zoom_(Dim z);
		
		/** Get the zoom ratio for the polygon
		@return zoom zoom value shifted by 6 bits */
		Dim zoom_() const;


		/** Get the filename value */
		const String & filename() const;

		/** Set filename value with string */
		void filename(const String & s);


		/** Load the icon from filename */
		bool load();

		// Render polygon
		void paint(const Area & foreclip, const Margin & padding, uint32_t stated_color);

	#ifdef _DEBUG
		static void test();
		static void test1();
		static void test2();
		static void test3();
		static void test4();
		static void test5();
		static void test6();
	#endif
	protected:
#ifndef SWIG
/// @cond DOXYGEN_IGNORE
		Polygon m_polygon; /**< Polygon representing the sketch*/
		Vector<Path*> m_paths;
		Size m_resolution;
		Dim  m_zoom = 1<<6;
		String m_filename;
		bool m_loaded = false;
		void * m_parent = 0;
/// @endcond
#endif
	};
}
