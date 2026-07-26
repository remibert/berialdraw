#include "berialdraw_imp.hpp"

using namespace berialdraw;

// Destructor
ImageDecoder::~ImageDecoder()
{
}

// Factory: create decoder based on file extension
ImageDecoder* ImageDecoder::create(const char* filename)
{
	ImageDecoder* result = nullptr;

	if (filename)
	{
		// Instantiate appropriate decoder based on file extension
		if (FileTools::match_pattern("*.png", filename, true))
		{
			result = new PngDecoder();
		}
		else if (FileTools::match_pattern("*.jpg", filename, true) || FileTools::match_pattern("*.jpeg", filename, true))
		{
			result = new JpegDecoder();
		}
	}

	return result;
}
