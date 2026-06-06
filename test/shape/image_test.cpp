#include "berialdraw_imp.hpp"
using namespace berialdraw;

// Test Image shape with different sizes (similar to sketch test1)
void Image::test1()
{
	Window window;
		window.color(Color::BLUE, 64);
		window.position(7, 5);

	Canvas * canvas = new Canvas(&window);
		canvas->color(Color::RED, 64);
		canvas->position(13, 29);

	Rect * rect = new Rect(canvas);
		rect->position(57, 31);
		rect->size(10, 360);
		rect->color(Color::WHITE, 64);

	Image * house = new Image(canvas);
		house->position(rect->position());
		house->size(rect->size());
		house->filename("$(ui.images)/house.png");

	Image * girl = new Image(canvas);
		girl->position(rect->position());
		girl->size(rect->size());
		girl->filename("$(ui.images)/filleperle.jpg");
		girl->alpha(180);

	UIManager::desktop()->dispatch("$(ui.tests)/out/image1_1.svg");
	rect->size(360, 10);
	girl->size(rect->size());
	house->size(rect->size());
	UIManager::desktop()->dispatch("$(ui.tests)/out/image1_2.svg");
	rect->size(360, 360);
	girl->size(rect->size());
	house->size(rect->size());
	UIManager::desktop()->dispatch("$(ui.tests)/out/image1_3.svg");
	rect->size(60, 360);
	girl->size(rect->size());
	house->size(rect->size());
	UIManager::desktop()->dispatch("$(ui.tests)/out/image1_4.svg");
	rect->size(360, 60);
	girl->size(rect->size());
	house->size(rect->size());
	UIManager::desktop()->dispatch("$(ui.tests)/out/image1_5.svg");
}

// Test Image shape with repeat and rotation (similar to sketch test2)
void Image::test2()
{
	Window window;

	Canvas * canvas = new Canvas(&window);
		canvas->color(Color::GREEN, 64);
		canvas->size(480, 480);

	Rect * rect = new Rect(canvas);
		rect->position(240, 240);
		rect->size(100, 100);
		rect->color(Color::RED, 64);
		rect->repeat(Shape::REPEAT_ANGLE, 0, 359, 30);

	Image * house = new Image(canvas);
		house->filename("$(ui.images)/house.png");
		house->size(rect->size());
		house->position(rect->position());
		house->repeat(Shape::REPEAT_ANGLE, 0, 359, 30);

	Image * girl = new Image(canvas);
		girl->position(rect->position());
		girl->size(rect->size());
		girl->filename("$(ui.images)/filleperle.jpg");
		girl->repeat(Shape::REPEAT_ANGLE, 0, 359, 30);

	Coord pos = 0;
	house->center(pos, 50);
	rect->center(pos, 50);
	girl->center(pos, 50);
	girl->alpha(128);
	UIManager::desktop()->dispatch("$(ui.tests)/out/image2_1.svg");

	pos -= 100;
	house->center(pos, 50);
	rect->center(pos, 50);
	girl->center(pos, 50);
	girl->alpha(180);
	UIManager::desktop()->dispatch("$(ui.tests)/out/image2_2.svg");

	pos -= 50;
	house->center(pos, 100);
	rect->center(pos, 100);
	girl->center(pos, 100);
	girl->alpha(200);
	UIManager::desktop()->dispatch("$(ui.tests)/out/image2_3.svg");
}

// Test Image shape with alignment (similar to sketch test3)
void Image::test3()
{
	Window window;

	Canvas * canvas = new Canvas(&window);
		canvas->color(Color::RED, 64);
		canvas->extend(Extend::EXTEND_ALL);

	Rect * rect = new Rect(canvas);
		rect->position(57, 31);
		rect->size(32, 360);
		rect->color(Color::WHITE, 64);

	Image * house = new Image(canvas);
		house->position(rect->position());
		house->size(rect->size());
		house->filename("$(ui.images)/house.png");
	UIManager::desktop()->dispatch("$(ui.tests)/out/image3_0.svg");

	house->align(Align::ALIGN_BOTTOM);
	UIManager::desktop()->dispatch("$(ui.tests)/out/image3_1.svg");

	house->align(Align::ALIGN_TOP);
	UIManager::desktop()->dispatch("$(ui.tests)/out/image3_2.svg");

	house->align(Align::ALIGN_VERTICAL);
	UIManager::desktop()->dispatch("$(ui.tests)/out/image3_3.svg");

	rect->size(360, 16);
	house->size(rect->size());
	house->align(Align::ALIGN_LEFT);
	UIManager::desktop()->dispatch("$(ui.tests)/out/image3_6.svg");

	house->align(Align::ALIGN_RIGHT);
	UIManager::desktop()->dispatch("$(ui.tests)/out/image3_4.svg");

	house->align(Align::ALIGN_HORIZONTAL);
	UIManager::desktop()->dispatch("$(ui.tests)/out/image3_5.svg");
}

