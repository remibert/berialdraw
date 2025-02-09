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

#ifdef _DEBUG
void Triangle::test()
{
	static bool done = false;
	if (done == false)
	{
		done = true;
		Window window;
			window.position(0,0);
			window.size(480,480);
			window.color(Color::WHITE);

		Canvas * canvas = new Canvas(&window);
			canvas->position(0,0);
			canvas->size(window.size());
			canvas->color(Color::TRANSPARENT);

		Point center(360, 140);

		{
			Cross * cross = new Cross(canvas);
			cross->color(Color::BLACK);
			cross->thickness(3);
			cross->position(center);
			cross->radius(10);
		}
		{
			Triangle * triangle = new Triangle(canvas);

			triangle->radius(100);
			triangle->position(center);
			triangle->thickness(40);
			triangle->color(Color::RED,64);
		}

		{
			Triangle * filltriangle = new Triangle(canvas);
			filltriangle->position(center);
			filltriangle->radius(100);
			filltriangle->color(Color::GREEN,64);
			filltriangle->thickness(0);
		}

		UIManager::desktop()->dispatch("test/out/triangle.svg");
	}
}
#endif
