#include "berialdraw_imp.hpp"

using namespace berialdraw;

const char filled_triangle[] = 
"D=r*7094>13;"
"d=r>1;"
".x+D,y+d;"
".x-D,y+d;"
".x,y-r;"
"~;";

const char un_filled_triangle[] = 
"D=r*7094>13;"
"d=r>1;"
"u=t>1;"
"U=t*7094>13;"
".x+D+U,y+d+u;"
".x-D-U,y+d+u;"
".x,y-r-t;"
"~;"
".x+D-U,y+d-u;"
".x,y-r+t;"
".x-D+U,y+d-u;"
"~;";

Triangle::Triangle(Canvas * canvas) :
	Marker(canvas)
{
}

// Render outline
void Triangle::paint(const Point & shift)
{
	Marker::paint(shift, filled_triangle, un_filled_triangle);
}

