#pragma once
namespace berialdraw
{
	/** Image fit mode enumeration. Defines how the image adapts to the widget area. */
	enum ImageFitMode
	{
		IMAGE_FIT_STRETCH          = 0, /**< Stretch the image to fill the entire area (ignores aspect ratio) */
		IMAGE_FIT_ASPECT_WIDTH     = 1, /**< Preserve aspect ratio, fit to area width */
		IMAGE_FIT_ASPECT_HEIGHT    = 2, /**< Preserve aspect ratio, fit to area height */
		IMAGE_FIT_ASPECT_FIT       = 3, /**< Preserve aspect ratio, fit entirely inside area (may leave borders) */
		IMAGE_FIT_ASPECT_FILL      = 4, /**< Preserve aspect ratio, fill entire area (may crop) */
		IMAGE_FIT_NONE             = 5, /**< Display at native size, no scaling */
	};
}
