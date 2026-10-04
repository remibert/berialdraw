#pragma once
namespace berialdraw
{
	class Polygon;
	class Json;

	/** Class defines the behavior of path */
	class Path
	{
	public:
		/** Constructor
		@param parent Object notified on change (may be null) */
		Path(void * parent = 0);

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
		void * m_parent;
		uint32_t   m_color = 0xFF000000;
/// @endcond
	};

	/** Vector image content (paths, resolution, file loading and painting).
	Independent of Shape/CommonStyle: placement, zoom and color are given by the caller. */
	class VectorImage
	{
	public:
		/** Constructor
		@param parent Object invalidated on change (may be null) */
		VectorImage(void * parent = 0);

		/** Copy constructor */
		VectorImage(const VectorImage & other);

		/** Destructor */
		~VectorImage();

		/** Serialize filename and resolution into json */
		void serialize(JsonIterator & it);

		/** Unserialize filename, resolution and paths from json */
		void unserialize(JsonIterator & it);

		/** Add bezier path */
		void add_path(uint32_t color, const String & path);

		/** Clear all bezier path */
		void clear_paths();

		/** Number of paths */
		uint32_t path_count() const;

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

		/** Set filename value (drops loaded paths) */
		void filename(const String & s);

		/** Same as load(), keeps the parsed json so the caller can read extra fields */
		bool load();

		/** True once a load has been attempted for the current filename */
		bool loaded() const;

		/** Zoom (Q6) fitting the resolution into size; returns zoom_q6 unchanged if size is undefined
		@param size Target size*/
		Dim fit_zoom_q6(const Size & size) const;

		/** Size of the image once zoomed
		@param zoom_q6 Zoom value shifted by 6 bits */
		Size content_size(Dim zoom_q6) const;

		/** Paint all paths
		@param position Top-left of the zoomed image
		@param shift Shift applied to the outline
		@param center Rotation center
		@param angle Rotation angle (Q6)
		@param zoom_q6 Zoom value shifted by 6 bits
		@param stated_color Color override (Color::TRANSPARENT keeps the file colors)
		@param scratch Optional polygon reused as work area */
		void paint(const Point & position, const Point & shift, const Point & center, Coord angle, Dim zoom_q6, uint32_t stated_color, Polygon * scratch = 0);

	protected:
/// @cond DOXYGEN_IGNORE
		Vector<Path*> m_paths;
		Size   m_resolution;
		String m_filename;
		bool   m_loaded = false;
		void * m_parent = 0;
/// @endcond
	};
}
