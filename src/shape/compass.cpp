#include "berialdraw_imp.hpp"

using namespace berialdraw;

Compass::Compass()
{
}

Point Compass::position()
{
	Point result;
	FT_Matrix matrix = vector_matrix((m_angle << 10));

	FT_Vector vec;
	vec.x = m_radius;
	vec.y = 0;

	FT_Vector_Transform(&vec, &matrix);

	result.x_(m_center.x_() + vec.x);
	result.y_(m_center.y_() + vec.y);
	return result;
}


