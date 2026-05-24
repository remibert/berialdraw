#pragma once
#include <stdint.h>

namespace berialdraw
{
	/** Abstract base class for image decoders.
	All image decoders (PNG, JPEG, etc.) must implement this interface. 
	Decoded pixels are stored in RGBA 32-bit format (8 bits per channel). */
	class ImageDecoder
	{
	public:
		/** Virtual destructor */
		virtual ~ImageDecoder();

		/** Decode an image from a file
		@param filename The path to the image file
		@return true if decoding succeeded, false otherwise */
		virtual bool decode(const char* filename) = 0;

		/** Get the decoded pixel data (RGBA format, 4 bytes per pixel)
		@return Pointer to the pixel data, or nullptr if not decoded */
		virtual const uint32_t* pixel_data() const = 0;

		/** Get the image width in pixels
		@return Width or 0 if not decoded */
		virtual uint32_t width() const = 0;

		/** Get the image height in pixels
		@return Height or 0 if not decoded */
		virtual uint32_t height() const = 0;

		/** Check if the image format supports an alpha channel
		@return true if the decoded image has alpha */
		virtual bool has_alpha() const = 0;

		/** Free the decoded pixel data */
		virtual void clear() = 0;

		/** Create the appropriate decoder based on file extension
		@param filename The path to the image file
		@return Pointer to a new ImageDecoder, or nullptr if format unsupported. Caller owns the pointer. */
		static ImageDecoder* create(const char* filename);
	};
}
