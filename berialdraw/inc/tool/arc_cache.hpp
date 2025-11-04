#pragma once
namespace berialdraw
{
	/** Stores the result of an arc-line intersection computation. */
	class ArcItem
	{
	public:
		Coord radius      = ~0;  ///< Radius of the arc.
		Dim   thickness   = ~0;  ///< Thickness of the line.
		Coord vec_x       = 0;   ///< Computed X component of the vector.
		Coord vec_y       = 0;   ///< Computed Y component of the vector.
		Coord handle      = 0;   ///< Computed handle value.
		Coord intersection = 0;  ///< Computed intersection point.
		Dim   width        = 0;  ///< Computed width of add_corner

	};

	/**  Caches recent arc-line intersection computations to avoid redundant calculations. */
	class ArcCache
	{
	public:
		/** Constructs an empty arc intersection cache.	*/
		ArcCache();

		/** Destructor.	*/
		virtual ~ArcCache();

		/** Computes or retrieves cached values for an arc-line intersection.
		@param radius Radius of the arc.
		@param thickness Thickness of the line.
		@param width : Computed width of add_corner
		@param vec_x : X component of the vector.
		@param vec_y : Y component of the vector.
		@param handle : Handle value.
		@param intersection : Intersection point.	*/
		void compute(Coord radius, Dim thickness, Dim & width, Coord & vec_x, Coord & vec_y, Coord & handle, Coord & intersection);

	protected:
#ifndef SWIG
	/// @cond DOXYGEN_IGNORE
		static const int CACHE_SIZE = 16; ///< Maximum number of cached entries.
		int m_index = 0;                  ///< Current index in the circular cache.
		ArcItem m_items[CACHE_SIZE];      ///< Fixed-size array of cached items.
	/// @endcond
#endif
	};
}
