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

#ifdef _DEBUG
void Circle::test()
{
	Window window;
		window.position(0,0);
		window.size(480,480);
		window.color(Color::WHITE);

	Canvas * canvas = new Canvas(&window);
		canvas->position(0,0);
		canvas->size(window.size());
		canvas->color(Color::TRANSPARENT);

	Point center(120, 120);

	{
		Cross * cross = new Cross(canvas);
		cross->color(Color::BLACK);
		cross->thickness(3);
		cross->position(center);
		cross->radius(10);
	}
	{
		Circle * circle = new Circle(canvas);

		circle->radius(100);
		circle->position(center);
		circle->thickness(40);
		circle->color(Color::RED,64);
	}

	{
		Circle * fillcircle = new Circle(canvas);
		fillcircle->position(center);
		fillcircle->radius(100);
		fillcircle->color(Color::GREEN,64);
		fillcircle->thickness(0);
	}

	UIManager::desktop()->dispatch("test/out/circle.svg");
}
#endif
