#pragma once
namespace berialdraw
{
	/** The Timer widget represents a timer that can trigger periodic or one-shot events.
	Unlike regular widgets, Timer does not render visually but is associated with a window.
	It sends TimerEvent to the application when its interval expires. */
	class Timer : public Widget, public TimerStyle
	{
	public:
		/** Create timer widget attached to a window
		@param parent The window that owns this timer */
		Timer(Widget * parent);

		/** Destroy timer widget */
		virtual ~Timer();

		/** Serialize the content of widget into json */
		virtual void serialize(JsonIterator & it) override;

		/** Unserialize the content of widget from json */
		virtual void unserialize(JsonIterator & it) override;

		/** Get the style cascade mode for this widget */
		virtual StyleCascadeMode style_cascade_mode() const override;

		/** Copy all properties from another timer */
		void copy(const Timer & timer);

		/** Copy all properties from another timer */
		void copy(const Timer * timer);

		/** Start the timer with current settings */
		void start();

		/** Stop the timer */
		void stop();

		/** Check if timer is currently running */
		bool is_running() const;

		/** Get the timer identifier assigned by the system */
		uint32_t timer_id() const { return m_timer_id; }

#ifdef _DEBUG
		static void test();
		static void test1();
		static void test2();
		static void test3();
		static void test4();
#endif
	protected:
/// @cond DOXYGEN_IGNORE
		/** Remove operator = */
		Timer& operator=(const Timer& other) = delete;

		/** Timer widgets are invisible - no painting */
		virtual void paint(const Region & parent_region) override;

		/** Timer widgets have no visual area */
		virtual void place(const Area & area, bool in_layout) override;

		/** Timer widgets have no content */
		virtual Size content_size() override;

		/** Timer widgets are never hovered */
		virtual Widget * hovered(const Region & parent_region, const Point & position) override;

		uint32_t m_timer_id;  ///< System-assigned timer identifier (0 = not running)
/// @endcond
	};
};
