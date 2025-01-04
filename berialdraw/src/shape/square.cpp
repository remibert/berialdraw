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

#ifdef _DEBUG
void Square::test()
{
	Window window;
		window.position(0,0);
		window.size(480,480);
		window.color(Color::WHITE);

	Canvas * canvas = new Canvas(&window);
		canvas->position(0,0);
		canvas->size(window.size());
		canvas->color(Color::TRANSPARENT);

	Point center(360, 360);

	{
		Cross * cross = new Cross(canvas);
		cross->color(Color::BLACK);
		cross->thickness(3);
		cross->position(center);
		cross->radius(10);
	}
	{
		Square * square = new Square(canvas);

		square->radius(100);
		square->position(center);
		square->thickness(40);
		square->color(Color::RED,64);
	}

	{
		Square * fillsquare = new Square(canvas);
		fillsquare->position(center);
		fillsquare->radius(100);
		fillsquare->color(Color::GREEN,64);
		fillsquare->thickness(0);
	}

	UIManager::desktop()->dispatch("test/out/square.svg");
}
#endif
