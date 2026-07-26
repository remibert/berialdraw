#pragma once
#include <stdint.h>

namespace berialdraw
{
	/** PNG image decoder. Uses libpng to decode PNG files with full alpha channel support. */
	class PngDecoder : public ImageDecoder
	{
	public:
		/** Constructor */
		PngDecoder();

		/** Destructor */
		virtual ~PngDecoder();

		/** Decode a PNG file
		@param filename The path to the PNG file
		@return true if decoding succeeded */
		virtual bool decode(const char* filename) override;

		/** Get the decoded pixel data (RGBA format)
		@return Pointer to the pixel data */
		virtual const uint32_t* pixel_data() const override;

		/** Get the image dimensions
		@return Size object with width and height */
		virtual const Size& size() const override;

		/** Check if the image has alpha channel
		@return true (PNG supports alpha) */
		virtual bool has_alpha() const override;

		/** Free the decoded pixel data */
		virtual void clear() override;

	protected:
	/// @cond DOXYGEN_IGNORE
		uint32_t* m_pixels = nullptr;
		Size      m_size;
		bool      m_has_alpha = false;
	/// @endcond
	};
}
