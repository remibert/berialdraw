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

		/** Get the image dimensions
		@return Size object with width and height */
		virtual const Size& size() const override;

		/** Check if the image has alpha channel
		@return false (JPEG has no alpha) */
		virtual bool has_alpha() const override;

		/** Free the decoded pixel data */
		virtual void clear() override;

	protected:
	/// @cond DOXYGEN_IGNORE
		uint32_t* m_pixels = nullptr;
		Size      m_size;
	/// @endcond
	};
}
