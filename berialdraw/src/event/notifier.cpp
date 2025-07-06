#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Mark the pointer to indicate that they should be destroyed as soon as possible.*/
inline Event* mark_destroy(Event * ptr) 
{
	return (Event*)((uintptr_t)ptr | 1);
}

/** Mark the pointer to indicate that it used.*/
inline Event* mark_used(Event* ptr) 
{
	return (Event*)((uintptr_t)ptr | 2);
}

/** Mark the pointer to indicate that it not used.*/
inline Event* unmark_used(Event* ptr) 
{
	return (Event*)((uintptr_t)ptr & (~(uintptr_t)2));
}

/** Indicates whether the pointer is marked for destruction and is not null */
inline bool is_marked_destroy(void* ptr) 
{
	return ((uintptr_t)ptr & 1) && (ptr != nullptr);
}

/** Indicates whether the pointer is used */
inline bool is_used(void* ptr) 
{
	return ((uintptr_t)ptr & 2) && (ptr != nullptr);
}

/** Indicates whether the pointer is not used */
inline bool is_not_used(void* ptr) 
{
	return (((uintptr_t)ptr & 2) == 0) && (ptr != nullptr);
}

/** Indicates whether the pointer is not marked for destruction and is not null */
inline bool is_not_marked_destroy(void* ptr) 
{
	return (((uintptr_t)ptr & 1) == 0) && (ptr != nullptr);
}

/** returns the value of the pointer without the mark for destruction */
inline void* remove_mark(void* ptr) 
{
	return (void*)((uintptr_t)ptr & (~(uintptr_t)3));
}

/** Constructor */
Notifier::Notifier()
{
	m_touch_down = 0;
	m_playing_script = 0;
	m_log = LogEvent::LOG_NONE;
}

/** Destructor */
Notifier::~Notifier()
{
	delete m_snapshot_name;
	delete m_snapshot_current;
	for (uint32_t i = 0; i < m_callbacks.size(); i++)
	{
		delete m_callbacks[i];
	}
	m_callbacks.clear();
	for (uint32_t i = 0; i < m_events.size(); i++)
	{
		delete (Event*)remove_mark(m_events[i]);
	}
	delete m_json_script;
	m_events.clear();
}

/** Notify event */
void Notifier::notify(Event & evt)
{
	Widget * widget = evt.widget();
	if (widget)
	{
		Widget * window = widget->root();

		// Search callback registered for this event
		for(uint32_t i = 0; i < m_callbacks.size(); i++)
		{
			Callable * callback = m_callbacks[i];
			if (callback)
			{
				// If event is touch or click
				if (callback->type() == evt.type())
				{
					// If the widget hovered have a callback registered
					if (widget == callback->widget() || window == callback->widget())
					{
						// Call callback
						callback->call(widget, &evt);
					}
				}
			}
		}
	}
}

/** Notify desktop with a key event */
void Notifier::notify(KeyEvent & evt)
{
	Widget * front_window = UIManager::desktop()->front();

	if (front_window)
	{
		// Search the callback registered for this event
		for(uint32_t i = 0; i < m_callbacks.size(); i++)
		{
			Callable * callback = m_callbacks[i];
			if (callback)
			{
				Widget * widget = callback->widget();

				// If callback is for this event type
				if(callback->type() == evt.type() && widget)
				{
					// If the widget or window is equal
					if(widget == front_window || widget->root() == front_window)
					{
						// Call callback
						callback->call(widget, &evt);
					}
				}
			}
		}
	}
}

/** Notify desktop with a touch down event */
void Notifier::treat_touch_down(TouchEvent & evt)
{
	if(m_clicked)
	{
		m_clicked->pressed(false);
		m_clicked = 0;
		m_selected = 0;
	}

	// If hovered widget detected
	if(m_hovered)
	{
		// Select new pressed widget
		m_clicked = m_hovered;
		m_clicked->pressed(true);
		m_down_position = evt.position();
		m_move_position = evt.position();
		m_scrolled = 0;
		m_selected = 0;
	}

	m_move_count = 0;
	m_touch_down = 1;
}

