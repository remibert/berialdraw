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

// Test PNG alpha preservation and JPEG matte compositing
void Image::test7()
{
	Argb8888 source(3, 1);
	source.clear(0x00000000);
	source.pixel(1, 0, 0xFFFF0000);
	source.pixel(2, 0, 0x800000FF);

	String png_path = File::resolve_path("$(ui.tests)/out/image7_1.png");
	String jpeg_path = File::resolve_path("$(ui.tests)/out/image7_2.jpg");
	File::Location png_location;
	assert(File::parse_location(png_path.c_str(), png_location));
	assert(PngEncoder::supports(png_location));
	assert(!JpegEncoder::supports(png_location));
	File::QueryParameter compression_parameter;
	compression_parameter.name = "compression";
	compression_parameter.value = "6";
	png_location.query_parameters.push_back(compression_parameter);
	assert(PngEncoder::encode(source, png_location));
	File::Location jpeg_location;
	assert(File::parse_location(jpeg_path.c_str(), jpeg_location));
	assert(JpegEncoder::supports(jpeg_location));
	assert(!PngEncoder::supports(jpeg_location));
	File::QueryParameter quality_parameter;
	quality_parameter.name = "quality";
	quality_parameter.value = "80";
	jpeg_location.query_parameters.push_back(quality_parameter);
	File::QueryParameter background_parameter;
	background_parameter.name = "background";
	background_parameter.value = "0xFF00FF00";
	jpeg_location.query_parameters.push_back(background_parameter);
	assert(JpegEncoder::encode(source, jpeg_location));

	PngDecoder png_decoder;
	assert(png_decoder.decode(png_path.c_str()));
	assert(png_decoder.has_alpha());
	assert(png_decoder.size().width() == 3);
	assert(png_decoder.size().height() == 1);
	assert(png_decoder.pixel_data()[0] == 0x00000000);
	assert(png_decoder.pixel_data()[1] == 0xFFFF0000);
	assert(png_decoder.pixel_data()[2] == 0x800000FF);

	JpegDecoder jpeg_decoder;
	assert(jpeg_decoder.decode(jpeg_path.c_str()));
	assert(!jpeg_decoder.has_alpha());
	assert(jpeg_decoder.size().width() == 3);
	assert(jpeg_decoder.size().height() == 1);
	for (uint32_t x = 0; x < jpeg_decoder.size().width(); x++)
	{
		assert((jpeg_decoder.pixel_data()[x] >> 24) == 255);
	}
}

// Test Desktop snapshot format routing and URL-based encoder options
void Image::test8()
{
	Window window;
		window.position(12, 13);
		window.size(40, 30);
		window.color(0x800000FF);

	String png_path = File::resolve_path("$(ui.tests)/out/image8_1.PNG");
	String png_snapshot = png_path + "?compression=0";
	UIManager::desktop()->dispatch(png_snapshot.c_str());

	PngDecoder png_decoder;
	assert(png_decoder.decode(png_path.c_str()));
	assert(png_decoder.has_alpha());
	assert(png_decoder.size().width() > 0);
	assert(png_decoder.size().height() > 0);
	uint32_t png_center = png_decoder.pixel_data()[(png_decoder.size().height() / 2) * png_decoder.size().width() + png_decoder.size().width() / 2];
	assert(png_center == 0x800000FF);

	String jpeg_path = File::resolve_path("$(ui.tests)/out/image8_2.jpg");
	String jpeg_snapshot = jpeg_path + "?quality=100&background=0xFFFF0000";
	UIManager::desktop()->dispatch(jpeg_snapshot.c_str());

	JpegDecoder jpeg_decoder;
	assert(jpeg_decoder.decode(jpeg_path.c_str()));
	assert(!jpeg_decoder.has_alpha());
	assert(jpeg_decoder.size().width() == png_decoder.size().width());
	assert(jpeg_decoder.size().height() == png_decoder.size().height());
	uint32_t jpeg_center = jpeg_decoder.pixel_data()[(jpeg_decoder.size().height() / 2) * jpeg_decoder.size().width() + jpeg_decoder.size().width() / 2];
	assert((jpeg_center >> 24) == 255);
	assert(((jpeg_center >> 16) & 0xFF) >= 100 && ((jpeg_center >> 16) & 0xFF) <= 155);
	assert(((jpeg_center >> 8) & 0xFF) <= 20);
	assert((jpeg_center & 0xFF) >= 100 && (jpeg_center & 0xFF) <= 155);
}

