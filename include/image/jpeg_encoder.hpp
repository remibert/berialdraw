#pragma once
#include <stdint.h>

namespace berialdraw
{
	class Framebuf;

	/** JPEG encoder. JPEG has no alpha channel, so transparent pixels are composited on a background. */
	class JpegEncoder
	{
	public:
		/** Encode a framebuffer to a JPEG file
		@param source Framebuffer containing packed 0xAARRGGBB pixels
		@param filename Output file path
		@param background_color Opaque ARGB color used behind transparent pixels (default white)
		@param quality JPEG quality from 1 to 100 (default 90)
		@return True when the JPEG was written successfully */
		static bool encode(Framebuf& source, const char* filename, uint32_t background_color = 0xFFFFFFFF, int quality = 90);

	private:
		JpegEncoder() = delete;
	};
}