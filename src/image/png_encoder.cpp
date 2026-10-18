#include "berialdraw_imp.hpp"
#include <png.h>
#include <limits.h>
#include <limits>

using namespace berialdraw;

namespace
{
	// Write PNG bytes through the project file abstraction.
	void png_write_file(png_structp png_ptr, png_bytep data, png_size_t length)
	{
		File* file = reinterpret_cast<File*>(png_get_io_ptr(png_ptr));
		png_size_t remaining = length;
		while (remaining > 0)
		{
			uint32_t chunk = remaining > INT_MAX ? INT_MAX : (uint32_t)remaining;
			if (file == nullptr || file->write(data, chunk) != (int)chunk)
			{
				png_error(png_ptr, "PNG write error");
			}
			data += chunk;
			remaining -= chunk;
		}
	}

	// File::close flushes the buffered local file output.
	void png_flush_file(png_structp png_ptr)
	{
		(void)png_ptr;
	}
}

// Encode ARGB8888 pixels to an RGBA PNG while preserving transparency.
bool PngEncoder::encode(Framebuf& source, const char* filename, int compression_level)
{
	bool result = false;
	uint32_t width = source.width();
	uint32_t height = source.height();
	if (filename != nullptr && filename[0] != '\0' && width > 0 && height > 0 &&
		width <= 0x7FFFFFFFU && height <= 0x7FFFFFFFU && (size_t)width <= std::numeric_limits<size_t>::max() / 4 &&
		compression_level >= -1 && compression_level <= 9)
	{
		std::unique_ptr<File> file = std::make_unique<File>();
		if (file->open(filename, "wb") == 0)
		{
			png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
			if (png_ptr != nullptr)
			{
				png_infop info_ptr = png_create_info_struct(png_ptr);
				if (info_ptr != nullptr)
				{
					png_bytep volatile row = nullptr;
					if (setjmp(png_jmpbuf(png_ptr)) == 0)
					{
						png_set_write_fn(png_ptr, file.get(), png_write_file, png_flush_file);
						png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGBA,
							PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
						if (compression_level >= 0)
						{
							png_set_compression_level(png_ptr, compression_level);
						}
						png_write_info(png_ptr, info_ptr);

						row = new png_byte[(size_t)width * 4];
						for (uint32_t y = 0; y < height; y++)
						{
							png_bytep row_data = const_cast<png_bytep>(row);
							for (uint32_t x = 0; x < width; x++)
							{
								uint32_t pixel = source.pixel((int32_t)x, (int32_t)y);
								row_data[x * 4 + 0] = (png_byte)((pixel >> 16) & 0xFF);
								row_data[x * 4 + 1] = (png_byte)((pixel >> 8) & 0xFF);
								row_data[x * 4 + 2] = (png_byte)(pixel & 0xFF);
								row_data[x * 4 + 3] = (png_byte)(pixel >> 24);
							}
							png_write_row(png_ptr, row_data);
						}
						png_write_end(png_ptr, info_ptr);
						result = true;
					}
					delete[] const_cast<png_bytep>(row);
				}
				png_destroy_write_struct(&png_ptr, info_ptr != nullptr ? &info_ptr : nullptr);
			}
			if (file->close() != 0)
			{
				result = false;
			}
		}
	}
	return result;
}