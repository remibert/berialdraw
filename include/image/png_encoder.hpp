#pragma once
#include <stdint.h>

namespace berialdraw
{
	class Framebuf;

	/** PNG encoder. Writes ARGB8888 framebuffer pixels as RGBA PNG data, preserving alpha. */
	class PngEncoder
	{
	public:
		/** Encode a framebuffer to a PNG file
		@param source Framebuffer containing packed 0xAARRGGBB pixels
		@param filename Output file path
		@param compression_level PNG compression level (-1 for library default, 0-9 otherwise)
		@return True when the PNG was written successfully */
		static bool encode(Framebuf& source, const char* filename, int compression_level = -1);

	private:
		PngEncoder() = delete;
	};
}