#include "berialdraw_imp.hpp"

using namespace berialdraw;

ImageDecoder::~ImageDecoder()
{
}

ImageDecoder* ImageDecoder::create(const char* filename)
{
	ImageDecoder* result = nullptr;

	if (filename)
	{
		String name(filename);
		int32_t dot_pos = -1;

		// Find last dot in filename
		for (int32_t i = (int32_t)name.size() - 1; i >= 0; i--)
		{
			if (name[i] == '.')
			{
				dot_pos = i;
				break;
			}
		}

		if (dot_pos >= 0)
		{
			String ext;
			for (int32_t i = dot_pos + 1; i < (int32_t)name.size(); i++)
			{
				char c = name[i];
				// Convert to lowercase
				if (c >= 'A' && c <= 'Z')
				{
					c = c + ('a' - 'A');
				}
				ext += c;
			}

			if (ext == "png")
			{
				result = new PngDecoder();
			}
			else if (ext == "jpg" || ext == "jpeg")
			{
				result = new JpegDecoder();
			}
		}
	}

	return result;
}
