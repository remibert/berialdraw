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

#ifdef _DEBUG
void Cross::test()
{
	Window window;
		window.position(0,0);
		window.size(480,480);
		window.color(Color::WHITE);

	Canvas * canvas = new Canvas(&window);
		canvas->position(0,0);
		canvas->size(window.size());
		canvas->color(Color::TRANSPARENT);

	Point center(120, 360);

	{
		Cross * cross = new Cross(canvas);
		cross->color(Color::BLACK);
		cross->thickness(3);
		cross->position(center);
		cross->radius(10);
	}
	{
		Cross * cross = new Cross(canvas);

		cross->radius(100);
		cross->position(center);
		cross->thickness(40);
		cross->color(Color::RED,64);
	}
	{
		Cross * fillcross = new Cross(canvas);
		fillcross->position(center);
		fillcross->radius(100);
		fillcross->color(Color::GREEN,64);
		fillcross->thickness(0);
	}

	UIManager::desktop()->dispatch("test/out/cross.svg");
}
#endif
