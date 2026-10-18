#include "berialdraw_imp.hpp"

using namespace berialdraw;

namespace
{
	// Blend a source ARGB pixel over a straight-alpha destination pixel.
	inline uint32_t blend_argb8888(uint32_t destination, uint32_t source)
	{
		uint32_t result = destination;
		uint32_t source_alpha = source >> 24;
		uint32_t destination_alpha = destination >> 24;

		if (source_alpha == 255)
		{
			result = source;
		}
		else if (source_alpha > 0)
		{
			uint32_t inverse_alpha = 255 - source_alpha;
			if (destination_alpha == 0)
			{
				result = source;
			}
			else if (destination_alpha == 255)
			{
				// Preserve the legacy RGB blend rounding for opaque screen pixels.
				uint32_t legacy_inverse_alpha = 256 - source_alpha;
				int32_t source_red = (source >> 16) & 0xFF;
				int32_t source_green = (source >> 8) & 0xFF;
				int32_t source_blue = source & 0xFF;
				int32_t destination_red = (destination >> 16) & 0xFF;
				int32_t destination_green = (destination >> 8) & 0xFF;
				int32_t destination_blue = destination & 0xFF;
				int32_t red = (((destination_red - source_red) * (int32_t)legacy_inverse_alpha) >> 8) + source_red;
				int32_t green = (((destination_green - source_green) * (int32_t)legacy_inverse_alpha) >> 8) + source_green;
				int32_t blue = (((destination_blue - source_blue) * (int32_t)legacy_inverse_alpha) >> 8) + source_blue;
				result = 0xFF000000 | ((uint32_t)(red & 0xFF) << 16) | ((uint32_t)(green & 0xFF) << 8) | (uint32_t)(blue & 0xFF);
			}
			else
			{
				uint32_t output_alpha_numerator = source_alpha * 255 + destination_alpha * inverse_alpha;
				uint32_t output_alpha = (output_alpha_numerator + 127) / 255;
				uint32_t source_red = (source >> 16) & 0xFF;
				uint32_t source_green = (source >> 8) & 0xFF;
				uint32_t source_blue = source & 0xFF;
				uint32_t destination_red = (destination >> 16) & 0xFF;
				uint32_t destination_green = (destination >> 8) & 0xFF;
				uint32_t destination_blue = destination & 0xFF;
				uint32_t red = (source_red * source_alpha * 255 + destination_red * destination_alpha * inverse_alpha + output_alpha_numerator / 2) / output_alpha_numerator;
				uint32_t green = (source_green * source_alpha * 255 + destination_green * destination_alpha * inverse_alpha + output_alpha_numerator / 2) / output_alpha_numerator;
				uint32_t blue = (source_blue * source_alpha * 255 + destination_blue * destination_alpha * inverse_alpha + output_alpha_numerator / 2) / output_alpha_numerator;
				result = (output_alpha << 24) | (red << 16) | (green << 8) | blue;
			}
		}

		return result;
	}
}



Argb8888::Argb8888(uint32_t width, uint32_t height)
{
	m_dirty = true;
	m_width  = width;
	m_height = height;
	m_buffer = new uint32_t[(size_t)m_width * (size_t)m_height]();
}

Argb8888::~Argb8888()
{
	delete[] m_buffer;
}

void     Argb8888::pixel(int32_t x, int32_t y, uint32_t col)
{
	if ((col >> 24) > 0 && y >= 0 && y < (int32_t)m_height && x >= 0 && x < (int32_t)m_width)
	{
		size_t index = (size_t)y * m_width + (size_t)x;
		m_buffer[index] = blend_argb8888(m_buffer[index], col);
		m_dirty = true;
	}
}

uint32_t Argb8888::pixel(int32_t x, int32_t y)
{
	uint32_t result = 0;

	if (y >= 0 && y < (int32_t)m_height && x >= 0 && x < (int32_t)m_width)
	{
		result = m_buffer[(size_t)y * m_width + (size_t)x];
	}
	return result;
}

