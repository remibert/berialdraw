#include "samples/samples.hpp"

using namespace berialdraw;

#define EVENT_TEXT_ID    122
#define PROGRESS_BAR_ID 123
#define SLIDER_ID       124
#define BUTTON_ID       125
#define EDIT_ID         126
#define SWITCH_ID       127
#define QUIT_ID         128


/** Button pressed */
void on_button_pressed(Widget * widget, const ClickEvent& click_event)
{
}

/** Switch modified */
void on_switch_event(Widget * widget, const CheckEvent& check_event)
{
}

/** Slider modified */
void on_slide_event(Widget * widget, const SlideEvent& slide_event)
{
	Slider * slider = dynamic_cast<Slider*>(widget);
	Window * window = dynamic_cast<Window*>(widget->root());
	if (window && slider)
	{
		ProgressBar * progress = dynamic_cast<ProgressBar*>(window->search(PROGRESS_BAR_ID));
		if (progress)
		{
			progress->value(slider->value());
		}
	}
}

void separator(Widget * parent)
{
	Pane * sep = new Pane(parent);
		sep->color(Color::METAL);
		sep->size(Size::MAX_SIZE,1);
		sep->size_policy(SizePolicy::SHRINK_HEIGHT);
		sep->extend(Extend::EXTEND_WIDTH);
		sep->margin(0,5);
}

/** Show all widgets supported */
void samples_widgets()
{
	Window window;
		//window.position(0,0);
		window.size(320,480);
		//window.color(0xFFF8F8F8);

	Column * main_layout = new Column(&window);
		Button * quit = new Button(main_layout);
			quit->text("< Quit");
			quit->size_policy(SizePolicy::SHRINK_HEIGHT);
			quit->text_align(Align::ALIGN_LEFT);
			quit->thickness(0);
			quit->color(HEADER_COLOR);
			quit->text_color(HEADER_TEXT_COLOR);
			quit->radius(0);
			quit->focusable(false);
			quit->padding(5,10);
			quit->id(QUIT_ID);

		ScrollView * scroll_view = new ScrollView(main_layout);
			scroll_view->extend(Extend::EXTEND_HEIGHT);
			scroll_view->align(Align::ALIGN_TOP);
			scroll_view->margin(5);

			Column * scroll_layout = new Column(scroll_view);

				Label * event_text = new Label(scroll_layout);
					event_text->text("No event");
					event_text->font_size(10);
					event_text->id(EVENT_TEXT_ID);

				Row * switch_layout = new Row(scroll_layout);
					Label * switch_text = new Label(switch_layout);
						switch_text->text("Switch");
						switch_text->align(Align::ALIGN_LEFT);
					Switch * switch_widget = new Switch(switch_layout);
						switch_widget->align(Align::ALIGN_RIGHT);
						switch_widget->id(SWITCH_ID);
						switch_widget->bind(on_switch_event);

				separator(scroll_layout);

				Row * slider_layout = new Row(scroll_layout);
					Label * slider_text = new Label(slider_layout);
						slider_text->text("Slider");
						slider_text->size_policy(SizePolicy::SHRINK_WIDTH);
					Slider * slider_widget = new Slider(slider_layout);
						slider_widget->bind(on_slide_event);
						slider_widget->id(SLIDER_ID);

				separator(scroll_layout);

				Label * progress_text = new Label(scroll_layout);
					progress_text->text("ProgressBar");

				ProgressBar * progress = new ProgressBar(scroll_layout);
					progress->value(25);
					progress->id(PROGRESS_BAR_ID);

				separator(scroll_layout);

				Button * button = new Button(scroll_layout);
					button->text("Button");
					button->extend(Extend::EXTEND_WIDTH);
					button->id(BUTTON_ID);
					button->bind(on_button_pressed);

				separator(scroll_layout);

				Label * edit_text = new Label(scroll_layout);
					edit_text->text("Edit");
					edit_text->text_align(Align::ALIGN_LEFT);

				Edit * edit = new Edit(scroll_layout);
					edit->place_holder("Enter text");
					edit->id(EDIT_ID);

				separator(scroll_layout);

				Label * keyboard_text = new Label(scroll_layout);
					keyboard_text->text("Keyboard");
					keyboard_text->text_align(Align::ALIGN_LEFT);

				Icon * icon = new Icon(scroll_layout);
					icon->filename("resources/icons/person.icn");
					icon->text("Icon");

				Keyboard * keyboard = new Keyboard(scroll_layout);

	UIManager::desktop()->mainloop();
}
