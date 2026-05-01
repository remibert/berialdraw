#include "berialdraw_imp.hpp"

using namespace berialdraw;

const char un_filled_cross[] = 
"d=t>1;"
"r=d+r;"
"d=d*5793>13;"
"D=r*5793>13;"
	".x+D+d,y+D-d;"
	".x+D-d,y+D+d;"
	".x-D-d,y-D+d;"
	".x-D+d,y-D-d;"
"~;"
	".x-D+d,y+D+d;"
	".x-D-d,y+D-d;"
	".x+D-d,y-D-d;"
	".x+D+d,y-D+d;"
"~;";

Cross::Cross(Canvas * canvas):
	Marker(canvas)
{
}

// Render outline
void Cross::paint(const Point & shift)
{
	Marker::paint(shift, un_filled_cross, un_filled_cross);
}

