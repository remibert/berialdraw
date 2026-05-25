#include "berialdraw_imp.hpp"
#include <png.h>

using namespace berialdraw;

/** Custom read callback for libpng using berialdraw File abstraction */
static void png_read_from_file(png_structp png_ptr, png_bytep data, png_size_t length)
{
	File* file = (File*)png_get_io_ptr(png_ptr);
	if (file->read(data, (uint32_t)length) != (int)length)
	{
		png_error(png_ptr, "Read error");
	}
}

PngDecoder::PngDecoder()
{
}

PngDecoder::~PngDecoder()
{
	clear();
}

bool PngDecoder::decode(const char* filename)
{
	bool result = false;

	clear();

	if (filename)
	{
		File file;
		if (file.open(filename, "rb") != -1)
		{
			// Check PNG signature
			uint8_t header[8];
			if (file.read(header, 8) == 8 && png_sig_cmp(header, 0, 8) == 0)
			{
				png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
				if (png_ptr)
				{
					png_infop info_ptr = png_create_info_struct(png_ptr);
					if (info_ptr)
					{
						if (setjmp(png_jmpbuf(png_ptr)) == 0)
						{
							// Use custom read function with File abstraction
							png_set_read_fn(png_ptr, &file, png_read_from_file);
							png_set_sig_bytes(png_ptr, 8);
							png_read_info(png_ptr, info_ptr);

							m_width = png_get_image_width(png_ptr, info_ptr);
							m_height = png_get_image_height(png_ptr, info_ptr);
							png_byte color_type = png_get_color_type(png_ptr, info_ptr);
							png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);

							// Convert palette to RGB
							if (color_type == PNG_COLOR_TYPE_PALETTE)
							{
								png_set_palette_to_rgb(png_ptr);
							}

							// Convert grayscale to 8 bits
							if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
							{
								png_set_expand_gray_1_2_4_to_8(png_ptr);
							}

							// Add alpha channel if transparency info exists
							if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
							{
								png_set_tRNS_to_alpha(png_ptr);
							}

							// Convert 16 bit to 8 bit
							if (bit_depth == 16)
							{
								png_set_strip_16(png_ptr);
							}

							// Convert grayscale to RGB
							if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
							{
								png_set_gray_to_rgb(png_ptr);
							}

							// Add opaque alpha if no alpha channel
							if ((color_type & PNG_COLOR_MASK_ALPHA) == 0)
							{
								png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);
								m_has_alpha = false;
							}
							else
							{
								m_has_alpha = true;
							}

							// Swap R and B channels to match ARGB8888 (0xAARRGGBB) format
							// libpng outputs RGBA bytes, which on little-endian gives 0xAABBGGRR
							// png_set_bgr swaps to BGRA bytes = 0xAARRGGBB on little-endian
							png_set_bgr(png_ptr);

							png_read_update_info(png_ptr, info_ptr);

							// Allocate pixel buffer
							m_pixels = new uint32_t[m_width * m_height];

							// Allocate row pointers
							png_bytep* row_pointers = new png_bytep[m_height];
							for (uint32_t y = 0; y < m_height; y++)
							{
								row_pointers[y] = (png_bytep)&m_pixels[y * m_width];
							}

							png_read_image(png_ptr, row_pointers);
							png_read_end(png_ptr, nullptr);

							delete[] row_pointers;
							result = true;
						}
					}
					png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
				}
			}
			file.close();
		}
	}

	if (result == false)
	{
		clear();
	}

	return result;
}

const uint32_t* PngDecoder::pixel_data() const
{
	return m_pixels;
}

uint32_t PngDecoder::width() const
{
	return m_width;
}

uint32_t PngDecoder::height() const
{
	return m_height;
}

bool PngDecoder::has_alpha() const
{
	return m_has_alpha;
}

void PngDecoder::clear()
{
	if (m_pixels)
	{
		delete[] m_pixels;
		m_pixels = nullptr;
	}
	m_width = 0;
	m_height = 0;
	m_has_alpha = false;
}
