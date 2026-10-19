#pragma once
#include <stdint.h>
#include "tool/file.hpp"

namespace berialdraw
{
	class Framebuf;

	/** JPEG encoder. JPEG has no alpha channel, so transparent pixels are composited on a background. */
	class JpegEncoder
	{
	public:
		/** Indicates whether the location has a JPEG filename extension. */
		static bool supports(const File::Location& location);

		/** Encode a framebuffer to a JPEG file
		@param source Framebuffer containing packed 0xAARRGGBB pixels
		@param location Output file location; optional quality (1-100) and numeric background query parameters
		@param default_background_color Opaque ARGB fallback behind transparent pixels
		@return True when the JPEG was written successfully */
		static bool encode(Framebuf& source, const File::Location& location, uint32_t default_background_color = 0xFFFFFFFF);

	private:
		JpegEncoder() = delete;
	};
}