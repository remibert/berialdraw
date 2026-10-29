#include "berialdraw_imp.hpp"

using namespace berialdraw;

void Label::test1()
{
	Dim s = 10;
	Window window;
		window.position(0,0);
		window.size(480,480);
		window.color(Color::WHITE_BLUE);

	Label * label1 = new Label(&window);
		label1->text("ABCDEFGHIJKLMNOPQRSTUVWXYZ");

	Label * label2 = new Label(&window);
		label2->text("abcdefghijklmnopqrstuvwxyz");

	Label * label3 = new Label(&window);
		label3->text("0123456789");

	Label * label4 = new Label(&window);
		label4->text(" &~'\\{([-|_\\^@)]+=}$%%?,.;/:!");

	Label * label5 = new Label(&window);
	int id = 0;
	for(Dim s = 0; s < 100; s+=3)
	{
		label1->position(0,s);
		label1->font_size(s,s);

		label2->position(0,2*s);
		label2->font_size(s,s);

		label3->position(0,3*s);
		label3->font_size(s,s);

		label4->position(0,4*s);
		label4->font_size(s,s);

		label5->text("%d",s);
		label5->position(0,5*s);
		label5->font_size(s,s);

		if (s % 18 == 0)
		{
			String name;
			name.print("$(ui.tests)/out/label1_%d.svg", ++id);
			 UIManager::desktop()->dispatch(name);
		}
		else
		{
			 UIManager::desktop()->dispatch();
		}
	}
}

void Label::test2()
{
	Window window;
		window.position(13,17);
		window.size(450,319);
		window.color(Color::BROWN,32);

	Grid * grid = new Grid(&window);
		Label * label = new Label(grid);
			label->cell(0,0);
			label->text("Hello\nWorld\nHave a nice day");
			label->extend(Extend::EXTEND_NONE);

		label = new Label(grid);
			label->cell(0,1);
			label->text("Hello\nWorld\nHave a nice day");
			label->text_align(Align::ALIGN_LEFT);
			label->extend(Extend::EXTEND_NONE);

		label = new Label(grid);
			label->cell(1,0);
			label->text("Hello\nWorld\nHave a nice day");
			label->text_align(Align::ALIGN_RIGHT);
			label->extend(Extend::EXTEND_NONE);

		label = new Label(grid);
			label->cell(1,1);
			label->text("Hello\nWorld\nHave a nice day");
			label->text_align(Align::CENTER);
			label->extend(Extend::EXTEND_NONE);

	 UIManager::desktop()->dispatch("$(ui.tests)/out/label2_1.svg");
}

void Label::test3()
{
	Window window;
		window.position(0,0);
		window.size(480,200);
		window.color(Color::WHITE);

	// Test rich text with different colors
	Label * label1 = new Label(&window);
		label1->text("Hello $<'text_color':4294901760>Red$$ World");
		label1->position(10,10);
		label1->font_size(20);

	// Test rich text with bold font
	Label * label2 = new Label(&window);
		label2->text("Normal $<'font_familly':'Cerial-bold'>Bold$$ Normal");
		label2->position(10,50);
		label2->font_size(20);

	// Test rich text with different font size
	Label * label3 = new Label(&window);
		label3->text("Small $<'font_size':{'width':32,'height':32}>BIG$$ Small");
		label3->position(10,90);
		label3->font_size(16);

	// Test rich text with combined attributes: bold + color + size
	Label * label4 = new Label(&window);
		label4->text("Mix $<'font_familly':'Cerial-bold','font_size':{'width':28,'height':28},'text_color':4278233600>GreenBold$$ end");
		label4->position(10,140);
		label4->font_size(16);

	 UIManager::desktop()->dispatch("$(ui.tests)/out/label3_1.svg");
}

void Label::test4()
{
	Window window;
		window.position(0,0);
		window.size(480,250);
		window.color(Color::WHITE);

	// Test multiline with rich text
	Label * label1 = new Label(&window);
		label1->text("Line1 $<'text_color':4278190335>Blue$$\nLine2 $<'font_familly':'Cerial-bold','text_color':4294901760>RedBold$$\nLine3 Normal");
		label1->position(10,10);
		label1->font_size(20);

	// Test multiple spans on same line
	Label * label2 = new Label(&window);
		label2->text("$<'text_color':4294901760>R$$$<'text_color':4278233600>G$$$<'text_color':4278190335>B$$ RGB");
		label2->position(10,100);
		label2->font_size(24);

	// Test text with no formatting tags (backward compatibility)
	Label * label3 = new Label(&window);
		label3->text("Plain text no formatting");
		label3->position(10,150);
		label3->font_size(20);

	// Test different sizes on same line
	Label * label4 = new Label(&window);
		label4->text("A$<'font_size':{'width':30,'height':30}>B$$$<'font_size':{'width':40,'height':40}>C$$$<'font_size':{'width':20,'height':20}>D$$");
		label4->position(10,190);
		label4->font_size(14);
	 UIManager::desktop()->dispatch("$(ui.tests)/out/label4_1.svg");
}

