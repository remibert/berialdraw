#include "berialdraw_imp.hpp"

using namespace berialdraw;

const char filled_square[] = 
"D=r*5793>13;"
"D=t>1+D;"
".x+D,y+D;"
".x+D,y-D;"
".x-D,y-D;"
".x-D,y+D;"
"~;";

const char un_filled_square[] = 
"D=r*5793>13;"
"D=t>1+D;"
".x+D,y+D;"
".x+D,y-D;"
".x-D,y-D;"
".x-D,y+D;"
"~;"
"d=r*5793>13;"
"d=-t>1+d;"
".x+d,y+d;"
".x-d,y+d;"
".x-d,y-d;"
".x+d,y-d;"
"~;";

Square::Square(Canvas * canvas):
	Marker(canvas)
{
}

// Render outline
void Square::paint(const Point & shift)
{
	Marker::paint(shift, filled_square, un_filled_square);
}

