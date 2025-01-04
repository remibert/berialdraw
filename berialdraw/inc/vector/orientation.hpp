#ifndef Orientation_hpp_INCLUDED
#define Orientation_hpp_INCLUDED

namespace berialdraw
{
	enum Orientation
	{
		ORIENTATION_NORMAL   = 0, // Rotate 0°
		ORIENTATION_LEFT     = 1, // Rotate 90° to the left
		ORIENTATION_RIGHT    = 2, // Rotate 90° to the right
		ORIENTATION_RETURNED = 3  // Rotate 180°
	};
}
#endif