void Label::test5()
{
	Window window;
	Label * label4 = new Label(&window);
		label4->position(10,140);
		label4->font_size(16);
		label4->text(
			"$<'font_familly':'Cerial','font_size':{'width':50,'height':80},'text_color':0xFF00FF00>"
			"GreenpÂ"
			"$<'font_familly':'Cerial','font_size':{'width':20,'height':20},'text_color':0xFFFF0000>"
			"Redp_$$endÂ");
	UIManager::desktop()->dispatch("$(ui.tests)/out/label5_1.svg");
	label4->text("Mix$<'font_familly':'Cerial','font_size':{'width':32,'height':32},'text_color':0xFF00FF00>Gr$$end");
	UIManager::desktop()->dispatch("$(ui.tests)/out/label5_2.svg");
	label4->text("Mix$<'font_familly':'Cerial','font_size':{'width':32,'height':32},'text_color':0xFF00FF00>Gre$$end");
	UIManager::desktop()->dispatch("$(ui.tests)/out/label5_3.svg");
	label4->text("Mix$<'font_familly':'Cerial','font_size':{'width':32,'height':32},'text_color':0xFF00FF00>Gren$$end");
	UIManager::desktop()->dispatch("$(ui.tests)/out/label5_4.svg");
}

void Label::test6()
{
	Window window;
	window.color(Color::LIGHT_BEIGE);
	Label* label = new Label(&window);
	label->position(10, 140);
	label->font_size(16);
	label->text(
		"Hello\n"
		"$<'font_familly':'Cerial','font_size':{'width':30,'height':30},'text_color':0xFF7F7F7F>"
		"Image : |_y->"
		"$<'sketch':'$(ui.icons)/filter_vintage.icn','size':{'width':100,'height':100},'color':0xFFFF0000, 'align':'center'><-"
		"$$\nWorld"
	);
	// while(1)
	UIManager::desktop()->dispatch("$(ui.tests)/out/label6_1.svg");

	label->text(
		"Hello\n"
		"$<'font_familly':'Cerial','font_size':{'width':30,'height':30},'text_color':0xFF7F7F7F>"
		"Image : |_y->"
		"$<'sketch':'$(ui.icons)/filter_vintage.icn','size':{'width':10,'height':10},'color':0xFFFF0000, 'align':'center'><-"
		"$$\nWorld"
	);

	UIManager::desktop()->dispatch("$(ui.tests)/out/label6_2.svg");

	label->text(
		"Hello\n"
		"$<'font_familly':'Cerial','font_size':{'width':30,'height':30},'text_color':0xFF7F7F7F>"
		"Image : |_y->"
		"$<'sketch':'$(ui.icons)/filter_vintage.icn','size':{'width':100,'height':100},'color':0xFFFF0000, 'align':'bottom'><-"
		"$$\nWorld"
	);

	UIManager::desktop()->dispatch("$(ui.tests)/out/label6_3.svg");
}

void Label::test7()
{
	Window window;
	window.color(Color::LIGHT_BEIGE);
	Label* label = new Label(&window);
		label->font_size(32);
		label->text(
			"One|\v|\n\n"
			"|\v|Two|\v|\n\n"
			"|\v|Three|\v|\v|\n\n"
			"|\v|\v|Four|\v|\v|\n\n"
			"|\v|\v|Five|\v|\v|\v|\n\n"
			"|\v|\v|\v|Six|\v|\v|\v|"
		);
		label->size_policy(SizePolicy::ENLARGE_ALL);
	UIManager::desktop()->dispatch("$(ui.tests)/out/label7_1.svg");
}

