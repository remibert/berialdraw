#pragma once
namespace berialdraw
{
	/** The Desktop class manages all top-level windows and serves as the root context for the user interface. 
	It handles global events and coordinates interactions between multiple windows. */
	class Desktop
	{
	public:
		/** Create desktop */
		Desktop();

		/** Destroy desktop */
		~Desktop();

		/** Refreshes the screen, processes user events, and exports the rendering to an svg file if the filename is specified */
		bool dispatch(const char * output_svg_filename=0);

		/** Asks to quit the mainloop */
		void quit();

		/** Indicates if the mainloop is running */
		bool is_loop();

		/** Handles all events and waits for the call to exit the loop */
		void mainloop();

		/** Add window */
		void add(Window * window);

		/** Remove and destroy selected window */
		void remove(Window * window);

		/** Places the specified window at the top of its parents canvas stack, above all of its sibling window. */
		void back(Window * window);

		/** Places the specified window at the bottom of its parents canvas stack, below all of its sibling window.*/
		void front(Window * window);

		/** Get the front window */
		Window * front();

		/** Get the widget hovered */
		Widget * hovered(const Point & position);

		/** Remove all windows */
		void remove_all();

	protected:
/// @cond DOXYGEN_IGNORE
		/** Remove operator = */
		Desktop& operator=(const Desktop& other) = delete;
		
		/** Refresh the desktop content */
		void refresh(const char * snapshot);

		/** List of all windows */
		Vector<Window *> m_windows;

		/** Position of touch down, used to compute the shift for control scrolling */
		Point m_point;

		unsigned int m_quit:1;
		unsigned int m_already_in_dispatch:1;
/// @endcond 
	};
}
