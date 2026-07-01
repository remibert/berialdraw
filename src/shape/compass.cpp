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

	result.x_q6(m_center.x_q6() + vec.x);
	result.y_q6(m_center.y_q6() + vec.y);
	return result;
}