void Label::test8()
{
	class OffscreenTestWindow : public Window
	{
	public:
		void place_for_test()
		{
			Window::place();
		}

		void paint_for_test(const Region& parent_region)
		{
			Window::paint(parent_region);
		}
	};

	Framebuf* screen_framebuf = UIManager::framebuf();
	Renderer* screen_renderer = UIManager::renderer();
	assert(screen_framebuf != nullptr);
	assert(screen_renderer != nullptr);

	std::unique_ptr<Framebuf> offscreen_framebuf(
		Framebuf::create(screen_framebuf->width(), screen_framebuf->height(), Framebuf::ARGB8888));
	assert(offscreen_framebuf != nullptr);

	Size render_size = screen_renderer->size();
	Renderer offscreen_renderer(render_size.width(), render_size.height(), screen_renderer->scale_q6());

	OffscreenTestWindow window;
		window.position(10, 10);
		window.size(100, 40);
		window.color(0x80FF0000);
		Label* label = new Label(&window);
			label->position(4, 4);
			label->font_size(8);
			label->text_color(Color::WHITE);
			label->text("Offscreen");
	const int32_t test_x = (10 + 100 - 5) * screen_renderer->scale();
	const int32_t test_y = (10 + 40 - 5) * screen_renderer->scale();
	const int32_t transparent_x = (int32_t)screen_framebuf->width() - 1;
	const int32_t transparent_y = (int32_t)screen_framebuf->height() - 1;
	uint32_t screen_pixel_before = screen_framebuf->pixel(test_x, test_y);

	assert(offscreen_framebuf->pixel(test_x, test_y) != 0x80FF0000);

	Region no_occlusion;
	{
		// Paint directly into the offscreen target; do not dispatch or refresh the device.
		UIManager::RenderContextScope render_context(offscreen_renderer, *offscreen_framebuf);
		assert(UIManager::renderer() == &offscreen_renderer);
		assert(UIManager::framebuf() == offscreen_framebuf.get());

		offscreen_framebuf->clear(0x00000000);
		window.place_for_test();
		window.paint_for_test(no_occlusion);
	}

	assert(UIManager::renderer() == screen_renderer);
	assert(UIManager::framebuf() == screen_framebuf);
	assert(offscreen_framebuf->pixel(test_x, test_y) == 0x80FF0000);
	assert(offscreen_framebuf->pixel(transparent_x, transparent_y) == 0x00000000);
	assert(screen_framebuf->pixel(test_x, test_y) == screen_pixel_before);

	// Convert the placed window's Q6 bounds to physical framebuffer pixel bounds.
	const int64_t scale_q6 = screen_renderer->scale_q6();
	auto floor_scaled = [scale_q6](int64_t coordinate_q6)
	{
		int64_t value = coordinate_q6 * scale_q6;
		return value >= 0 ? value / 4096 : -((-value + 4095) / 4096);
	};
	auto ceil_scaled = [scale_q6](int64_t coordinate_q6)
	{
		int64_t value = coordinate_q6 * scale_q6;
		return value >= 0 ? (value + 4095) / 4096 : -((-value) / 4096);
	};

	const Area& window_bounds = window.backclip();
	int64_t crop_left = floor_scaled(window_bounds.x_q6());
	int64_t crop_top = floor_scaled(window_bounds.y_q6());
	int64_t crop_right = ceil_scaled((int64_t)window_bounds.x_q6() + window_bounds.width_q6());
	int64_t crop_bottom = ceil_scaled((int64_t)window_bounds.y_q6() + window_bounds.height_q6());

	uint32_t requested_width = (uint32_t)(crop_right - crop_left);
	uint32_t requested_height = (uint32_t)(crop_bottom - crop_top);
	std::unique_ptr<Argb8888> cropped = Argb8888::crop(
		*offscreen_framebuf, (int32_t)crop_left, (int32_t)crop_top, requested_width, requested_height);
	assert(cropped != nullptr);

	int64_t clipped_left = crop_left < 0 ? 0 : crop_left;
	int64_t clipped_top = crop_top < 0 ? 0 : crop_top;
	int64_t clipped_right = crop_right > screen_framebuf->width() ? screen_framebuf->width() : crop_right;
	int64_t clipped_bottom = crop_bottom > screen_framebuf->height() ? screen_framebuf->height() : crop_bottom;
	assert(cropped->width() == (uint32_t)(clipped_right - clipped_left));
	assert(cropped->height() == (uint32_t)(clipped_bottom - clipped_top));
	assert(cropped->pixel(test_x - (int32_t)clipped_left, test_y - (int32_t)clipped_top) == 0x80FF0000);
	assert(cropped->pixel(0, 0) == offscreen_framebuf->pixel((int32_t)clipped_left, (int32_t)clipped_top));
	std::unique_ptr<Argb8888> edge_cropped = Argb8888::crop(
		*offscreen_framebuf, -5, -7, screen_framebuf->width() + 5, screen_framebuf->height() + 7);
	assert(edge_cropped != nullptr);
	assert(edge_cropped->width() == screen_framebuf->width());
	assert(edge_cropped->height() == screen_framebuf->height());
	assert(Argb8888::crop(*offscreen_framebuf, (int32_t)screen_framebuf->width(), 0, 1, 1) == nullptr);
}

void Label::test9()
{
	Fonts * fonts = UIManager::fonts();
	assert(fonts != nullptr);
	assert(fonts->define_vector_glyph((wchar_t)'Z', String("$(ui.icons)/maison.icn")));
	assert(fonts->define_vector_glyph((wchar_t)0xE000, String("$(ui.icons)/maison.icn")));
	assert(fonts->define_vector_glyph((wchar_t)0xE001, String("$(ui.icons)/home.icn")));

	Window window;
	window.size(480, 240);
	window.color(Color::WHITE);

	Label * label = new Label(&window);
	label->font_size(22);
	label->text_color(Color::BLUE);

	String text("\n _Z_ : existing char not replaced\n\n _");
	text += (wchar_t)0xE000;
	text += "_ : colored icon\n\n _";
	text += (wchar_t)0xE001;
	text += "_ : monochrome icon";
	label->text(text);

	UIManager::desktop()->dispatch("$(ui.tests)/out/label9_1.svg");

	fonts->remove_vector_glyph((wchar_t)'Z');
	fonts->remove_vector_glyph((wchar_t)0xE000);
	fonts->remove_vector_glyph((wchar_t)0xE001);
}

void Label::test()
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
