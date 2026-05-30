#include "berialdraw_imp.hpp"

using namespace berialdraw;

uint8_t ImageProcessor::clamp_byte(int32_t val)
{
	uint8_t result;

	if (val < 0)
	{
		result = 0;
	}
	else if (val > 255)
	{
		result = 255;
	}
	else
	{
		result = (uint8_t)val;
	}

	return result;
}

uint32_t* ImageProcessor::resize_bicubic(
	const uint32_t* src_pixels,
	uint32_t src_width,
	uint32_t src_height,
	uint32_t dst_width,
	uint32_t dst_height)
{
	uint32_t* result = nullptr;

	if (src_pixels && src_width > 0 && src_height > 0 && dst_width > 0 && dst_height > 0)
	{
		result = new uint32_t[dst_width * dst_height];

		// Fixed-point Q6 bicubic interpolation (ESP32/embedded optimized)
		// Uses 1 << 6 = 64 like freetype, to stay consistent with the project
		const int64_t SCALE = (1LL << 6);  // 2^6 = 64 (Q6 fixed-point)

		int64_t x_ratio = ((int64_t)src_width * SCALE) / (int64_t)dst_width;
		int64_t y_ratio = ((int64_t)src_height * SCALE) / (int64_t)dst_height;

		for (uint32_t dy = 0; dy < dst_height; dy++)
		{
			// src_y = (dy + 0.5) * y_ratio - 0.5 in Q6 fixed-point
			int64_t src_y_fp = (int64_t)dy * y_ratio + (y_ratio >> 1) - (SCALE >> 1);
			int32_t y0 = (int32_t)(src_y_fp >> 6);

			for (uint32_t dx = 0; dx < dst_width; dx++)
			{
				// src_x = (dx + 0.5) * x_ratio - 0.5 in Q6 fixed-point
				int64_t src_x_fp = (int64_t)dx * x_ratio + (x_ratio >> 1) - (SCALE >> 1);
				int32_t x0 = (int32_t)(src_x_fp >> 6);

				int64_t r_sum = 0;
				int64_t g_sum = 0;
				int64_t b_sum = 0;
				int64_t a_sum = 0;
				int64_t w_sum = 0;

				for (int32_t j = -1; j <= 2; j++)
				{
					int32_t sy = y0 + j;
					if (sy < 0) sy = 0;
					if (sy >= (int32_t)src_height) sy = (int32_t)src_height - 1;

					// Compute bicubic weight for Y (Q6 fixed-point)
					int64_t wy_t = src_y_fp - ((int64_t)(y0 + j) << 6);
					int64_t wy = 0;
					int64_t ay = (wy_t < 0) ? -wy_t : wy_t;
					if (ay <= SCALE)  // |t| <= 1
					{
						int64_t ay2 = (ay * ay) >> 6;
						int64_t ay3 = (ay2 * ay) >> 6;
						wy = 3 * ay3 - 5 * ay2 + 2 * SCALE;
						wy >>= 1;
					}
					else if (ay < 2 * SCALE)  // 1 < |t| < 2
					{
						int64_t ay2 = (ay * ay) >> 6;
						int64_t ay3 = (ay2 * ay) >> 6;
						wy = -ay3 + 5 * ay2 - 8 * ay + 4 * SCALE;
						wy >>= 1;
					}

					for (int32_t i = -1; i <= 2; i++)
					{
						int32_t sx = x0 + i;
						if (sx < 0) sx = 0;
						if (sx >= (int32_t)src_width) sx = (int32_t)src_width - 1;

						// Compute bicubic weight for X (Q6 fixed-point)
						int64_t wx_t = src_x_fp - ((int64_t)(x0 + i) << 6);
						int64_t wx = 0;
						int64_t ax = (wx_t < 0) ? -wx_t : wx_t;
						if (ax <= SCALE)  // |t| <= 1
						{
							int64_t ax2 = (ax * ax) >> 6;
							int64_t ax3 = (ax2 * ax) >> 6;
							wx = 3 * ax3 - 5 * ax2 + 2 * SCALE;
							wx >>= 1;
						}
						else if (ax < 2 * SCALE)  // 1 < |t| < 2
						{
							int64_t ax2 = (ax * ax) >> 6;
							int64_t ax3 = (ax2 * ax) >> 6;
							wx = -ax3 + 5 * ax2 - 8 * ax + 4 * SCALE;
							wx >>= 1;
						}

						int64_t w = (wx * wy) >> 6;  // Multiply and shift back to Q6

						uint32_t pixel = src_pixels[sy * src_width + sx];
						r_sum += ((pixel >> 16) & 0xFF) * w;
						g_sum += ((pixel >> 8) & 0xFF) * w;
						b_sum += ((pixel) & 0xFF) * w;
						a_sum += ((pixel >> 24) & 0xFF) * w;
						w_sum += w;
					}
				}

				if (w_sum > 0)
				{
					// r_sum is "pixel_value * Q6_weight", w_sum is "Q6_weight"
					// Division cancels Q6 factor, result is directly 0-255
					// Add w_sum/2 before division for rounding
					uint8_t r = clamp_byte((int32_t)((r_sum + (w_sum >> 1)) / w_sum));
					uint8_t g = clamp_byte((int32_t)((g_sum + (w_sum >> 1)) / w_sum));
					uint8_t b = clamp_byte((int32_t)((b_sum + (w_sum >> 1)) / w_sum));
					uint8_t a = clamp_byte((int32_t)((a_sum + (w_sum >> 1)) / w_sum));

					result[dy * dst_width + dx] = ((uint32_t)a << 24) | ((uint32_t)r << 16) | ((uint32_t)g << 8) | (uint32_t)b;
				}
				else
				{
					result[dy * dst_width + dx] = 0;
				}
			}
		}
	}
	return result;
}

void ImageProcessor::compute_fit_size(
	uint32_t src_width,
	uint32_t src_height,
	uint32_t area_width,
	uint32_t area_height,
	ImageFitMode fit_mode,
	uint32_t & dst_width,
	uint32_t & dst_height)
{
	dst_width = area_width;
	dst_height = area_height;

	if (src_width == 0 || src_height == 0 || area_width == 0 || area_height == 0)
	{
		dst_width = 0;
		dst_height = 0;
	}
	else if (fit_mode == STRETCH)
	{
		dst_width = area_width;
		dst_height = area_height;
	}
	else
	{
		// FIT: preserve aspect ratio, fit entirely inside area
		// Compare ratios using integer arithmetic: area_width/src_width vs area_height/src_height
		// Avoids floating-point by cross-multiplying: area_width * src_height vs area_height * src_width
		if ((uint64_t)area_width * (uint64_t)src_height < (uint64_t)area_height * (uint64_t)src_width)
		{
			// area_width/src_width is smaller ratio - use width as reference
			dst_width = area_width;
			dst_height = (uint32_t)(((uint64_t)area_width * (uint64_t)src_height + (uint64_t)src_width / 2) / (uint64_t)src_width);
		}
		else
		{
			// area_height/src_height is smaller ratio - use height as reference
			dst_height = area_height;
			dst_width = (uint32_t)(((uint64_t)area_height * (uint64_t)src_width + (uint64_t)src_height / 2) / (uint64_t)src_height);
		}
	}

	// Ensure minimum 1 pixel
	if (dst_width == 0) dst_width = 1;
	if (dst_height == 0) dst_height = 1;
}

uint8_t ImageProcessor::combine_alpha(uint8_t pixel_alpha, uint8_t widget_alpha)
{
	return (uint8_t)(((uint32_t)pixel_alpha * (uint32_t)widget_alpha) / 255u);
}