// Export a transparent window showing both the alpha PNG and opaque JPEG image.
void Image::test9()
{
	const Size display_size = UIManager::renderer()->size();
	const uint32_t display_width = display_size.width();
	const uint32_t display_height = display_size.height();
	const uint32_t margin = 12;
	const uint32_t panel_width = (display_width - margin * 3) / 2;
	const uint32_t panel_height = display_height - margin * 2;
	assert(panel_width > 0 && panel_height > 0);

	Window window;
		window.position(0, 0);
		window.size(display_width, display_height);
		window.color(Color::TRANSPARENT);

	Canvas* canvas = new Canvas(&window);
		canvas->position(0, 0);
		canvas->size(display_width, display_height);
		canvas->color(Color::TRANSPARENT);

	Image* house = new Image(canvas);
		house->filename("$(ui.images)/house.png");
		house->position(margin, margin);
		house->size(panel_width, panel_height);
		house->fit_mode(STRETCH);

	Image* girl = new Image(canvas);
		girl->filename("$(ui.images)/filleperle.jpg");
		girl->position(margin * 2 + panel_width, margin);
		girl->size(panel_width, panel_height);
		girl->fit_mode(STRETCH);

	Coord overlay_x = (Coord)(display_width / 2) - 115;
	Coord overlay_y = (Coord)(display_height / 2) - 30;
	Rect* transparent_red = new Rect(canvas);
		transparent_red->position(overlay_x, overlay_y);
		transparent_red->size(230, 60);
		transparent_red->color(0x80FF0000);

	Text* transparent_blue_text = new Text(canvas);
		transparent_blue_text->position(overlay_x, overlay_y + 50);
		transparent_blue_text->font_size(80);
		transparent_blue_text->color(0x800000FF);
		transparent_blue_text->text("BLUE 50%");

	String output = "$(ui.tests)/out/image9_1.png";
	UIManager::desktop()->dispatch(output);

	PngDecoder decoder;
	assert(decoder.decode(output));
	assert(decoder.has_alpha());
	assert(decoder.size().width() > 0 && decoder.size().height() > 0);
	assert(decoder.pixel_data()[0] >> 24 == 0);

	const Dim scale_q6 = UIManager::renderer()->scale_q6();
	const uint32_t house_left = (uint32_t)(((uint64_t)margin * scale_q6) >> 6);
	const uint32_t house_right = (uint32_t)(((uint64_t)(margin + panel_width) * scale_q6) >> 6);
	const uint32_t jpeg_left = (uint32_t)(((uint64_t)(margin * 2 + panel_width) * scale_q6) >> 6);
	const uint32_t jpeg_right = (uint32_t)(((uint64_t)(margin * 2 + panel_width * 2) * scale_q6) >> 6);
	const uint32_t image_top = (uint32_t)(((uint64_t)margin * scale_q6) >> 6);
	const uint32_t image_bottom = (uint32_t)(((uint64_t)(margin + panel_height) * scale_q6) >> 6);

	bool house_transparency_visible = false;
	for (uint32_t y = image_top; y < image_bottom && !house_transparency_visible; y++)
	{
		for (uint32_t x = house_left; x < house_right; x++)
		{
			if ((decoder.pixel_data()[(size_t)y * decoder.size().width() + x] >> 24) < 255)
			{
				house_transparency_visible = true;
			}
		}
	}
	assert(house_transparency_visible);

	bool jpeg_panel_is_opaque = true;
	for (uint32_t y = image_top; y < image_bottom && jpeg_panel_is_opaque; y++)
	{
		for (uint32_t x = jpeg_left; x < jpeg_right; x++)
		{
			if ((decoder.pixel_data()[(size_t)y * decoder.size().width() + x] >> 24) != 255)
			{
				jpeg_panel_is_opaque = false;
			}
		}
	}
	assert(jpeg_panel_is_opaque);
}

void Image::test()
{
	static bool done = false;
	if (done == false)
	{
		MemoryLeakLog
		done = true;
		test9();
		test8();
		test7();
		test6();
		test5();
		test4();
		test3();
		test2();
		test1();
	}
}
