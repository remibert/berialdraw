#include "berialdraw_imp.hpp"
#include <jpeglib.h>
#include <setjmp.h>
#include <limits.h>
#include <limits>

using namespace berialdraw;

namespace
{
	struct JpegEncoderErrorManager
	{
		struct jpeg_error_mgr pub;
		jmp_buf setjmp_buffer;
	};

	struct JpegEncoderOutput
	{
		unsigned char* data = nullptr;
		unsigned long size = 0;
		uint8_t* row = nullptr;
	};

	// Convert a straight-alpha channel to an opaque channel over the matte.
	uint8_t composite_channel(uint8_t source, uint32_t alpha, uint8_t background)
	{
		uint32_t inverse_alpha = 255 - alpha;
		return (uint8_t)((source * alpha + background * inverse_alpha + 127) / 255);
	}

	// Redirect libjpeg fatal errors to the active setjmp recovery point.
	void jpeg_encoder_error_exit(j_common_ptr cinfo)
	{
		JpegEncoderErrorManager* error = reinterpret_cast<JpegEncoderErrorManager*>(cinfo->err);
		longjmp(error->setjmp_buffer, 1);
	}
}

// Encode ARGB8888 pixels to JPEG, compositing alpha over the requested matte.
bool JpegEncoder::encode(Framebuf& source, const char* filename, uint32_t background_color, int quality)
{
	bool result = false;
	uint32_t width = source.width();
	uint32_t height = source.height();
	if (filename != nullptr && filename[0] != '\0' && width > 0 && height > 0 &&
		width <= JPEG_MAX_DIMENSION && height <= JPEG_MAX_DIMENSION && (size_t)width <= std::numeric_limits<size_t>::max() / 3 &&
		quality >= 1 && quality <= 100)
	{
		std::unique_ptr<jpeg_compress_struct> compressor = std::make_unique<jpeg_compress_struct>();
		std::unique_ptr<JpegEncoderErrorManager> error = std::make_unique<JpegEncoderErrorManager>();
		std::unique_ptr<JpegEncoderOutput> output = std::make_unique<JpegEncoderOutput>();
		compressor->err = jpeg_std_error(&error->pub);
		error->pub.error_exit = jpeg_encoder_error_exit;

		if (setjmp(error->setjmp_buffer) == 0)
		{
			jpeg_create_compress(compressor.get());
			jpeg_mem_dest(compressor.get(), &output->data, &output->size);
			compressor->image_width = width;
			compressor->image_height = height;
			compressor->input_components = 3;
			compressor->in_color_space = JCS_RGB;
			jpeg_set_defaults(compressor.get());
			jpeg_set_quality(compressor.get(), quality, TRUE);

			output->row = new uint8_t[(size_t)width * 3];
			jpeg_start_compress(compressor.get(), TRUE);
			uint32_t background_red = (background_color >> 16) & 0xFF;
			uint32_t background_green = (background_color >> 8) & 0xFF;
			uint32_t background_blue = background_color & 0xFF;
			while (compressor->next_scanline < compressor->image_height)
			{
				uint32_t y = compressor->next_scanline;
				for (uint32_t x = 0; x < width; x++)
				{
					uint32_t pixel = source.pixel((int32_t)x, (int32_t)y);
					uint32_t alpha = pixel >> 24;
					output->row[x * 3 + 0] = composite_channel((uint8_t)(pixel >> 16), alpha, (uint8_t)background_red);
					output->row[x * 3 + 1] = composite_channel((uint8_t)(pixel >> 8), alpha, (uint8_t)background_green);
					output->row[x * 3 + 2] = composite_channel((uint8_t)pixel, alpha, (uint8_t)background_blue);
				}
				JSAMPROW row_pointer = output->row;
				jpeg_write_scanlines(compressor.get(), &row_pointer, 1);
			}
			jpeg_finish_compress(compressor.get());
			result = true;
		}

		jpeg_destroy_compress(compressor.get());
		delete[] output->row;

		if (result)
		{
			File file;
			if (file.open(filename, "wb") == 0)
			{
				unsigned long written = 0;
				while (written < output->size)
				{
					uint32_t chunk = output->size - written > (unsigned long)INT_MAX ? INT_MAX : (uint32_t)(output->size - written);
					if (file.write(output->data + written, chunk) != (int)chunk)
					{
						result = false;
						break;
					}
					written += chunk;
				}
				if (file.close() != 0)
				{
					result = false;
				}
			}
			else
			{
				result = false;
			}
		}

		free(output->data);
	}
	return result;
}