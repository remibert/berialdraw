#pragma once
namespace berialdraw
{
	/** Entry in the image item's rotation cache, storing one resized+rotated version */
	struct RotatedEntry
	{
		uint32_t* pixels = nullptr;
		uint32_t  width = 0;
		uint32_t  height = 0;
		Coord     angle = 0;
		uint32_t  src_width = 0;   // Requested source size (before rotation)
		uint32_t  src_height = 0;

		~RotatedEntry()
		{
			if (pixels)
			{
				delete[] pixels;
				pixels = nullptr;
			}
		}
	};

	/** Image item that manages a source image and its rotated/resized versions.
	Stores the source image from ImageCache plus metadata (filename) and manages
	cached rotated versions. Used by Renderer::draw_image to get pre-rotated pixels
	while keeping the center/angle logic in the renderer (like for vector shapes). */
	class ImageItem
	{
	public:
		/** Constructor
		@param source The source image from ImageCache
		@param filename The original image filename for SVG export */
		ImageItem(const ImageCacheEntry* source, const String & filename);

		/** Destructor - frees all cached rotated versions */
		~ImageItem();

		/** Get rotated pixels for the given parameters.
		If not cached, resizes and rotates the source image.
		@param angle Rotation angle in Q6 (64 = 1 degree)
		@param target_width Desired width before rotation
		@param target_height Desired height before rotation
		@param fit_mode How to fit the image to target size
		@param out_width Output: actual width after rotation
		@param out_height Output: actual height after rotation
		@return Pointer to rotated pixel data, or nullptr if failed */
		const uint32_t* get_pixels(
			Coord angle,
			uint32_t target_width,
			uint32_t target_height,
			ImageFitMode fit_mode,
			uint32_t& out_width,
			uint32_t& out_height);

		/** Clear all cached rotated versions */
		void clear();

		/** Get the source image entry */
		const ImageCacheEntry* source() const { return m_source; }

		/** Get the filename for SVG export */
		const String & filename() const { return m_filename; }

		/** Check if this item has a valid source */
		bool is_valid() const { return m_source != nullptr && m_source->is_valid(); }

		/** Get source image width */
		uint32_t source_width() const { return m_source ? m_source->width() : 0; }

		/** Get source image height */
		uint32_t source_height() const { return m_source ? m_source->height() : 0; }

	protected:
	/// @cond DOXYGEN_IGNORE
		/** Find a cached entry matching the parameters
		@return Index of the entry, or size if not found */
		uint32_t find(Coord angle, uint32_t src_w, uint32_t src_h) const;

		/** Step 1: Resize pixels to fit dimensions
		@param fit_width Target width after fit mode
		@param fit_height Target height after fit mode
		@param out_allocated Set to true if memory was allocated (needs deletion)
		@return Pointer to resized pixels, or nullptr if failed */
		uint32_t* resize_to_fit_size(uint32_t fit_width, uint32_t fit_height, bool& out_allocated) const;

		/** Step 2: Apply rotation transform if needed
		@param resized_pixels Input: pixels to rotate
		@param resized_allocated Whether resized_pixels were newly allocated
		@param fit_width Width of resized pixels
		@param fit_height Height of resized pixels
		@param angle Rotation angle in Q6
		@param out_final_width Output: width after rotation
		@param out_final_height Output: height after rotation
		@return Final pixels (rotated or copied), or nullptr if failed */
		uint32_t* apply_rotation_transform(
			uint32_t* resized_pixels, bool resized_allocated,
			uint32_t fit_width, uint32_t fit_height,
			Coord angle,
			uint32_t& out_final_width, uint32_t& out_final_height);

		/** Step 3: Cache final entry and return pixels
		@param final_pixels Pixels to cache
		@param final_width Width of final pixels
		@param final_height Height of final pixels
		@param angle Rotation angle in Q6
		@param fit_width Fit width (before rotation)
		@param fit_height Fit height (before rotation)
		@return Pointer to cached pixels (same as final_pixels) */
		const uint32_t* cache_final_entry(
			uint32_t* final_pixels,
			uint32_t final_width, uint32_t final_height,
			Coord angle, uint32_t fit_width, uint32_t fit_height);

		/** Compute hash key for cache lookup */
		static uint64_t make_key(Coord angle, uint32_t w, uint32_t h);

		const ImageCacheEntry*  m_source;
		String                  m_filename;
		Vector<RotatedEntry*>   m_entries;
		static const uint32_t   MAX_ENTRIES = 8;  // Max rotated versions per source
	/// @endcond
	};
}
