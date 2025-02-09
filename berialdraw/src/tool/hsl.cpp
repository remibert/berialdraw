#include "berialdraw_imp.hpp"

using namespace berialdraw;

inline int32_t mapToPercent(int32_t value)
{
	if (value > 100)
	{
		value = 100;
	}
	else if (value < 0)
	{
		value = 0;
	}
	return value;
}

inline int32_t mapToRange(int32_t value)
{
	if (value > 100)
	{
		value = 100;
	}
	else if (value < -100)
	{
		value = -100;
	}
	return value;
}

/** Adds one color over another by managing the alpha channel
@param color color to change
@param added_color color to add
@param added_alpha alpha to add 
@return the new color */
uint32_t Hsl::add_color(uint32_t color, uint32_t added_color, uint32_t added_alpha)
{
	uint32_t inv_alpha = 256 - added_alpha;

	uint32_t fore_red   = ((added_color & 0x00FF0000) >> 16);
	uint32_t fore_green = ((added_color & 0x0000FF00) >> 8 );
	uint32_t fore_blue  = ((added_color & 0x000000FF)      );

	uint32_t color_red   = ((color & 0x00FF0000) >> 16);
	uint32_t color_green = ((color & 0x0000FF00) >> 8 );
	uint32_t color_blue  = ((color & 0x000000FF)      );

	uint32_t back_red   = ((((color_red   - fore_red  ) * inv_alpha)>> 8) + fore_red  );
	uint32_t back_green = ((((color_green - fore_green) * inv_alpha)>> 8) + fore_green);
	uint32_t back_blue  = ((((color_blue  - fore_blue ) * inv_alpha)>> 8) + fore_blue );

	uint32_t result = (color & 0xFF000000) | (back_red << 16) | (back_green << 8) | (back_blue);
	return result;
}

/** Conversion from hue saturation light to RGB color
@param hue hue value [0-359]
@param saturation saturation [0-100]
@param light light [0-100]
@return rgb color */
uint32_t Hsl::to_rgb(uint32_t hue, uint32_t saturation, uint32_t light)
{
	int32_t v;
	int32_t m, sv, fract, vsf, middle1, middle2, sext;
	uint32_t red;
	uint32_t green;
	uint32_t blue;
	uint32_t res = 0;
	red = green = blue = 0;

	saturation = mapToPercent(saturation);
	light = mapToPercent(light);

	hue %= 360;
	v = (light < 50) ? (light * (saturation + 100) / 100) : (light + saturation - (light * saturation / 100));
	if (v > 0) 
	{
		m = light + light - v;
		sv = 100 * (v - m) / v;

		sext = hue/60;
		fract = 100 * (hue - (sext * 60)) / 60;
		vsf = v * sv * fract / 10000;
		middle1 = m + vsf;
		middle2 = v - vsf;

		switch (sext) 
		{
		case 0: 
			red = v;
			green = middle1;
			blue = m;
			break;

		case 1:
			red = middle2;
			green = v;
			blue = m;
			break;

		case 2:
			red = m;
			green = v;
			blue = middle1;
			break;

		case 3:
			red = m;
			green = middle2;
			blue = v;
			break;

		case 4:
			red = middle1;
			green = m;
			blue = v;
			break;

		case 5:
			red = v;
			green = m;
			blue = middle2;
			break;
		}

		red   = (red   * 255) / 100;
		green = (green * 255) / 100;
		blue  = (blue  * 255) / 100;
	}
	res = 0xFF000000;
	res |= red   << 16;
	res |= green << 8;
	res |= blue;
	return res;
}

/** Conversion from RGB to hue saturation light 
@param color color to convert
@param hue value returned [0-359]
@param saturation value returned [0-100]
@param light value returned [0-100] */
void Hsl::from_rgb(uint32_t color, uint32_t & hue, uint32_t & saturation, uint32_t &light)
{
	uint32_t v, m, vm;
	uint32_t hue2, saturation2, light2;
	uint32_t red;
	uint32_t green;
	uint32_t blue;

	{
		red   = ((color >> 16) & 0xFF) << 8;
		green = ((color >> 8) & 0xFF) << 8;
		blue  = (color & 0xFF) << 8;

		hue2 = saturation2 = light2 = 0;

		v = max(red,green);
		v = max(v,blue);
		m = min(red,green);
		m = min(m,blue);

		light2 = (m + v)/2;
		if (light2 != 0) 
		{
			saturation2 = vm = (v - m);
			if (saturation2 != 0)
			{
				uint32_t red2, green2, blue2;
				saturation2 = (saturation2 << 16) / ((light2 < 32768) ? (v+m) : (131072 - v - m));

				red2   = ((((v-red)   << 16) / vm) * 60) >> 16;
				green2 = ((((v-green) << 16) / vm) * 60) >> 16;
				blue2  = ((((v-blue)  << 16) / vm) * 60) >> 16;

				if (red == v)
				{
					hue2 = (green == m ? 300 + blue2 : 60 - green2);
				}
				else if (green == v)
				{
					hue2 = (blue == m ? 60 + red2 : 180 - blue2);
				}
				else
				{
					hue2 = (red == m ? 180 + green2 : 300 - red2);
				}
			}
		}

		hue = hue2 % 360;
		saturation = (saturation2 * 100) >> 16;
		light = (light2 * 100) >> 16;
	}
}

/** Enlighten the color */
uint32_t Hsl::enlighten(uint32_t color, int32_t level)
{
	uint32_t hue;
	uint32_t saturation;
	uint32_t light;
	Hsl::from_rgb(color, hue, saturation, light);
	level = mapToRange(level);
	return Hsl::to_rgb(hue, saturation, mapToPercent(light + ((100-light) * level) / 100));
}

