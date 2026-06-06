#include "berialdraw_imp.hpp"

using namespace berialdraw;

// Clamp value to uint8 range [0, 255]
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

// Resize image with bicubic interpolation (Q6 fixed-point, no float)
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
		// Stretch to fill area
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

// Combine alpha channels: pixel_alpha * widget_alpha
uint8_t ImageProcessor::combine_alpha(uint8_t pixel_alpha, uint8_t widget_alpha)
{
	return (uint8_t)(((uint32_t)pixel_alpha * (uint32_t)widget_alpha) / 255u);
}

// Compute bounding box size after rotation (Q6 angles, Q16.16 trig)
void ImageProcessor::compute_rotated_size(
	uint32_t src_width,
	uint32_t src_height,
	Coord angle_q6,
	uint32_t & dst_width,
	uint32_t & dst_height)
{
	bool needs_complex_rotation = true;

	// Handle zero dimensions
	if (src_width == 0 || src_height == 0)
	{
		dst_width = 0;
		dst_height = 0;
		needs_complex_rotation = false;
	}

	if (needs_complex_rotation)
	{
		// Normalize angle to [0, 360) in Q6
		int32_t a = (int32_t)(angle_q6 % (360 << 6));
		if (a < 0) a += (360 << 6);

		// Handle exact multiples of 90 degrees
		if (a == 0)
		{
			dst_width = src_width;
			dst_height = src_height;
			needs_complex_rotation = false;
		}
		else if (a == (90 << 6) || a == (270 << 6))
		{
			dst_width = src_height;
			dst_height = src_width;
			needs_complex_rotation = false;
		}
		else if (a == (180 << 6))
		{
			dst_width = src_width;
			dst_height = src_height;
			needs_complex_rotation = false;
		}
	}

	// Perform complex rotation calculation if needed
	if (needs_complex_rotation)
	{
		// Normalize angle to [0, 360) in Q6
		int32_t a = (int32_t)(angle_q6 % (360 << 6));
		if (a < 0) a += (360 << 6);

		// Use FreeType's FT_Vector_Unit to get cos/sin in Q16.16
		// FT_ANGLE_PI2 = 90 degrees in FreeType angle units
		// FreeType angle = degrees * 65536 / 360 * 2*PI... actually FT_ANGLE_PI = 180*65536 = 0x10000 * 180
		// Actually FT_ANGLE_2PI = 0x40000 (= 2*PI), so angle_ft = angle_degrees * 0x40000 / 360
		// But we have angle in Q6 degrees, so angle_degrees_q16 = angle_q6 << 10
		int32_t angle_ft = (int32_t)((int64_t)a << 10);

		FT_Vector sincos;
		FT_Vector_Unit(&sincos, angle_ft);

		// sincos.x = cos(angle) in Q16.16, sincos.y = sin(angle) in Q16.16
		int64_t abs_cos = (sincos.x < 0) ? -sincos.x : sincos.x;
		int64_t abs_sin = (sincos.y < 0) ? -sincos.y : sincos.y;

		// new_w = |W * cos| + |H * sin|, new_h = |W * sin| + |H * cos| in Q16.16
		int64_t new_w = ((int64_t)src_width * abs_cos + (int64_t)src_height * abs_sin + (1LL << 15)) >> 16;
		int64_t new_h = ((int64_t)src_width * abs_sin + (int64_t)src_height * abs_cos + (1LL << 15)) >> 16;

		dst_width  = (uint32_t)(new_w > 0 ? new_w : 1);
		dst_height = (uint32_t)(new_h > 0 ? new_h : 1);
	}
}

