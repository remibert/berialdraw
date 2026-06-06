#include "berialdraw_imp.hpp"
#include <jpeglib.h>
#include <setjmp.h>

using namespace berialdraw;

/** Custom error handler for libjpeg */
struct JpegErrorManager
{
	struct jpeg_error_mgr pub;
	jmp_buf setjmp_buffer;
};

static void jpeg_error_exit(j_common_ptr cinfo)
{
	JpegErrorManager* err = (JpegErrorManager*)cinfo->err;
	longjmp(err->setjmp_buffer, 1);
}

/** Custom source manager for libjpeg using berialdraw File abstraction */
struct JpegFileSourceManager
{
	struct jpeg_source_mgr pub;
	File* file;
	uint8_t buffer[4096];
};

static void jpeg_init_source(j_decompress_ptr cinfo)
{
	(void)cinfo;
}

static boolean jpeg_fill_input_buffer(j_decompress_ptr cinfo)
{
	JpegFileSourceManager* src = (JpegFileSourceManager*)cinfo->src;
	int bytes_read = src->file->read(src->buffer, sizeof(src->buffer));
	if (bytes_read <= 0)
	{
		// Insert a fake EOI marker
		src->buffer[0] = 0xFF;
		src->buffer[1] = JPEG_EOI;
		bytes_read = 2;
	}
	src->pub.next_input_byte = src->buffer;
	src->pub.bytes_in_buffer = (size_t)bytes_read;
	return TRUE;
}

static void jpeg_skip_input_data(j_decompress_ptr cinfo, long num_bytes)
{
	JpegFileSourceManager* src = (JpegFileSourceManager*)cinfo->src;
	if (num_bytes > 0)
	{
		while (num_bytes > (long)src->pub.bytes_in_buffer)
		{
			num_bytes -= (long)src->pub.bytes_in_buffer;
			jpeg_fill_input_buffer(cinfo);
		}
		src->pub.next_input_byte += num_bytes;
		src->pub.bytes_in_buffer -= (size_t)num_bytes;
	}
}

static void jpeg_term_source(j_decompress_ptr cinfo)
{
	(void)cinfo;
}

static void jpeg_set_file_source(j_decompress_ptr cinfo, JpegFileSourceManager* src, File* file)
{
	src->file = file;
	src->pub.init_source = jpeg_init_source;
	src->pub.fill_input_buffer = jpeg_fill_input_buffer;
	src->pub.skip_input_data = jpeg_skip_input_data;
	src->pub.resync_to_restart = jpeg_resync_to_restart;
	src->pub.term_source = jpeg_term_source;
	src->pub.bytes_in_buffer = 0;
	src->pub.next_input_byte = nullptr;
	cinfo->src = (struct jpeg_source_mgr*)src;
}

// Constructor
JpegDecoder::JpegDecoder()
{
}

// Destructor
JpegDecoder::~JpegDecoder()
{
	clear();
}

// Decode JPEG file to RGBA pixels
bool JpegDecoder::decode(const char* filename)
{
	bool result = false;

	clear();

	if (filename)
	{
		File file;
		if (file.open(filename, "rb") != -1)
		{
			struct jpeg_decompress_struct cinfo;
			JpegErrorManager jerr;
			JpegFileSourceManager src_mgr;

			cinfo.err = jpeg_std_error(&jerr.pub);
			jerr.pub.error_exit = jpeg_error_exit;

			if (setjmp(jerr.setjmp_buffer) == 0)
			{
				jpeg_create_decompress(&cinfo);

				// Configure custom source manager
				jpeg_set_file_source(&cinfo, &src_mgr, &file);

				jpeg_read_header(&cinfo, TRUE);
				cinfo.out_color_space = JCS_RGB;
				jpeg_start_decompress(&cinfo);

				m_width = cinfo.output_width;
				m_height = cinfo.output_height;

				// Allocate output buffer
				m_pixels = new uint32_t[m_width * m_height];

				// Allocate one scanline buffer (RGB)
				uint8_t* row_buffer = new uint8_t[m_width * 3];

				// Decode scanlines
				uint32_t row = 0;
				while (cinfo.output_scanline < cinfo.output_height)
				{
					JSAMPROW row_ptr = row_buffer;
					jpeg_read_scanlines(&cinfo, &row_ptr, 1);

					// Convert RGB scanline to RGBA pixels
					for (uint32_t x = 0; x < m_width; x++)
					{
						uint8_t r = row_buffer[x * 3 + 0];
						uint8_t g = row_buffer[x * 3 + 1];
						uint8_t b = row_buffer[x * 3 + 2];
						m_pixels[row * m_width + x] = (0xFFu << 24) | ((uint32_t)r << 16) | ((uint32_t)g << 8) | (uint32_t)b;
					}
					row++;
				}

				delete[] row_buffer;

				jpeg_finish_decompress(&cinfo);
				result = true;
			}
			jpeg_destroy_decompress(&cinfo);
			file.close();
		}
	}

	if (result == false)
	{
		clear();
	}

	return result;
}

// Get decoded pixels
const uint32_t* JpegDecoder::pixel_data() const
{
	return m_pixels;
}

// Get image width
uint32_t JpegDecoder::width() const
{
	return m_width;
}

// Get image height
uint32_t JpegDecoder::height() const
{
	return m_height;
}

// Check for alpha channel (JPEG never has alpha)
bool JpegDecoder::has_alpha() const
{
	return false;
}

// Clear and deallocate
void JpegDecoder::clear()
{
	if (m_pixels)
	{
		delete[] m_pixels;
		m_pixels = nullptr;
	}
	m_width = 0;
	m_height = 0;
}
