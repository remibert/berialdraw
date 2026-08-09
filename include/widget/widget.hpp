#pragma once
namespace berialdraw
{
	class Desktop;
	class Region;

	/** Style cascade mode for widget hierarchy */
	enum class StyleCascadeMode : uint8_t
	{
		NONE,        /**< No cascading (default) */
		CASCADING,   /**< Cascades style to children */
		TRANSPARENT  /**< Passes parent's cascade through (transparent) */
	};

	/** The Widget class serves as the foundational building block for all user interface components. 
	It provides common properties and methods such as dimensions, and event handling */
	class Widget : public CommonStyle, public WidgetStyle, public PaddingStyle
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

		/** Place the widget in the area */
		virtual void place_children(const Area& area, bool in_layout);

		/** Scroll the widget and its children by the specified move offset */
		virtual void scroll(const Point & move);

		/** Return the parent of this widget */
		virtual Widget * parent();

		/** Return the focusable widget parent of this widget */
		virtual Widget * focusable_parent();

		/** Return the root widget parent of this widget (window pointer generaly) */
		virtual Widget * root();

		/** Return the scroll_view parent of this widget or null if not found */
		virtual Widget * scrollable_content();

		/** Return the size of content without marges */
		virtual Size content_size();

		/** Return the size of children content without marges */
		virtual Size children_content_size();

		/** Return the size of content with its marges */
		virtual Size marged_size();

		/** Clears all children from the widget */
		virtual void clear();

		/** Get the foreground area clip */
		virtual const Area & foreclip() const;

		/** Get the background area clip */
		virtual const Area & backclip() const;

		 /** Get the foreground area clip (non-const inline version) */
		inline Area & foreclip()
		{
			return m_foreclip;
		}

		/** Get the background area clip (non-const inline version) */
		inline Area & backclip()
		{
			return m_backclip;
		}

		 /** Clean all dirty flag in all */
		void clean_all();

		/** Get the next widget */
		inline Widget * next()
		{
			return m_next;
		}

		/** Get the children widget */
		inline Widget * children()
		{
			return m_children;
		}

		/** Search widget with this identifier */
		Widget * search(uint16_t id);

		/** Return the area occuped by this window */
		virtual Area area();

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

		/** Get the actual parent for a child widget (can be overridden to redirect children) */
		virtual Widget * get_parent_for_child()
		{
			return this;
		}

		/** Get the style cascade mode for this widget */
		virtual StyleCascadeMode style_cascade_mode() const = 0;

		/** Serialize the content of widget into json */
		virtual void serialize(JsonIterator & it) = 0;

		/** Unserialize the content of widget from json */
		virtual void unserialize(JsonIterator & it) = 0;

		/** Get the class name of this widget (e.g., "Button", "TextEdit", "Label") */
		virtual const char* classname() const { return m_classname; }

		/** Return the color value when the widget is pressed */
		uint32_t pressed_color(uint32_t color, bool pressed);

		/** Return the stated color */
		uint32_t stated_color(uint32_t color);

		/** Return the parent focus color */
		uint32_t parent_focus_color(uint32_t color);

		/** Return the stated color with alpha */
		uint32_t stated_color(uint32_t color, uint8_t alpha);

		/** Return the parent focus color with alpha */
		uint32_t parent_focus_color(uint32_t color, uint8_t alpha);

		/** Paint the widget background */
		void paint_background(Area& rectclip, const CommonStyle& common_style,
			const BorderStyle& border_style);

	protected:
/// @cond DOXYGEN_IGNORE

		/** Indicates whether the widget is positioned absolutely */
		bool is_absolute();

		/** Remove operator = */
		Widget& operator=(const Widget& other) = delete;

		/** Add dirty on the widget and all of its children */
		void dirty_children(enum Invalidator::Status status);

		/** Get the size of children */
		Size children_size();

		/** Set the focus on this widget */
		void focus_to(Widget * & current_focus, Widget * new_focus);

		void focusables(Vector<Widget *> & lst);
		void focus_next(Widget * & widget);
		void focus_previous(Widget * & widget);
		void change_focus(Widget * & widget, Widget * & new_widget_focus, Vector<Widget *> & all);

		/** Compute the size according to different size defined */
		Size compute_size(const Size & size, const Size & min_size, const Size & max_size, const Margin & margin);

		/** Compute the scroll area */
		virtual void space_occupied(Point & min_position, Point & max_position);

		/** Compute the scroll area */
		virtual void one_space_occupied(Point & min_position, Point & max_position, const Point & position, const Size & margin_size);

		/** Apply cascade styles to this widget and all its children recursively */
		void apply_cascade_styles();

		Area m_foreclip;
		Area m_backclip;
		Area m_contentclip;
		Widget * m_parent = 0;
		Widget * m_children = 0;
		Widget * m_last_children = 0;
		Widget * m_next = 0;
		const char * m_classname = "Widget";

		
		void place_in_area(const Area & area, bool in_layout);
		void place_in_area_extend(const Area& area, bool & in_layout);		

		void place_text_with_element(
			const Size& text_size, const Size& element_size, Dim text_padding, 
			Area& text_foreclip, Area& element_foreclip, Align text_align_with_bottom,
			Extend extend);

		// New placement pipeline methods

		/** Check if the widget has any axis of position explicitly defined */
		bool has_defined_position() const;

		/** Determine if widget should use layout mode.
		Layout mode is used when parent_in_layout is true, or when the widget
		has no explicit position and has an extend policy set */
		bool resolve_in_layout(bool parent_in_layout) const;

		/** Constrain a base size by clamping to m_min_size and m_max_size */
		Size constrain_content_size(const Size & base) const;

		/** Compute m_foreclip in layout mode using margin, extend, align */
		void compute_layout_clip(const Area & area);

		/** Compute m_foreclip in absolute mode using position offset and constrained size */
		void compute_absolute_clip(const Area & area);

		/** Full placement pipeline: sets backclip, resolves mode, computes foreclip,
		applies thickness, computes contentclip.
		@param area Parent area
		@param in_layout Hint from parent (updated by this method)
		@param thickness Border thickness to subtract (0 = no border)
		@param expand true indicates that the placement must be occuped the total area */
		void compute_widget_placement(const Area & area, bool & in_layout, uint16_t thickness = 0, bool expand=false);
/// @endcond 
	};
}