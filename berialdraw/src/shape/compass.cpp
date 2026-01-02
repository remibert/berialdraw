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

	result.x_(m_center.x_() + vec.x);
	result.y_(m_center.y_() + vec.y);
	return result;
}


#ifdef _DEBUG
void Compass::test()
{
	static bool done = false;
	if (done == false)
	{
		done = true;
		Window window;
			window.color(Color::WHITE);
			window.size(480,480);

		Canvas * canvas = new Canvas(&window);
			canvas->size(window.size());
			canvas->color(Color::LAVENDER);

		Point center(240,240);

		Cross * cross = new Cross(canvas);
			cross->position(center);

		Circle * circle = new Circle(canvas);
			circle->position(center);
			circle->radius(220);

		Compass compass;
			compass.center(center);
			compass.radius(200);


		for(int32_t angle=0; angle < 360; angle+=15)
		{
			Text * text = new Text(canvas);
				text->font_size(20);
			compass.angle(angle);
			text->text("%d",angle);
			text->center(text->content_size().middle());
			text->position(compass.position());
		}
		UIManager::desktop()->dispatch("${ui.tests}/out/compass1.svg");
	}
}
#endif