/** Notify desktop with a touch down event */
void Notifier::treat_touch_move(TouchEvent & evt)
{
	// Move on same widget
	if (m_clicked && (m_hovered == m_clicked))
	{
		if (m_move_count <= 6)
		{
			m_move_count ++;
		}
		else
		{
			if (m_clicked->focused() || m_clicked->selectable())
			{
				push_event(new SelectEvent(SelectEvent::SELECT_START, m_down_position,m_clicked));
				m_selected = m_clicked;
				m_clicked->pressed(false);
				m_clicked = 0;
			}
			else
			{
				m_scrolled = m_clicked;
				m_clicked->pressed(false);
				m_clicked = 0;
			}
		}
	}
	else
	{
		Widget * scrolled = 0;

		if (m_selected)
		{
			push_event(new SelectEvent(SelectEvent::SELECT_PROGRESS, evt.position(), m_selected));
		}
		else if (m_clicked)
		{
			scrolled = m_clicked;
			m_clicked->pressed(false);
			m_scrolled = m_clicked;
			m_clicked = 0;
		}
		else if (m_scrolled)
		{
			scrolled = m_scrolled;
		}

		if (scrolled)
		{
			Point shift;
			shift.set(m_move_position.x()-evt.position().x(), m_move_position.y()-evt.position().y());
			scroll(shift.x(), shift.y(), scrolled);
			m_move_position = evt.position();
		}
	}
}

/** Notify desktop with a touch up event */
Widget * Notifier::treat_touch_up(TouchEvent & evt)
{
	Widget * clicked = 0;

	if (m_selected)
	{
		push_event(new SelectEvent(SelectEvent::SELECT_END, evt.position(), m_selected));
	}

	if (m_clicked)
	{
		// If the widget is the same that the widget which reveive the touch down
		if(m_clicked == m_hovered)
		{
			clicked = m_clicked;
			m_clicked->pressed(false);
		}
		if(m_clicked)
		{
			m_clicked->pressed(false);
			m_clicked = 0;
		}
	}
	m_scrolled = 0;
	m_clicked = 0;
	m_selected = 0;
	m_touch_down = 0;
	return clicked;
}

/** Call callback touch event */
void Notifier::notify_touch_callback(TouchEvent & evt)
{
	// Search callback registered for this event
	for(uint32_t i = 0; i < m_callbacks.size() && m_hovered; i++)
	{
		Callable * callback = m_callbacks[i];
		if (callback)
		{
			// If the widget hovered have a callback registered
			if (m_hovered == callback->widget())
			{
				// If event is touch or click
				if (callback->type() == evt.type())
				{
					// Call callback
					callback->call(m_hovered, &evt);
				}
			}
		}
	}
}

/** Notify desktop with a touch event */
Widget * Notifier::notify(TouchEvent & evt)
{
	Widget * clicked = 0;

	m_hovered = UIManager::desktop()->hovered(evt.position());

	// If touch down detected
	if(evt.state() == TouchEvent::TOUCH_DOWN)
	{
		treat_touch_down(evt);
	}
	// If touch up detected
	else if(evt.state() == TouchEvent::TOUCH_UP)
	{
		clicked = treat_touch_up(evt);
	}
	// If touch move detected
	else if (evt.state() == TouchEvent::TOUCH_MOVE)
	{
		treat_touch_move(evt);
	}
	notify_touch_callback(evt);
	return clicked;
}

/** Scroll widget if scroll possible */
void Notifier::scroll(Coord x, Coord y, Widget * widget)
{
	if (widget)
	{
		ScrollView * scroll_view = dynamic_cast<ScrollView*>(widget);
		if (scroll_view == 0)
		{
			scroll_view = dynamic_cast<ScrollView*>(widget->scroll_view());
		}
		if (scroll_view)
		{
			const Point shift(x,y);
			push_event(new ScrollEvent(shift,scroll_view));
		}
	}
}

/** Click on check */
void Notifier::check(bool state, Widget * widget)
{
	if (widget)
	{
		push_event(new CheckEvent(state, widget));
	}
}

/** Slide the slider */
void Notifier::slide(int32_t value, Widget *widget)
{
	if (widget)
	{
		push_event(new SlideEvent(value, widget));
	}
}

/** Click on widget */
void Notifier::click(Coord x, Coord y, Widget * widget)
{
	Point position(x,y);
	Widget * clicked = widget;
	if (clicked == 0)
	{
		clicked = UIManager::desktop()->hovered(position);
	}
	if (clicked)
	{
		push_event(new ClickEvent(position, clicked));
	}
}

/** Notify desktop with an event */
void Notifier::notify(Event * evt)
{
	if(is_not_marked_destroy(evt))
	{
		// If touch pressed
		if(evt->type() == TouchEvent::type_id())
		{
			TouchEvent * touch_event = dynamic_cast<TouchEvent*>(evt);
			if (touch_event)
			{
				Widget * clicked = notify(*touch_event);
				if (clicked)
				{
					click(touch_event->position().x(), touch_event->position().y(), clicked);
				}
			}
		}
		// If key pressed
		else if(evt->type() == KeyEvent::type_id())
		{
			KeyEvent * key_event = dynamic_cast<KeyEvent*>(evt);
			if (key_event)
			{
				notify(*key_event);
			}
		}
		else
		{
			notify(*evt);
		}
	}
}

