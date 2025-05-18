#include "berialdraw_imp.hpp"

using namespace berialdraw;

ArcCache::ArcCache()
{
}

ArcCache::~ArcCache()
{
}

void ArcCache::compute(Coord radius, Dim thickness, Dim & width, Coord & vec_x, Coord & vec_y, Coord & handle, Coord & intersection)
{
	bool found = false;

	// Search in cache the previous intersection
	for (int i = 0; i < CACHE_SIZE; ++i) 
	{
		if (m_items[i].radius != ~0)
		{
			if (m_items[i].thickness == thickness && m_items[i].radius == radius) 
			{
				vec_x        = m_items[i].vec_x;
				vec_y        = m_items[i].vec_y;
				handle       = m_items[i].handle;
				intersection = m_items[i].intersection;
				width        = m_items[i].width;
				found = true;
				break;
			}
		}
	}

	// If value in cache not found
	if (found == false)
	{
		FT_Angle   angle;
		FT_Vector  sincosvec;
		FT_Vector  vec;

		// It the angle is not 90 degres
		if ((Coord)thickness < radius)
		{
			thickness <<=  2;
			radius    <<=  2;

			intersection = FT_SqrtFixed(FT_MulFix(2 * radius, thickness) - FT_MulFix(thickness, thickness)); // Circle intersection with line
			angle        = FT_Atan2((radius-thickness),intersection);                        // Angle of the intersection point
			handle       = FT_MulDiv(FT_Tan(angle/4),radius,3) >> 14;                        // Length of bezier control handle

			handle       >>= 2;
			thickness    >>= 2;
			radius       >>= 2;
			intersection >>= 2;

			FT_Fixed s = FT_Tan(angle/4);

			// Get the sinus/cos of the second bezier control handle
			FT_Vector_Unit(&sincosvec, FT_ANGLE_PI2-angle);
			FT_Matrix matrix;
				matrix.yy = sincosvec.y;
				matrix.xx = sincosvec.y;
				matrix.yx = sincosvec.x;
				matrix.xy = sincosvec.x;

				vec.x = handle;
				vec.y = 0;

			// Rotate the second bezier control handle according to handle
			FT_Vector_Transform(&vec, &matrix);
			width        = thickness;
		}
		// The angle is 90 degres
		else
		{
			if (radius == 0)
			{
				vec.x        = 0;
				vec.y        = 0;
				handle       = 0;
				intersection = 0;
				width        = 0;
			}
			else
			{
				handle       = FT_MulDiv(27146,radius,3) >> 14;                // Length of bezier control handle
				vec.x        = 0;
				vec.y        = handle;
				intersection = radius;
				width        = radius;
			}
		}

		vec_x = vec.x;
		vec_y = vec.y;
		
		// Add new value in cache
		m_items[m_index].thickness    = thickness;
		m_items[m_index].radius       = radius;
		m_items[m_index].vec_x        = vec_x;
		m_items[m_index].vec_y        = vec_y;
		m_items[m_index].handle       = handle;
		m_items[m_index].intersection = intersection;
		m_items[m_index].width        = width;

		m_index = (m_index + 1) % CACHE_SIZE;
	}
}