void     Argb8888::paint_rect(int32_t x, int32_t y, uint32_t width, uint32_t height, uint32_t col)
{
	if ((col >> 24) > 0)
	{
		int64_t left = x;
		int64_t top = y;
		int64_t right = left + (int64_t)width;
		int64_t bottom = top + (int64_t)height;
		if (left < 0)
		{
			left = 0;
		}
		if (top < 0)
		{
			top = 0;
		}
		if (right > m_width)
		{
			right = m_width;
		}
		if (bottom > m_height)
		{
			bottom = m_height;
		}

		if (left < right && top < bottom)
		{
			m_dirty = true;
			uint32_t alpha = col >> 24;
			for (int64_t row = top; row < bottom; row++)
			{
				uint32_t* pixel = m_buffer + (size_t)row * m_width + (size_t)left;
				if (alpha == 255)
				{
					// Fill repeated 32-bit pixels; the standard algorithm can use optimized bulk stores.
					std::fill(pixel, pixel + (right - left), col);
				}
				else
				{
					for (int64_t column = left; column < right; column++)
					{
						*pixel = blend_argb8888(*pixel, col);
						pixel++;
					}
				}
			}
		}
	}
}

const uint8_t* Argb8888::buffer()
{
	return reinterpret_cast<const uint8_t*>(m_buffer);
}

uint32_t        Argb8888::width()
{
	return m_width;
}
uint32_t        Argb8888::height()
{
	return m_height;
}

void Argb8888::clear(uint32_t color)
{
	std::fill(m_buffer, m_buffer + (size_t)m_width * m_height, color);
	m_dirty = true;
}

bool Argb8888::dirty(int32_t val)
{
	bool result = m_dirty;
	if(val == (int32_t)true)
	{
		m_dirty = true;
	}
	else if (val == (int32_t)false)
	{
		m_dirty = false;
	}
	return m_dirty;
}

/** Compute crc32 on the content of framebuffer */
uint32_t Argb8888::crc32()
{
	uint32_t result = 0;
	uint8_t rgb[3];
	for (size_t index = 0; index < (size_t)m_width * m_height; index++)
	{
		uint32_t color = m_buffer[index];
		rgb[0] = (uint8_t)((color >> 16) & 0xFF);
		rgb[1] = (uint8_t)((color >> 8) & 0xFF);
		rgb[2] = (uint8_t)(color & 0xFF);
		compute_crc32(rgb, sizeof(rgb), result);
	}
	return result;
}

std::unique_ptr<Argb8888> Argb8888::crop(Framebuf& source, int32_t x, int32_t y, uint32_t width, uint32_t height)
{
	std::unique_ptr<Argb8888> result;
	int64_t left = x;
	int64_t top = y;
	int64_t right = left + (int64_t)width;
	int64_t bottom = top + (int64_t)height;

	if (left < 0)
	{
		left = 0;
	}
	if (top < 0)
	{
		top = 0;
	}
	if (right > source.width())
	{
		right = source.width();
	}
	if (bottom > source.height())
	{
		bottom = source.height();
	}

	if (left < right && top < bottom)
	{
		uint32_t crop_width = (uint32_t)(right - left);
		uint32_t crop_height = (uint32_t)(bottom - top);
		result = std::make_unique<Argb8888>(crop_width, crop_height);
		for (uint32_t row = 0; row < crop_height; row++)
		{
			for (uint32_t column = 0; column < crop_width; column++)
			{
				result->m_buffer[(size_t)row * crop_width + column] =
					source.pixel((int32_t)(left + column), (int32_t)(top + row));
			}
		}
		result->m_dirty = true;
	}

	return result;
}

#pragma pack(push, 1) // Disable structure alignment for BMP headers

