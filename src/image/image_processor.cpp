#include "berialdraw_imp.hpp"
#include <math.h>

using namespace berialdraw;

double ImageProcessor::bicubic_weight(double t)
{
	double result;
	double at = fabs(t);

	if (at <= 1.0)
	{
		result = (1.5 * at * at * at) - (2.5 * at * at) + 1.0;
	}
	else if (at < 2.0)
	{
		result = (-0.5 * at * at * at) + (2.5 * at * at) - (4.0 * at) + 2.0;
	}
	else
	{
		result = 0.0;
	}

	return result;
}

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

		double x_ratio = (double)src_width / (double)dst_width;
		double y_ratio = (double)src_height / (double)dst_height;

		for (uint32_t dy = 0; dy < dst_height; dy++)
		{
			double src_y = (dy + 0.5) * y_ratio - 0.5;
			int32_t y0 = (int32_t)floor(src_y);

			for (uint32_t dx = 0; dx < dst_width; dx++)
			{
				double src_x = (dx + 0.5) * x_ratio - 0.5;
				int32_t x0 = (int32_t)floor(src_x);

				double r_sum = 0.0;
				double g_sum = 0.0;
				double b_sum = 0.0;
				double a_sum = 0.0;
				double w_sum = 0.0;

				for (int32_t j = -1; j <= 2; j++)
				{
					int32_t sy = y0 + j;
					// Clamp to image bounds
					if (sy < 0) sy = 0;
					if (sy >= (int32_t)src_height) sy = (int32_t)src_height - 1;

					double wy = bicubic_weight(src_y - (double)(y0 + j));

					for (int32_t i = -1; i <= 2; i++)
					{
						int32_t sx = x0 + i;
						// Clamp to image bounds
						if (sx < 0) sx = 0;
						if (sx >= (int32_t)src_width) sx = (int32_t)src_width - 1;

						double wx = bicubic_weight(src_x - (double)(x0 + i));
						double w = wx * wy;

						uint32_t pixel = src_pixels[sy * src_width + sx];
						r_sum += ((pixel >> 16) & 0xFF) * w;
						g_sum += ((pixel >> 8) & 0xFF) * w;
						b_sum += ((pixel) & 0xFF) * w;
						a_sum += ((pixel >> 24) & 0xFF) * w;
						w_sum += w;
					}
				}

				if (w_sum > 0.0)
				{
					r_sum /= w_sum;
					g_sum /= w_sum;
					b_sum /= w_sum;
					a_sum /= w_sum;
				}

				uint8_t r = clamp_byte((int32_t)(r_sum + 0.5));
				uint8_t g = clamp_byte((int32_t)(g_sum + 0.5));
				uint8_t b = clamp_byte((int32_t)(b_sum + 0.5));
				uint8_t a = clamp_byte((int32_t)(a_sum + 0.5));

				result[dy * dst_width + dx] = ((uint32_t)a << 24) | ((uint32_t)r << 16) | ((uint32_t)g << 8) | (uint32_t)b;
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
	else if (fit_mode == IMAGE_FIT_STRETCH)
	{
		dst_width = area_width;
		dst_height = area_height;
	}
	else if (fit_mode == IMAGE_FIT_NONE)
	{
		dst_width = src_width;
		dst_height = src_height;
	}
	else if (fit_mode == IMAGE_FIT_ASPECT_WIDTH)
	{
		dst_width = area_width;
		dst_height = (uint32_t)(((uint64_t)src_height * (uint64_t)area_width) / (uint64_t)src_width);
	}
	else if (fit_mode == IMAGE_FIT_ASPECT_HEIGHT)
	{
		dst_height = area_height;
		dst_width = (uint32_t)(((uint64_t)src_width * (uint64_t)area_height) / (uint64_t)src_height);
	}
	else if (fit_mode == IMAGE_FIT_ASPECT_FIT)
	{
		double ratio_w = (double)area_width / (double)src_width;
		double ratio_h = (double)area_height / (double)src_height;

		if (ratio_w < ratio_h)
		{
			dst_width = area_width;
			dst_height = (uint32_t)((double)src_height * ratio_w + 0.5);
		}
		else
		{
			dst_height = area_height;
			dst_width = (uint32_t)((double)src_width * ratio_h + 0.5);
		}
	}
	else if (fit_mode == IMAGE_FIT_ASPECT_FILL)
	{
		double ratio_w = (double)area_width / (double)src_width;
		double ratio_h = (double)area_height / (double)src_height;

		if (ratio_w > ratio_h)
		{
			dst_width = area_width;
			dst_height = (uint32_t)((double)src_height * ratio_w + 0.5);
		}
		else
		{
			dst_height = area_height;
			dst_width = (uint32_t)((double)src_width * ratio_h + 0.5);
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