/** Clean up call back according to widget handle */
void Notifier::clean_up_callback(Widget *widget)
{
	for (uint32_t i = 0; i < m_callbacks.size(); i++)
	{
		Callable * callback = m_callbacks[i];
		if (callback)
		{
			if (callback->widget() == widget)
			{
				delete callback;
				m_callbacks[i] = 0;
			}
		}
	}
}

/** Remove and destroy selected widget */
void Notifier::remove(Widget * widget)
{
	clean_up_callback(widget);
	if (m_hovered == widget)
	{
		m_hovered = 0;
	}
	if (m_clicked == widget)
	{
		m_clicked = 0;
	}
	if (m_selected == widget)
	{
		m_selected = 0;
	}
}

/** Registers method call back 
@param bind callback class */
void Notifier::bind(Callable * bind)
{
	m_callbacks.push_or_reuse(bind);
}

/** Send touch notification */
void Notifier::touch(Coord x, Coord y, TouchEvent::TouchState state)
{
	Point position(x, y);
	push_event(new TouchEvent(position, state));
}

/** Send key notification */
void Notifier::key(wchar_t key, KeyEvent::KeyState state, KeyEvent::Modifier modifier)
{
	push_event(new KeyEvent(key, state, modifier));
}

/** Indicate if the event is a touch move */
bool Notifier::is_touch_move(Event * evt)
{
	bool result = false;

	if (evt)
	{
		if (evt->type() == TouchEvent::type_id())
		{
			TouchEvent * touch_event = dynamic_cast<TouchEvent*>(evt);
			if (touch_event->state() == TouchEvent::TOUCH_MOVE)
			{
				result = true;
			}
		}
	}
	return result;
}

/** Log event */
void Notifier::log(Event * evt)
{
	if (m_log != LOG_NONE)
	{
 		String str;
		bool display = true;
		evt->to_string(str);
		if (evt->type() == TouchEvent::type_id())
		{
			TouchEvent * touch_event = dynamic_cast<TouchEvent*>(evt);
			if (touch_event->state() == TouchEvent::TOUCH_MOVE)
			{
				static int move_counter = 0;
				if ((move_counter ++) % 4 == 0)
				{
					display = true;
				}
				else
				{
					display = false;
				}
			}
		}

		if (evt->type() == TouchEvent::type_id())
		{
			TouchEvent * touch_event = dynamic_cast<TouchEvent*>(evt);
			if (touch_event->state() == TouchEvent::TOUCH_MOVE)
			{
				if ((m_log & LOG_SCROLL))
				{
					if (m_touch_down == 0)
					{
						display = false;
					}
				}
				else if ((m_log & LOG_MOVE) == 0)
				{
					display = false;
				}
			}
			else
			{
				if ((m_log & LOG_CLICK) == 0)
				{
					display = false;
				}
			}
		}
		if (evt->type() == KeyEvent::type_id())
		{
			if ((m_log & LOG_KEY) == 0)
			{
				display = false;
			}
		}

		if (evt->type() == TouchEvent::type_id() || evt->type() == KeyEvent::type_id())
		{
			if (display)
			{
				bd_printf("\"%s\"\n",str.c_str());
			}
		}
	}
}

/** Adds an event to the list of events that will be processed later */
void Notifier::push_event(Event * evt)
{
	if (evt)
	{
		log(evt);

		// If a space in events list
		if (m_events.size() < 16)
		{
			// Push the event
			m_events.push_or_reuse(evt);
		}
		// If the space not saturated
		else if (m_events.size() < 32)
		{
			bool collapse = false;

			// If the event can be ignored
			if (is_touch_move(evt) == false)
			{
				// Push the event
				m_events.push_or_reuse(evt);
			}
			else
			{
				// List full all new events ignored
				delete evt;
			}
		}
		else
		{
			// List full all new events ignored
			delete evt;
		}
	}
}


/** Dispatch all events stored */
void Notifier::dispatch()
{
	Event * evt;
	m_proof ++;
	for (uint32_t i = 0; i < m_events.size(); i++)
	{
		evt = m_events[i];
		if (evt)
		{
			bool not_used = is_not_used(evt);
			if (not_used)
			{
				m_events[i] = mark_used(m_events[i]);
			}
			m_events[i] = mark_destroy(m_events[i]);
			notify(evt);
			if (not_used)
			{
				m_events[i] = unmark_used(m_events[i]);
			}
		}
	}

	for (uint32_t i = 0; i < m_events.size(); i++)
	{
		Event * evt = m_events[i];
		if (is_marked_destroy(evt) && is_not_used(evt))
		{
			delete (Event*)remove_mark(evt);
			m_events[i] = 0;
		}
	}

	m_proof --;
	if (m_proof == 0)
	{
		m_events.clear();
	}
}

