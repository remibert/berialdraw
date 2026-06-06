#pragma once
#include <stdint.h>

namespace berialdraw
{
	/** Image processing utility class. Provides bicubic resizing and alpha blending operations. */
	class ImageProcessor
	{
	public:
		/** Resize an image using bicubic interpolation
		@param src_pixels Source pixel data (RGBA 32-bit)
		@param src_width Source image width
		@param src_height Source image height
		@param dst_width Destination width
		@param dst_height Destination height
		@return Newly allocated pixel buffer with resized image. Caller owns the pointer. */
		static uint32_t* resize_bicubic(
			const uint32_t* src_pixels,
			uint32_t src_width,
			uint32_t src_height,
			uint32_t dst_width,
			uint32_t dst_height);

		/** Compute the destination size based on the fit mode
		@param src_width Source image width
		@param src_height Source image height
		@param area_width Available area width
		@param area_height Available area height
		@param fit_mode The image fit mode
		@param dst_width Output: computed destination width
		@param dst_height Output: computed destination height */
		static void compute_fit_size(
			uint32_t src_width,
			uint32_t src_height,
			uint32_t area_width,
			uint32_t area_height,
			ImageFitMode fit_mode,
			uint32_t & dst_width,
			uint32_t & dst_height);

		/** Rotate an image using bilinear interpolation (Q6 fixed-point, no float/double).
		The output image is larger to contain the full rotated image, with transparent pixels
		in areas outside the rotated source.
		@param src_pixels Source pixel data (RGBA 32-bit)
		@param src_width Source image width
		@param src_height Source image height
		@param angle_q6 Rotation angle in Q6 fixed-point degrees (64 = 1 degree)
		@param dst_width Output: width of the rotated image
		@param dst_height Output: height of the rotated image
		@return Newly allocated pixel buffer with rotated image. Caller owns the pointer. */
		static uint32_t* rotate_bilinear(
			const uint32_t* src_pixels,
			uint32_t src_width,
			uint32_t src_height,
			Coord angle_q6,
			uint32_t & dst_width,
			uint32_t & dst_height);

		/** Compute the bounding box size of a rotated image
		@param src_width Source image width
		@param src_height Source image height
		@param angle_q6 Rotation angle in Q6 fixed-point degrees (64 = 1 degree)
		@param dst_width Output: width of the bounding box
		@param dst_height Output: height of the bounding box */
		static void compute_rotated_size(
			uint32_t src_width,
			uint32_t src_height,
			Coord angle_q6,
			uint32_t & dst_width,
			uint32_t & dst_height);

		/** Combine pixel alpha with widget alpha
		@param pixel_alpha The pixel's own alpha (0-255)
		@param widget_alpha The widget-level alpha (0-255)
		@return Combined alpha value (0-255) */
		static uint8_t combine_alpha(uint8_t pixel_alpha, uint8_t widget_alpha);

	protected:
		/** Clamp a value to 0-255 range
		@param val Value to clamp
		@return Clamped value */
		static uint8_t clamp_byte(int32_t val);
	};
}
