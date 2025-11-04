#pragma once
namespace berialdraw
{
	class Widget;
	/** Store event to notify callback */
	class Notifier
	{
	public:
		/** Constructor */
		Notifier();

		/** Destructor */
		virtual ~Notifier();

		/** Remove and destroy selected widget */
		void remove(Widget * widget);

		/** Registers method call back 
		@param bind callback class */
		void bind (Callable * bind);

		/** Send touch notification */
		void touch(Coord x, Coord y, TouchEvent::TouchState state);

		/** Send key notification */
		void key(wchar_t key, KeyEvent::KeyState state, KeyEvent::Modifier modifier = KeyEvent::MODIFIER_NONE);

		/** Scroll widget if scroll possible */
		void scroll(Coord x, Coord y, Widget * widget);

		/** Click on widget */
		void click(Coord x, Coord y, Widget * widget=0);

		/** Click on check */
		void check(bool state, Widget * widget);
		
		/** Slide the slider */
		void slide(int32_t value, Widget *widget);

		/** Adds an event to the list of events that will be processed later */
		void push_event(Event * evt);

		/** Dispatch all events stored */
		void dispatch();

		/** Enumeration for different types of log events. */
		enum LogEvent
		{
			LOG_NONE   = 0,                        ///< No event.
			LOG_CLICK  = 1,                        ///< Click event.
			LOG_MOVE   = 2,                        ///< Move event.
			LOG_KEY    = 4,                        ///< Key event.
			LOG_SCROLL = 8,                        ///< Scroll event.
			LOG_USER   = (LOG_CLICK | LOG_SCROLL | LOG_KEY | LOG_SCROLL), ///< User event combining click, scroll, and key events.
		};

		/** Log all events to the output console */
		void log(LogEvent value=LOG_USER);

		/** Plays a file containing events, and takes a picture when the Pause key is pressed.
		@param script JSON script, composed of all the events to play.
		Json example:

		[
			{"type":"touch","x":0,"y":0,"state":"down"},
			{"type":"key","state":"down","modifier":"alt","key":0x20}
		]

		Each line contains a low-level event (only those triggered by the keyboard and touch panel press).
		The supported values are:
		- type: touch, key
		- state: up, down, move (only for touch)
		- modifier: alt, shift, control
		@param snapshot_name Name of the SVG file to generate when the Pause key is pressed. If the key is pressed multiple times, the name should include a %d in it. */
		void play_script(TextStream & script, const String & snapshot_name);

		/** Open a file containing events, and takes a picture when the Pause key is pressed. */
		bool start_script(TextStream & script, const String & snapshot_name);

		/** Play one step of event script
		@return filename of the snapshot to take or null if there is no snapshot */
		const char * next_step_script();

		/** Indicate script started */
		bool is_script_started();

		/** Set the snapshot filename when the pause is pressed, the file will be saved */
		void snapshot(const String & filename);
	private:
#ifndef SWIG
/// @cond DOXYGEN_IGNORE
		/** Log event */
		void log(Event * evt);

		/** Notify desktop with a key event */
		void notify(KeyEvent & evt);

		/** Notify focus event */
		void notify(Event & evt);

		/** Notify desktop with a touch event */
		Widget * notify(TouchEvent & evt);

		/** Notify desktop with a touch up event */
		Widget * treat_touch_up(TouchEvent & evt);

		/** Notify desktop with a touch down event */
		void treat_touch_down(TouchEvent & evt);

		/** Notify desktop with a touch down event */
		void treat_touch_move(TouchEvent & evt);

		/** Call callback touch event */
		void notify_touch_callback(TouchEvent & evt);

		/** Notify desktop with an event */
		void notify(Event * evt);

		/** Indicate if the event is a touch move */
		bool is_touch_move(Event * evt);

		/** Clean up call back according to widget handle */
		void clean_up_callback(Widget *widget);

		/** Clean up event according to widget handle */
		void clean_up_event(Widget *widget);

		Vector<Callable *> m_callbacks;
		Vector<Event *> m_events;

		Widget * m_clicked = 0;
		Widget * m_selected = 0;
		Widget * m_hovered = 0;
		Widget * m_scrolled = 0;
		
		Point   m_down_position;
		Point   m_move_position;
		String * m_snapshot_name = 0;
		uint32_t m_snapshot_id = 0;
		String * m_snapshot_current = 0;
		Json *   m_json_script = 0;
		int32_t  m_json_index = 0;
		uint32_t m_proof = 0;
		unsigned int     m_playing_script:1;
		unsigned int     m_touch_down:1;
		unsigned int     m_log:4;
		unsigned int     m_move_count:6;
/// @endcond
#endif
	};
};
