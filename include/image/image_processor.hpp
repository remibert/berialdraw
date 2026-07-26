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
		@param src_size Source image size
		@param dst_size Destination size
		@return Newly allocated pixel buffer with resized image. Caller owns the pointer. */
		static uint32_t* resize_bicubic(
			const uint32_t* src_pixels,
			const Size& src_size,
			const Size& dst_size);

		/** Compute the destination size based on the fit mode
		@param src_size Source image size
		@param area_size Available area size
		@param fit_mode The image fit mode
		@param dst_size Output: computed destination size */
		static void compute_fit_size(
			const Size& src_size,
			const Size& area_size,
			ImageFitMode fit_mode,
			Size & dst_size);

		/** Rotate an image using bilinear interpolation (Q6 fixed-point, no float/double).
		The output image is larger to contain the full rotated image, with transparent pixels
		in areas outside the rotated source.
		@param src_pixels Source pixel data (RGBA 32-bit)
		@param src_size Source image size
		@param angle_q6 Rotation angle in Q6 fixed-point degrees (64 = 1 degree)
		@param dst_size Output: size of the rotated image
		@return Newly allocated pixel buffer with rotated image. Caller owns the pointer. */
		static uint32_t* rotate_bilinear(
			const uint32_t* src_pixels,
			const Size& src_size,
			Coord angle_q6,
			Size & dst_size);

		/** Compute the bounding box size of a rotated image
		@param src_size Source image size
		@param angle_q6 Rotation angle in Q6 fixed-point degrees (64 = 1 degree)
		@param dst_size Output: size of the bounding box */
		static void compute_rotated_size(
			const Size& src_size,
			Coord angle_q6,
			Size & dst_size);

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
