#include "berialdraw_imp.hpp"

using namespace berialdraw;

Framebuf * Framebuf::create(uint32_t width, uint32_t height, enum Type type) 
{
	if (type == Framebuf::ARGB8888)
	{
		return new Argb8888(width, height);
	}
	return 0;
}
