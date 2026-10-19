#pragma once
#include <stdint.h>
#include "tool/file.hpp"

namespace berialdraw
{
	class Framebuf;

	/** PNG encoder. Writes ARGB8888 framebuffer pixels as RGBA PNG data, preserving alpha. */
	class PngEncoder
	{
	public:
		/** Indicates whether the location has a PNG filename extension. */
		static bool supports(const File::Location& location);

		/** Encode a framebuffer to a PNG file
		@param source Framebuffer containing packed 0xAARRGGBB pixels
		@param location Output file location; optional compression query parameter accepts 0-9
		@return True when the PNG was written successfully */
		static bool encode(Framebuf& source, const File::Location& location);

	private:
		PngEncoder() = delete;
	};
}