/** Adjust the saturation of selected color */
uint32_t Hsl::saturate(uint32_t color, int32_t level)
{
	uint32_t hue;
	uint32_t saturation;
	uint32_t light;
	Hsl::from_rgb(color, hue, saturation, light);
	level = mapToRange(level);
	return Hsl::to_rgb(hue, mapToPercent(saturation - ((100-saturation) * level) / 100), light);
}

/** Change the color into pastel */
uint32_t Hsl::pastel(uint32_t color, int32_t level)
{
	uint32_t hue;
	uint32_t saturation;
	uint32_t light;
	Hsl::from_rgb(color, hue, saturation, light);
	level = mapToRange(level);
	return Hsl::to_rgb(hue, 
		mapToPercent(saturation - ((100-saturation) * level) / 100), 
		mapToPercent(light      + ((100-light)      * level) / 100));
}

/** Convert color to gray
@param color
@return gray color */
uint32_t Hsl::to_gray(uint32_t color)
{
	uint32_t red;
	uint32_t green;
	uint32_t blue;
	uint32_t alpha;
	uint32_t gray;
	uint32_t result = 0;

	alpha = ((color >> 24) & 0xFF);
	red   = ((color >> 16) & 0xFF);
	green = ((color >>  8) & 0xFF);
	blue  = (color & 0xFF);
	
	red   = (red   * 306) >> 10;
	green = (green * 601) >> 10;
	blue  = (blue  * 117) >> 10;

	gray  = red + green + blue;
	if (gray > 255)
	{
		gray = 255;
	}

	result = (alpha << 24) | (gray << 16) | (gray << 8) | (gray);
	return result;
}


uint32_t channel_luminance(uint32_t channel)
{
	Linear line;
	if      (channel <  32) line.set_coef(  0,       0,  31,     137);
	else if (channel <  64) line.set_coef( 32,     144,  63,     497);
	else if (channel <  96) line.set_coef( 64,     512,  95,    1144);
	else if (channel < 128) line.set_coef( 96,    1169, 127,    2122);
	else if (channel < 160) line.set_coef(128,    2158, 159,    3467);
	else if (channel < 192) line.set_coef(160,    3515, 191,    5209);
	else if (channel < 224) line.set_coef(192,    5271, 223,    7379);
	else                    line.set_coef(224,    7454, 255,   10000);
	uint32_t result = line.fx(channel);
	return result;
}



// Calculate the luminance of a color
uint32_t Hsl::luminance(uint32_t color)
{
	static const uint32_t weight_r = 2126; // 0.2126 * 10000
	static const uint32_t weight_g = 7152; // 0.7152 * 10000
	static const uint32_t weight_b = 722 ; // 0.0722 * 10000

	uint32_t red, green, blue;

	red   = ((color >> 16) & 0xFF);
	green = ((color >> 8) & 0xFF);
	blue  = ((color & 0xFF));

	uint32_t result;

	red   = channel_luminance(((color >> 16) & 0xFF));
	green = channel_luminance(((color >> 8) & 0xFF));
	blue  = channel_luminance((color & 0xFF));

	result = (red * weight_r + green * weight_g + blue * weight_b)/10000;
	return result;
}

// Indicates if the constrast is readable 
uint32_t Hsl::delta_contrast(uint32_t color1, uint32_t color2)
{
	uint32_t luminance1 = luminance(color1);
	uint32_t luminance2 = luminance(color2);

	uint32_t v1 = (max(luminance1, luminance2) + 500) * 10000;
	uint32_t v2 =  min(luminance1, luminance2) + 500;

	uint32_t result = v1/v2;
	return result;
}

// Convert a HS type color with a determined gray level to achieve, to have homogeneity in the colors
uint32_t Hsl::to_balanced_color(uint32_t hue, uint32_t saturation, uint32_t gray_level)
{
	int32_t min = 0;
	int32_t max = 100;
	uint32_t result = 0;
	uint32_t color;

	uint32_t delta_gray = 0;
	if (hue >= 10 && hue <= 40)
	{
		Linear line(10,0,40,40);
		delta_gray = line.fx(hue);
	}
	else if (hue >= 60 && hue <= 90) 
	{
		Linear line(60,40,90,0);
		delta_gray = line.fx(hue);
	}
	else if (hue > 40 && hue < 60) 
	{
		delta_gray = 40;
	}

	gray_level += delta_gray;

	if (gray_level > 255)
	{
		gray_level = 255;
	}

	do
	{
		uint32_t delta = (max + min)>>1;
		color = Hsl::to_rgb(hue, saturation, delta);
		uint32_t gray = Hsl::to_gray(color) & 0xFF;

		if (gray < gray_level)
		{
			min = delta;
		}
		else if (gray > gray_level)
		{
			max = delta;
		}
		else
		{
			result = color;
			break;
		}
	}
	while(abs(min - max) > 4);
	
	if (result == 0)
	{
		result = color;
	}
	return result;
}



uint32_t Hsl::harmonious_colors(uint32_t hue, uint32_t current, uint32_t n_colors, uint32_t saturation, uint32_t light_min, uint32_t light_max)
{
	if (current < 1)
	{
		current = 1;
	}
	if (current > n_colors)
	{
		current = n_colors;
	}
	// Generate evenly spaced brightness levels
	uint32_t luminosities = min(light_min, light_max) + current * (max(light_max,light_min) - min(light_min,light_max)) / (n_colors - 1);

	// Convert HSV -> RGB for each color
	return to_rgb(hue, saturation, luminosities);
}