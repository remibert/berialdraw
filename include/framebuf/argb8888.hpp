#pragma once
namespace berialdraw
{
	/** Framebuffer in ARGB 8888 format */
	class Argb8888 : public Framebuf
	{
	public:
		/** Constructor
		@param width The width of the framebuffer
		@param height The height of the framebuffer */
		Argb8888(uint32_t width, uint32_t height);

		/** Destructor */
		virtual ~Argb8888();

		/** Set the pixel color at (x, y)
		@param x The x-coordinate
		@param y The y-coordinate
		@param color The color to set */
		virtual void pixel(int32_t x, int32_t y, uint32_t color);

		/** Get the pixel color at (x, y)
		@param x The x-coordinate
		@param y The y-coordinate
		@return The color at the specified coordinates */
		virtual uint32_t pixel(int32_t x, int32_t y);

		/** Fill a rectangle with color
		@param x The x-coordinate of the rectangle
		@param y The y-coordinate of the rectangle
		@param width The width of the rectangle
		@param height The height of the rectangle
		@param color The color to fill */
		virtual void paint_rect(int32_t x, int32_t y, uint32_t width, uint32_t height, uint32_t color);

		/** Get the buffer as native-endian uint32_t pixels in 0xAARRGGBB format
		@return Pointer to the packed pixel buffer */
		virtual const uint8_t* buffer();

		/** Get the width of the framebuffer
		@return The width of the framebuffer */
		virtual uint32_t width();

		/** Get the height of the framebuffer
		@return The height of the framebuffer */
		virtual uint32_t height();

		/** Check if the framebuffer is dirty
		@param val Optional value to set the dirty flag
		@return True if the framebuffer is dirty, false otherwise */
		virtual bool dirty(int32_t val = -1);

		/** Clear the framebuffer with a color
		@param color The color to clear the framebuffer with */
		virtual void clear(uint32_t color);

		/** Compute crc32 on the content of framebuffer */
		virtual uint32_t crc32();

		/** Copy a clipped pixel rectangle from a framebuffer, preserving ARGB values
		@param source Source framebuffer
		@param x Left edge in source pixels
		@param y Top edge in source pixels
		@param width Requested rectangle width
		@param height Requested rectangle height
		@return Cropped ARGB framebuffer, or null when the clipped rectangle is empty */
		static std::unique_ptr<Argb8888> crop(Framebuf& source, int32_t x, int32_t y, uint32_t width, uint32_t height);
 
	protected:
/// @cond DOXYGEN_IGNORE
		void to_bmp(const char * filename);
		uint32_t* m_buffer;  /**< Native-endian packed 0xAARRGGBB pixels with straight alpha */
		uint32_t m_width;    /**< Width of the framebuffer */
		uint32_t m_height;   /**< Height of the framebuffer */
		bool m_dirty;        /**< Dirty flag */
/// @endcond 
	};
}