/** Log all events to the output console */
void Notifier::log(Notifier::LogEvent value)
{
	m_log = (Notifier::LogEvent)value;
}

/** Set the snapshot filename when the pause is pressed, the file will be saved */
void Notifier::snapshot(const String & snapshot_name)
{
	if (m_snapshot_current == 0)
	{
		m_snapshot_current = new String;
	}
	if (m_snapshot_name == 0)
	{
		m_snapshot_name = new String;
	}
	*m_snapshot_name = snapshot_name;
	
	m_snapshot_id = 0;
}

/** Open a file containing events, and takes a picture when the Pause key is pressed. */
bool Notifier::start_script(TextStream & script, const String & snapshot_name)
{
	bool result = false;
	if (m_json_script)
	{
		delete m_json_script;
	}
	m_json_script = new Json;
	m_json_index = 0;
	
	try
	{
		m_json_script->unserialize(script);
		result = true;
		
	}
	catch(JsonUnserializeError err)
	{
		bd_printf("Error parsing event script at line %d column %d",err.m_line,err.m_column);
	}

	snapshot(snapshot_name);
	return result;
}

/** Indicate script started */
bool Notifier::is_script_started()
{
	return m_json_script != 0 && m_playing_script != 1;
}


const char * Notifier::next_step_script()
{
	const char * result = 0;
	if (m_json_script && m_snapshot_current && m_snapshot_name)
	{
		JsonIterator arr(*m_json_script);
		m_snapshot_current->clear();

		if (m_json_index < arr.count())
		{
			JsonIterator evt(arr[m_json_index++]);
			if (evt["type"] == "snapshot")
			{
				snapshot(evt["filename"]);
			}
			else if (evt["type"] == "key")
			{
				KeyEvent::KeyState state = KeyEvent::KeyState::KEY_DOWN;
				wchar_t key_ = evt["key"] | 0;
				KeyEvent::Modifier modifier = KeyEvent::Modifier::MODIFIER_NONE;
				
				if (key_)
				{
					if (evt["state"] == "up")
					{
						state = KeyEvent::KeyState::KEY_UP;
					}

					if (evt["modifier"] == "alt")
					{
						modifier = KeyEvent::Modifier::MODIFIER_ALT;
					}
					else if (evt["modifier"] == "ctrl")
					{
						modifier = KeyEvent::Modifier::MODIFIER_CONTROL;
					}
					else if (evt["modifier"] == "shift")
					{
						modifier = KeyEvent::Modifier::MODIFIER_SHIFT;
					}

					if (key_ == (wchar_t)ReservedKey::KEY_PAUSE && state == KeyEvent::KEY_DOWN)
					{
						m_snapshot_current->print(m_snapshot_name->c_str(), ++m_snapshot_id);
					}
					key(key_, state, modifier);
				}
			}
			else if (evt["type"] == "touch")
			{
				Coord x = evt["x"] | Size::MAX_SIZE;
				Coord y = evt["y"] | Size::MAX_SIZE;

				if (x != Size::MAX_SIZE && y != Size::MAX_SIZE)
				{
					TouchEvent::TouchState state = TouchEvent::TouchState::TOUCH_DOWN;

					if (evt["state"] == "up")
					{
						state = TouchEvent::TouchState::TOUCH_UP;
					}
					else if (evt["state"] == "down")
					{
						state = TouchEvent::TouchState::TOUCH_DOWN;
					}
					else if (evt["state"] == "move")
					{
						state = TouchEvent::TouchState::TOUCH_MOVE;
					}
					touch(x,y,state);
				}
			}
			
			if (m_snapshot_current->size() > 0)
			{
				result = m_snapshot_current->c_str();
			}
		}
		else
		{
			delete m_json_script;
			delete m_snapshot_current;
			m_snapshot_current = 0;
			delete m_snapshot_name;
			m_snapshot_name = 0;
			m_json_script = 0;
			m_json_index = 0;
		}
	}
	return result;
}


/** Plays a file containing events, and takes a picture when the Pause key is pressed. */
void Notifier::play_script(TextStream & script, const String & snapshot_name)
{
	if (start_script(script, snapshot_name))
	{
		const char * snapshot;
		m_playing_script = 1;
		while (m_json_script != 0)
		{
			snapshot = next_step_script();
			UIManager::desktop()->dispatch(snapshot);
		}
		m_playing_script = 0;
	}
}
