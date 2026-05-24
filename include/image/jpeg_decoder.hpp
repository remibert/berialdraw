#pragma once
#include <stdint.h>

namespace berialdraw
{
	/** JPEG image decoder. Uses libjpeg-turbo to decode JPEG files. No alpha channel support. */
	class JpegDecoder : public ImageDecoder
	{
	public:
		/** Constructor */
		JpegDecoder();

		/** Destructor */
		virtual ~JpegDecoder();

		/** Decode a JPEG file
		@param filename The path to the JPEG file
		@return true if decoding succeeded */
		virtual bool decode(const char* filename) override;

		/** Get the decoded pixel data (RGBA format, alpha set to 255)
		@return Pointer to the pixel data */
		virtual const uint32_t* pixel_data() const override;

		/** Get the image width
		@return Width in pixels */
		virtual uint32_t width() const override;

		/** Get the image height
		@return Height in pixels */
		virtual uint32_t height() const override;

		/** Check if the image has alpha channel
		@return false (JPEG has no alpha) */
		virtual bool has_alpha() const override;

		/** Free the decoded pixel data */
		virtual void clear() override;

	protected:
	/// @cond DOXYGEN_IGNORE
		uint32_t* m_pixels = nullptr;
		uint32_t  m_width = 0;
		uint32_t  m_height = 0;
	/// @endcond
	};
}
