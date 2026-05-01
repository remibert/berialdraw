#include "berialdraw_imp.hpp"

using namespace berialdraw;



Argb8888::Argb8888(uint32_t width, uint32_t height)
{
	m_dirty = true;
	m_width  = width;
	m_height = height;
	m_buffer = new uint8_t[(int)m_width * (int)m_height * 3]();
}

Argb8888::~Argb8888()
{
	delete[] m_buffer;
}

void     Argb8888::pixel(int32_t x, int32_t y, uint32_t col)
{
	uint32_t alpha = (uint32_t)((col & 0xFF000000) >> 24);
	if (alpha > 0)
	{
		uint32_t inv_alpha = 256 - alpha;

		m_dirty = true;

		// If the rectangle is not outside the screen
		if (y >= 0 && y < (int32_t)m_height && x >= 0 && x < (int32_t)m_width)
		{
			uint32_t red   = (uint32_t)((col & 0x00FF0000) >> 16);
			uint32_t green = (uint32_t)((col & 0x0000FF00) >> 8);
			uint32_t blue  = (uint32_t)((col & 0x000000FF));
			uint8_t* buf   = &((uint8_t*)m_buffer)[(x * 3) + (y * m_width * 3)];

			if (alpha == 0xFF)
			{
				*buf++ = red;
				*buf++ = green;
				*buf++ = blue;
			}
			else
			{
				*buf = ((((*buf - red  ) * inv_alpha) >> 8) + red  ); buf++;
				*buf = ((((*buf - green) * inv_alpha) >> 8) + green); buf++;
				*buf = ((((*buf - blue ) * inv_alpha) >> 8) + blue ); buf++;
			}
		}
	}
}

uint32_t Argb8888::pixel(int32_t x, int32_t y)
{
	uint32_t result = 0;

	// If the rectangle is not outside the screen
	if (y >= 0 && y < (int32_t)m_height && x >= 0 && x < (int32_t)m_width)
	{
		uint8_t* buf = &((uint8_t*)m_buffer)[(x * 3) + (y * m_width * 3)];

		result = ((uint32_t)*buf++) << 16;
		result |= ((uint32_t)*buf++) << 8;
		result |= ((uint32_t)*buf++);
		result |= 0xFF000000;
	}
	return result;
}

void     Argb8888::fill_rect(int32_t x, int32_t y, uint32_t width, uint32_t height, uint32_t col)
{
	uint8_t alpha = (uint8_t)((col & 0xFF000000) >> 24);
	uint8_t inv_alpha = 256 - alpha;

	// If the color is visible
	if (alpha > 0)
	{
		m_dirty = true;

		// If the rectangle start on the left of screen
		if (x < 0)
		{
			// If the rectangle continu on the screen
			if (width > (unsigned int)-x)
			{
				// Adapt the width
				width += x;
			}
			else
			{
				// No width
				width = 0;
			}
			x = 0;
		}

		// If the rectangle start on before the top of screen
		if (y < 0)
		{
			// If the rectangle continu on the screen
			if (height > (unsigned int)-y)
			{
				// Adapt the height
				height += y;
			}
			else
			{
				// No height
				height = 0;
			}
			y = 0;
		}

		// If the rectangle is not outside the screen
		if (y < (int32_t)m_height && x < (int32_t)m_width && width > 0 && height > 0)
		{
			// If the rectangle exceed the screen width
			if (x + width >= m_width)
			{
				// Reduce the width
				width = m_width - x;
			}

			// If the rectangle exceed the screen height
			if (y + height >= m_height)
			{
				// Reduce the height
				height = m_height - y;
			}

			{
				uint8_t* buf   = &((uint8_t*)m_buffer)[(x * 3) + (y * m_width * 3)];
				uint8_t  red   = (uint8_t)((col & 0x00FF0000) >> 16);
				uint8_t  green = (uint8_t)((col & 0x0000FF00) >> 8);
				uint8_t  blue  = (uint8_t)((col & 0x000000FF));
				uint32_t offset_line = (m_width * 3);
				uint8_t* buf_line = buf;

				// If no transparency
				if (alpha == 0xFF)
				{
					uint32_t c1;
					uint32_t c2;
					uint32_t c3;
					uint16_t number = 0x1;
					char *ptr = reinterpret_cast<char*>(&number);
					if (ptr[0] == 1)
					{
						// Init color for fast line
						c1 = red   << 24 | blue  << 16 | green << 8 | red  ;
						c2 = green << 24 | red   << 16 | blue  << 8 | green;
						c3 = blue  << 24 | green << 16 | red   << 8 | blue ;
					}
					else
					{
						// Init color for fast line
						c1 = red   << 24 | green << 16 | blue  << 8 | red  ;
						c2 = green << 24 | blue  << 16 | red   << 8 | green;
						c3 = blue  << 24 | red   << 16 | green << 8 | blue ;
					}
					
					while (height--)
					{
						uint32_t width_remaining = width;
						if(width >= 16)
						{
							uint32_t * buf_fast = (uint32_t *)buf;

							// Fast set line
							for(unsigned int i = width/16; i; --i)
							{
								*buf_fast++ = c1;  // RGBR
								*buf_fast++ = c2;  // GBRG
								*buf_fast++ = c3;  // BRGB
								*buf_fast++ = c1;  // RGBR
								*buf_fast++ = c2;  // GBRG
								*buf_fast++ = c3;  // BRGB
								*buf_fast++ = c1;  // RGBR
								*buf_fast++ = c2;  // GBRG
								*buf_fast++ = c3;  // BRGB
								*buf_fast++ = c1;  // RGBR
								*buf_fast++ = c2;  // GBRG
								*buf_fast++ = c3;  // BRGB
							}
							width_remaining = width%16;
							buf = (uint8_t *)buf_fast;
						}
						for (unsigned int i = width_remaining; i; --i)
						{
							*buf++ = red;
							*buf++ = green;
							*buf++ = blue;
						}
						buf_line += offset_line;
						buf = buf_line;
					}
				}
				// else transparency
				else
				{
					while (height--)
					{
						for (unsigned int i = width; i; --i)
						{
							*buf = ((((*buf - red  ) * inv_alpha) >> 8) + red  ); buf++;
							*buf = ((((*buf - green) * inv_alpha) >> 8) + green); buf++;
							*buf = ((((*buf - blue ) * inv_alpha) >> 8) + blue ); buf++;
						}
						buf_line += offset_line;
						buf = buf_line;
					}
				}
			}
		}
	}
}

const uint8_t* Argb8888::buffer()
{
	return m_buffer;
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
	fill_rect(0,0,width(), height(), color);
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
	//to_bmp("result.bmp");
	return compute_crc32(m_buffer, m_width*m_height*3);
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
		for (int y = 0; y < (int)m_height; ++y) 
		{
			file.write(m_buffer + (int)y * (int)m_width * 3,(int) m_width * 3);
			file.write(padding, (int)row_stride - (int)m_width * 3);  // Padding
		}
	}
}
