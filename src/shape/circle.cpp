#include "berialdraw_imp.hpp"

using namespace berialdraw;

const char filled_circle[] = 
"d=r*4521>13;"
".x,y+r;"
	"$x+d,y+r;"
	"$x+r,y+d;"
".x+r,y;"
	"$x+r,y-d;"
	"$x+d,y-r;"
".x,y-r;"
	"$x-d,y-r;"
	"$x-r,y-d;"
".x-r,y;"
	"$x-r,y+d;"
	"$x-d,y+r;"
"~;"
;

const char un_filled_circle[] = 
"a=r;"
"R=t>1;"
"R=R+a;"
"r=t>1;"
"r=a-r;"
"D=R*4521>13;"
"d=r*4521>13;"

".x,y+R;"
	"$x+D,y+R;"
	"$x+R,y+D;"
".x+R,y;"
	"$x+R,y-D;"
	"$x+D,y-R;"
".x,y-R;"
	"$x-D,y-R;"
	"$x-R,y-D;"
".x-R,y;"
	"$x-R,y+D;"
	"$x-D,y+R;"
"~;"

".x,y+r;"
	"$x-d,y+r;"
	"$x-r,y+d;"
".x-r,y;"
	"$x-r,y-d;"
	"$x-d,y-r;"
".x,y-r;"
	"$x+d,y-r;"
	"$x+r,y-d;"
".x+r,y;"
	"$x+r,y+d;"
	"$x+d,y+r;"
"~;";

Circle::Circle(Canvas * canvas) :
	Marker(canvas)
{
}

// Render outline
void Circle::paint(const Point & shift)
{
	Marker::paint(shift, filled_circle, un_filled_circle);
}

