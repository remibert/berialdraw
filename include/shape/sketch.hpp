#pragma once
namespace berialdraw
{
	class Widget;

	/** Vector image shape: layout and style come from Shape, content from VectorImage */
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
		virtual void paint(const Point & shift) override;

		/** Get the polygon representing the sketch
		@return Reference to the polygon of the sketch */
		virtual const Polygon & polygon() const override
		{
			return m_polygon;
		}

		/** Serialize the content of widget into json */
		void serialize(JsonIterator & it) override;

		/** Unserialize the content of widget from json */
		void unserialize(JsonIterator & it) override;

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
		void resolution_q6(Dim w, Dim h=0);


		/** Get the filename value */
		const String & filename() const;

		/** Set filename value with string */
		void filename(const String & s);


		/** Load the icon from filename */
		bool load();

		// Render polygon
		void paint(const Area & foreclip, uint32_t stated_color);

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
/// @cond DOXYGEN_IGNORE
		Polygon m_polygon; /**< Polygon representing the sketch*/
		VectorImage m_image;
		void * m_parent = 0;
/// @endcond
	};
}
