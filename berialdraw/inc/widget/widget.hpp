#ifndef Widget_hpp_INCLUDED
#define Widget_hpp_INCLUDED
namespace berialdraw
{
	class Desktop;
	class Region;

	/** The Widget class serves as the foundational building block for all user interface components. 
	It provides common properties and methods such as dimensions, and event handling */
	class Widget : public CommonStyle, public WidgetStyle
	{
	friend class Cells;
	friend class Invalidator;
	public:
		/** Create widget */
		Widget(const char * classname, Widget * parent, size_t size_of_widget);

		/** Destroy widget */
		virtual ~Widget();

		/** Paint on screen memory the content of this widget */
		virtual void paint(const Region & parent_region);

		/** Place the widget in the area */
		virtual void place(const Area & area, bool in_layout);

		/** Return the parent of this widget */
		virtual Widget * parent();

		/** Return the root widget parent of this widget (window pointer generaly) */
		virtual Widget * root();

		/** Return the scroll_view parent of this widget or null if not found */
		virtual ScrollView * scroll_view();

		/** Return the size of content without marges */
		virtual Size content_size();

		/** Return the size of content with its marges */
		virtual Size marged_size();

		/** Clears all children from the widget */
		virtual void clear();

		/** Get the foreground area clip */
		virtual const Area & foreclip() const;

		/** Get the background area clip */
		virtual const Area & backclip() const;

		/** Clean all dirty flag in all */
		void clean_all();

		/** Get the next widget */
		inline Widget * next()
		{
			return m_next;
		}

		/** Search widget with this identifier */
		Widget * search(uint16_t id);

		/** Return the area occuped by this window */
		virtual Area area();

		/** Return the name of class */
		const char * classname() const;

		/** Bind event on a method */
		template<class CLASS, class EVENT> void bind(
			CLASS * object, 
			void (CLASS::*method)(Widget * , const EVENT & ))
		{
			if (object && method)
			{
				UIManager::notifier()->bind(new MethodCaller<CLASS,EVENT>(object, method, this));
			}
		}

		/** Bind event on a function */
		template<class EVENT>
		void bind(void (*function)(Widget *, const EVENT &))
		{
			if (function)
			{
				UIManager::notifier()->bind(new FunctionCaller<EVENT>(function, this));
			}
		}
		/** Return the widget hovered */
		virtual Widget * hovered(const Region & parent_region, const Point & position);

		/** Serialize the content of widget into json */
		virtual void serialize(JsonIterator & it) = 0;

		/** Unserialize the content of widget from json */
		virtual void unserialize(JsonIterator & it) = 0;

		/** Return the color value when the widget is pressed */
		uint32_t pressed_color(uint32_t color, bool pressed);

		/** Return the stated color */
		uint32_t stated_color(uint32_t color);

	protected:
/// @cond DOXYGEN_IGNORE

		/** Add dirty on the widget and all of its children */
		void dirty_children(enum Invalidator::Status status);

		/** Get the size of children */
		Size children_size();

		/** Set the focus on this widget */
		void focus_to(Widget * & current_focus, Widget * new_focus);

		void focusables(Vector<Widget *> & lst);
		void focus_next(Widget * & widget);
		void focus_previous(Widget * & widget);

		/** Compute the scroll area */
		virtual void space_occupied(Point & min_position, Point & max_position);

		Area m_foreclip;
		Area m_backclip;
		Widget * m_parent = 0;
		Widget * m_children = 0;
		Widget * m_next = 0;
		const char * m_classname = "Widget";

		
		void place_in_area(const Area & area, bool in_layout);
/// @endcond 
	};
}
#endif