// Rotate image with bilinear interpolation (Q6 angles, Q16.16 mapping, no float)
uint32_t* ImageProcessor::rotate_bilinear(
	const uint32_t* src_pixels,
	uint32_t src_width,
	uint32_t src_height,
	Coord angle_q6,
	uint32_t & dst_width,
	uint32_t & dst_height)
{
	uint32_t* result = nullptr;
	bool should_perform_rotation = false;

	if (src_pixels == nullptr || src_width == 0 || src_height == 0)
	{
		dst_width = 0;
		dst_height = 0;
	}
	else
	{
		// Normalize angle
		int32_t a = (int32_t)(angle_q6 % (360 << 6));
		if (a < 0) a += (360 << 6);

		// No rotation needed
		if (a == 0)
		{
			dst_width = src_width;
			dst_height = src_height;
			uint32_t count = src_width * src_height;
			result = new uint32_t[count];
			for (uint32_t i = 0; i < count; i++)
			{
				result[i] = src_pixels[i];
			}
		}
		else
		{
			should_perform_rotation = true;
		}
	}

	if (should_perform_rotation)
	{
		// Compute rotated bounding box
		compute_rotated_size(src_width, src_height, angle_q6, dst_width, dst_height);

		result = new uint32_t[dst_width * dst_height];

		// Initialize all pixels to transparent
		for (uint32_t i = 0; i < dst_width * dst_height; i++)
		{
			result[i] = 0;
		}

		// Normalize angle again for computation
		int32_t a = (int32_t)(angle_q6 % (360 << 6));
		if (a < 0) a += (360 << 6);

		// Get sin/cos via FreeType in Q16.16
		int32_t angle_ft = (int32_t)((int64_t)a << 10);
		FT_Vector sincos;
		FT_Vector_Unit(&sincos, angle_ft);

		// We need the INVERSE rotation to map dst -> src
		// inv_cos = cos(-angle) = cos(angle), inv_sin = sin(-angle) = -sin(angle)
		int64_t inv_cos = sincos.x;   // Q16.16
		int64_t inv_sin = -sincos.y;  // Q16.16

		// Center of source and destination images in Q16.16
		int64_t src_cx = ((int64_t)src_width  << 15);  // (src_width / 2) in Q16.16
		int64_t src_cy = ((int64_t)src_height << 15);
		int64_t dst_cx = ((int64_t)dst_width  << 15);
		int64_t dst_cy = ((int64_t)dst_height << 15);

		for (uint32_t dy = 0; dy < dst_height; dy++)
		{
			// Vector from dst center in Q16.16
			int64_t rel_y = ((int64_t)dy << 16) + (1LL << 15) - dst_cy;

			for (uint32_t dx = 0; dx < dst_width; dx++)
			{
				int64_t rel_x = ((int64_t)dx << 16) + (1LL << 15) - dst_cx;

				// Apply inverse rotation: src_rel = R^-1 * dst_rel
				// src_rel_x = rel_x * cos + rel_y * sin  (inv_sin = -sin, so rel_y * (-(-sin)) = rel_y * sin)
				// Actually: inv rotation matrix is [cos, sin; -sin, cos] for standard rotation
				int64_t src_rel_x = (rel_x * inv_cos - rel_y * inv_sin) >> 16;
				int64_t src_rel_y = (rel_x * inv_sin + rel_y * inv_cos) >> 16;

				// Convert back to source coordinates in Q16.16
				int64_t src_x_fp = src_rel_x + src_cx;
				int64_t src_y_fp = src_rel_y + src_cy;

				// Convert from Q16.16 to integer + fraction for bilinear interpolation
				// Subtract 0.5 pixel (Q16.16: 1<<15) to center the sampling
				src_x_fp -= (1LL << 15);
				src_y_fp -= (1LL << 15);

				int32_t src_x0 = (int32_t)(src_x_fp >> 16);
				int32_t src_y0 = (int32_t)(src_y_fp >> 16);

				// Check if completely outside source
				if (src_x0 < -1 || src_x0 >= (int32_t)src_width ||
					src_y0 < -1 || src_y0 >= (int32_t)src_height)
				{
					continue;  // Transparent pixel
				}

				// Fractional parts in Q16 (0..65535)
				uint32_t frac_x = (uint32_t)(src_x_fp & 0xFFFF);
				uint32_t frac_y = (uint32_t)(src_y_fp & 0xFFFF);

				// Handle negative coordinates properly
				if (src_x_fp < 0)
				{
					src_x0 = -1;
					frac_x = (uint32_t)((src_x_fp + (1LL << 16)) & 0xFFFF);
				}
				if (src_y_fp < 0)
				{
					src_y0 = -1;
					frac_y = (uint32_t)((src_y_fp + (1LL << 16)) & 0xFFFF);
				}

				int32_t x1 = src_x0 + 1;
				int32_t y1 = src_y0 + 1;

				// Bilinear weights in Q16
				uint32_t w_x1 = frac_x;
				uint32_t w_x0 = (1 << 16) - frac_x;
				uint32_t w_y1 = frac_y;
				uint32_t w_y0 = (1 << 16) - frac_y;

				// Sample 4 pixels (clamp to transparent if outside)
				uint32_t p00 = 0, p10 = 0, p01 = 0, p11 = 0;

				if (src_x0 >= 0 && src_x0 < (int32_t)src_width && src_y0 >= 0 && src_y0 < (int32_t)src_height)
				{
					p00 = src_pixels[src_y0 * src_width + src_x0];
				}
				if (x1 >= 0 && x1 < (int32_t)src_width && src_y0 >= 0 && src_y0 < (int32_t)src_height)
				{
					p10 = src_pixels[src_y0 * src_width + x1];
				}
				if (src_x0 >= 0 && src_x0 < (int32_t)src_width && y1 >= 0 && y1 < (int32_t)src_height)
				{
					p01 = src_pixels[y1 * src_width + src_x0];
				}
				if (x1 >= 0 && x1 < (int32_t)src_width && y1 >= 0 && y1 < (int32_t)src_height)
				{
					p11 = src_pixels[y1 * src_width + x1];
				}

				// Bilinear interpolation for each channel using Q16 fixed-point
				// w = w_x * w_y >> 16 gives weight in Q16
				int64_t w00 = ((int64_t)w_x0 * w_y0) >> 16;
				int64_t w10 = ((int64_t)w_x1 * w_y0) >> 16;
				int64_t w01 = ((int64_t)w_x0 * w_y1) >> 16;
				int64_t w11 = ((int64_t)w_x1 * w_y1) >> 16;

				int64_t r = (((p00 >> 16) & 0xFF) * w00 + ((p10 >> 16) & 0xFF) * w10 +
							 ((p01 >> 16) & 0xFF) * w01 + ((p11 >> 16) & 0xFF) * w11 + (1LL << 15)) >> 16;
				int64_t g = (((p00 >> 8) & 0xFF) * w00 + ((p10 >> 8) & 0xFF) * w10 +
							 ((p01 >> 8) & 0xFF) * w01 + ((p11 >> 8) & 0xFF) * w11 + (1LL << 15)) >> 16;
				int64_t b = ((p00 & 0xFF) * w00 + (p10 & 0xFF) * w10 +
							 (p01 & 0xFF) * w01 + (p11 & 0xFF) * w11 + (1LL << 15)) >> 16;
				int64_t aa = (((p00 >> 24) & 0xFF) * w00 + ((p10 >> 24) & 0xFF) * w10 +
							  ((p01 >> 24) & 0xFF) * w01 + ((p11 >> 24) & 0xFF) * w11 + (1LL << 15)) >> 16;

				result[dy * dst_width + dx] = ((uint32_t)clamp_byte((int32_t)aa) << 24) |
											  ((uint32_t)clamp_byte((int32_t)r)  << 16) |
											  ((uint32_t)clamp_byte((int32_t)g)  << 8)  |
											   (uint32_t)clamp_byte((int32_t)b);
			}
		}
	}

	return result;
}
