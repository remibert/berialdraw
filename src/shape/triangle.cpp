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
"R=r;"
"D=R*7094>13;"
"d=R>1;"
".x+D,y+d;"
".x-D,y+d;"
".x,y-R;"
"~;"
"b=t+t;"
"a=R-b;"
"D=a*7094>13;"
"d=a>1;"
".x+D,y+d;"
".x,y-a;"
".x-D,y+d;"
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

