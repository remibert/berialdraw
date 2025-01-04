#include "berialdraw_imp.hpp"

using namespace berialdraw;

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

	if (saturation > 100)
	{
		saturation = 100;
	}

	if (light > 100)
	{
		light = 100;
	}

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
uint32_t Hsl::adjust_light(uint32_t color, int32_t light_level)
{
	uint32_t hue;
	uint32_t saturation;
	uint32_t light;
	Hsl::from_rgb(color, hue, saturation, light);

	int32_t new_light = ((int32_t)light) + light_level;

	if (new_light < 0)
	{
		new_light = 0;
	}
	if (new_light > 100)
	{
		new_light = 100;
	}
	return Hsl::to_rgb(hue, saturation, (uint32_t)new_light);
}


/** Adjust the saturation of selected color
@param color color to convert
@param saturation value added [-100..100]
@return rgb color */
uint32_t Hsl::adjust_saturation(uint32_t color, int32_t saturation_level)
{
	uint32_t hue;
	uint32_t saturation;
	uint32_t light;
	Hsl::from_rgb(color, hue, saturation, light);

	int32_t new_saturation = ((int32_t)saturation) + saturation_level;

	if (new_saturation < 0)
	{
		new_saturation = 0;
	}
	if (new_saturation > 100)
	{
		new_saturation = 100;
	}
	return Hsl::to_rgb(hue, (uint32_t)new_saturation, light);
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