typedef struct
{
	uint16_t bfType;      // Signature "BM"
	uint32_t bfSize;      // File size
	uint16_t bfReserved1; // Reserved (0)
	uint16_t bfReserved2; // Reserved (0)
	uint32_t bfOffBits;   // Offset of image data
} BITMAPFILEHEADER_;

typedef struct
{
	uint32_t biSize;           // Size of this header (40 bytes)
	int32_t  biWidth;          // Width of the image in pixels
	int32_t  biHeight;         // Height of the image in pixels (positive = top-down, negative = bottom-up)
	uint16_t biPlanes;         // Number of planes (must be 1)
	uint16_t biBitCount;       // Number of bits per pixel (24 for RGB)
	uint32_t biCompression;    // Compression (0 = no compression)
	uint32_t biSizeImage;      // Size of the image in bytes (can be 0 if uncompressed)
	int32_t  biXPelsPerMeter;  // Horizontal resolution (pixels per meter)
	int32_t  biYPelsPerMeter;  // Vertical resolution (pixels per meter)
	uint32_t biClrUsed;        // Number of colors in the palette (0 = no palette)
	uint32_t biClrImportant;   // Important colors (0 = all important)
} BITMAPINFOHEADER_;
#pragma pack(pop)


void Argb8888::to_bmp(const char * filename)
{
	File file;
	if (file.open(filename, "wb") == 0)
	{
		// Image size in bytes (row size must be padded to a multiple of 4 bytes)
		int row_stride = (m_width * 3 + 3) & (~3); // Align rows to 4 bytes
		int image_size = row_stride * m_height;

		// Constructing the BMP header
		BITMAPFILEHEADER_ file_header;
		file_header.bfType = 0x4D42;  // "BM"
		file_header.bfSize = sizeof(BITMAPFILEHEADER_) + sizeof(BITMAPINFOHEADER_) + image_size;
		file_header.bfReserved1 = 0;
		file_header.bfReserved2 = 0;
		file_header.bfOffBits = sizeof(BITMAPFILEHEADER_) + sizeof(BITMAPINFOHEADER_);

		BITMAPINFOHEADER_ info_header;
		info_header.biSize = sizeof(BITMAPINFOHEADER_);
		info_header.biWidth = m_width;
		info_header.biHeight = 0 - m_height;  // Negative for top-down image (positive for bottom-up)
		info_header.biPlanes = 1;
		info_header.biBitCount = 24;  // 24 bits for RGB
		info_header.biCompression = 0;
		info_header.biSizeImage = image_size;
		info_header.biXPelsPerMeter = 0;
		info_header.biYPelsPerMeter = 0;
		info_header.biClrUsed = 0;
		info_header.biClrImportant = 0;

		// Write headers to BMP file
		file.write(&file_header, sizeof(file_header));
		file.write(&info_header, sizeof(info_header));

		// Write image data with padding on rows if necessary
		uint8_t padding[3] = {0, 0, 0};  // Padding to align to 4 bytes
		uint8_t* row = new uint8_t[(size_t)m_width * 3];
		for (int y = 0; y < (int)m_height; ++y)
		{
			for (uint32_t x = 0; x < m_width; x++)
			{
				uint32_t color = m_buffer[(size_t)y * m_width + x];
				uint32_t alpha = color >> 24;
				uint32_t inverse_alpha = 255 - alpha;
				row[x * 3 + 0] = (uint8_t)(((color & 0xFF) * alpha + 255 * inverse_alpha + 127) / 255);
				row[x * 3 + 1] = (uint8_t)((((color >> 8) & 0xFF) * alpha + 255 * inverse_alpha + 127) / 255);
				row[x * 3 + 2] = (uint8_t)((((color >> 16) & 0xFF) * alpha + 255 * inverse_alpha + 127) / 255);
			}
			file.write(row, (int)m_width * 3);
			file.write(padding, (int)row_stride - (int)m_width * 3);  // Padding
		}
		delete[] row;
	}
}
