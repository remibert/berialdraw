#ifndef Hsl_hpp_INCLUDED
#define Hsl_hpp_INCLUDED

namespace berialdraw
{
	/** Conversion from hue saturation light to RGB color and inverse */
	class Hsl
	{
	public:
		/** Conversion from hue saturation light to RGB color
		@param hue hue value [0-359]
		@param saturation saturation [0-100]
		@param light light [0-100]
		@return rgb color */
		static uint32_t to_rgb(uint32_t hue, uint32_t saturation, uint32_t light);

		/** Conversion from RGB to hue saturation light 
		@param color color to convert
		@param hue value returned [0-359]
		@param saturation value returned [0-100]
		@param light value returned [0-100] */
		static void from_rgb(uint32_t color, uint32_t & hue, uint32_t & saturation, uint32_t &light);

		/** Adjust the brightness of selected color
		@param color color to convert
		@param light value added [-100..100]
		@return rgb color */
		static uint32_t adjust_light(uint32_t color, int32_t light);

		/** Adjust the saturation of selected color
		@param color color to convert
		@param saturation value added [-100..100]
		@return rgb color */
		static uint32_t adjust_saturation(uint32_t color, int32_t saturation);

		/** Convert color to gray
		@param color
		@return gray color */
		static uint32_t to_gray(uint32_t color);

		/** Convert a HS type color with a determined gray level to achieve, to have homogeneity in the colors
		@param hue value returned [0-359]
		@param saturation value returned [0-100]
		@param gray_level target gray level [0-255]
		@return rgb color */
		static uint32_t to_balanced_color(uint32_t hue, uint32_t saturation, uint32_t gray_level);

		/** Generate harmonious color
		@param hue value returned [0-359]
		@param current number from 1 to n_colors
		@param n_colors max colors
		@param saturation value returned [0-100]
		@param light_min light [0-100]
		@param light_max light [0-100]
		@return rgb color */
		static uint32_t harmonious_colors(uint32_t hue, uint32_t current, uint32_t n_colors=5, uint32_t saturation=80, uint32_t light_min=10, uint32_t light_max=80);

	};
}
#endif