// Test Image rotation on canvas
void Image::test4()
{
	Window window;
		window.position(0, 0);
		window.size(480, 480);
		window.color(Color::WHITE);

	Canvas * canvas = new Canvas(&window);
		canvas->position(0, 0);
		canvas->size(window.size());

	// Debug rectangle - same size/position/center/angle as image
	Rect * debug_rect = new Rect(canvas);
		debug_rect->position(240, 240);
		debug_rect->size(160, 240);
		debug_rect->center(80, 120);
		debug_rect->color(Color::BLUE, 64);  // Semi-transparent blue

	Image * img = new Image(canvas);
		img->filename("$(ui.images)/house.png");
		img->position(240, 240);
		img->size(160, 240);
		img->center(80, 120);

	int id = 0;
	for (int angle = 0; angle <= 360; angle += 45)
	{
		debug_rect->angle(angle);
		img->angle(angle);
		String name;
		name.print("$(ui.tests)/out/image4_%d.svg", ++id);
		UIManager::desktop()->dispatch(name);
	}
}

// Test Image and fit modes
void Image::test5()
{
	Window window;
		window.position(0, 0);
		window.size(480, 480);
		window.color(Color::WHITE);

	Canvas * canvas = new Canvas(&window);
		canvas->position(0, 0);
		canvas->size(window.size());

	// FIT mode (default)
	Image * img1 = new Image(canvas);
		img1->filename("$(ui.images)/house.png");
		img1->position(20, 20);
		img1->size(200, 200);
		img1->fit_mode(FIT);

	// STRETCH mode
	Image * img2 = new Image(canvas);
		img2->filename("$(ui.images)/filleperle.jpg");
		img2->position(240, 20);
		img2->size(200, 200);
		img2->fit_mode(STRETCH);

	// FIT with alpha
	Image * img3 = new Image(canvas);
		img3->filename("$(ui.images)/house.png");
		img3->position(20, 240);
		img3->size(200, 200);
		img3->fit_mode(FIT);
		img3->alpha(128);

	// STRETCH with alpha
	Image * img4 = new Image(canvas);
		img4->filename("$(ui.images)/filleperle.jpg");
		img4->position(240, 240);
		img4->size(200, 200);
		img4->fit_mode(STRETCH);
		img4->alpha(180);

	UIManager::desktop()->dispatch("$(ui.tests)/out/image5.svg");
}

// Test mixed Image shapes on same canvas
void Image::test6()
{
	Window window;
		window.position(0, 0);
		window.size(480, 480);
		window.color(Color::LIGHT_BLUE);

	Canvas * canvas = new Canvas(&window);
		canvas->position(0, 0);
		canvas->size(window.size());

	// PNG images
	Image * img1 = new Image(canvas);
		img1->filename("$(ui.images)/house.png");
		img1->position(10, 10);
		img1->size(150, 150);

	Image * img2 = new Image(canvas);
		img2->filename("$(ui.images)/house.png");
		img2->position(170, 10);
		img2->size(150, 150);
		img2->alpha(180);

	// JPEG images
	Image * img3 = new Image(canvas);
		img3->filename("$(ui.images)/filleperle.jpg");
		img3->position(330, 10);
		img3->size(140, 140);

	Image * img4 = new Image(canvas);
		img4->filename("$(ui.images)/filleperle.jpg");
		img4->position(10, 170);
		img4->size(150, 150);
		img4->alpha(200);

	Image * img5 = new Image(canvas);
		img5->filename("$(ui.images)/house.png");
		img5->position(170, 170);
		img5->size(150, 150);
		img5->fit_mode(STRETCH);

	Image * img6 = new Image(canvas);
		img6->filename("$(ui.images)/filleperle.jpg");
		img6->position(330, 170);
		img6->size(140, 140);
		img6->fit_mode(STRETCH);
		img6->alpha(128);

	// Bottom row with overlapping
	Image * img7 = new Image(canvas);
		img7->filename("$(ui.images)/house.png");
		img7->position(50, 340);
		img7->size(200, 130);
		img7->alpha(200);

	Image * img8 = new Image(canvas);
		img8->filename("$(ui.images)/filleperle.jpg");
		img8->position(200, 340);
		img8->size(130, 130);

	Image * img9 = new Image(canvas);
		img9->filename("$(ui.images)/house.png");
		img9->position(280, 340);
		img9->size(180, 130);
		img9->alpha(180);

	UIManager::desktop()->dispatch("$(ui.tests)/out/image6.svg");
}

void Image::test()
{
	static bool done = false;
	if (done == false)
	{
		MemoryLeakLog
		done = true;
		test6();
		test5();
		test4();
		test3();
		test2();
		test1();
	}
}
