#pragma once
namespace berialdraw
{
	/** Abstract framebuffer class */
	class Framebuf 
	{
	public:
		/** Framebuffer type enumeration */
		enum Type
		{
			ARGB8888, /**< ARGB8888 format */
		};

		/** Create a framebuffer
		@param width The width of the framebuffer
		@param height The height of the framebuffer
		@param type The type of the framebuffer
		@return Pointer to the created framebuffer */
		static Framebuf * create(uint32_t width, uint32_t height, enum Type type);

		/** Destructor */
		virtual ~Framebuf() {}

		/** Set the pixel color at (x, y)
		@param x The x-coordinate
		@param y The y-coordinate
		@param color The color to set */
		virtual void pixel(int32_t x, int32_t y, uint32_t color) = 0;

		/** Get the pixel color at (x, y)
		@param x The x-coordinate
		@param y The y-coordinate
		@return The color at the specified coordinates */
		virtual uint32_t pixel(int32_t x, int32_t y) = 0;

		/** Fill a rectangle with color
		@param x The x-coordinate of the rectangle
		@param y The y-coordinate of the rectangle
		@param width The width of the rectangle
		@param height The height of the rectangle
		@param color The color to fill */
		virtual void fill_rect(int32_t x, int32_t y, uint32_t width, uint32_t height, uint32_t color) = 0;

		/** Get the buffer
		@return Pointer to the buffer */
		virtual const uint8_t* buffer() = 0;

		/** Get the width of the framebuffer
		@return The width of the framebuffer */
		virtual uint32_t width() = 0;

		/** Get the height of the framebuffer
		@return The height of the framebuffer */
		virtual uint32_t height() = 0;

		/** Check if the framebuffer is dirty
		@param val Optional value to set the dirty flag
		@return True if the framebuffer is dirty, false otherwise */
		virtual bool dirty(int32_t val = -1) = 0;

		/** Clear the framebuffer with a color
		@param color The color to clear the framebuffer with */
		virtual void clear(uint32_t color = 0xFFF8F8F8) = 0;

		/** Compute crc32 on the content of framebuffer */
		virtual uint32_t crc32() = 0;
 
	private:
	};

}
