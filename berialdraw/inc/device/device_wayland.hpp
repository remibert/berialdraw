#pragma once
namespace berialdraw
{
	/** Wayland implementation of the Device class */
	class DeviceWayland : public Device
	{
	public:
		/** Constructor
		@param title window title 
		@param width The width of the window
		@param height The height of the window
		@param x The x position of the window
		@param y The y position of the window */
		DeviceWayland(const char * title, Dim width=0, Dim height=0, Coord x=0, Coord y=0);

		/** Destructor */
		virtual ~DeviceWayland();

		/** Dispatch events
		@param blocking If true, the call blocks until an event is dispatched
		@return True if an event was dispatched, false otherwise */
		virtual bool dispatch(bool blocking = true);

		/** Enter the main event loop */
		virtual void mainloop();

		/** Get the size of screen device in pixels 
		@return the size */
		virtual Size size() const;

		/** Set the size of screen device in pixels
		@param s size of screen */
		virtual void size(const Size & s);

		/** Resize the window
		@param width The new width of the window
		@param height The new height of the window */
		virtual void size(Dim width, Dim height);

		/** Get the position of the window
		@return the position as a Point */
		virtual Point position() const;

		/** Set the position of the window
		@param p position of the window */
		virtual void position(const Point & p);

		/** Move the window
		@param x The x position of the window
		@param y The y position of the window */
		virtual void position(Coord x, Coord y);

		/** Clear the window */
		virtual void clear();

		/** Copy a buffer to the window
		@param buffer The buffer to copy from
		@param width The width of the buffer
		@param height The height of the buffer */
		virtual void copy(const uint8_t* buffer = 0, Dim width = 0, Dim height = 0);

		/** Refresh the window display */
		virtual void refresh();

		/** Blit the buffer to the window */
		virtual void blit();

		/** Quit the application */
		void quit();

#ifndef SWIG
	private:
		// Wayland handles and state (to be implemented)
		void* m_display;  /**< Handle to the Wayland display */
		void* m_surface;  /**< Handle to the Wayland surface */
		void* m_buffer;   /**< Buffer */
		Dim   m_width;    /**< Width of the window */
		Dim   m_height;   /**< Height of the window */
		bool  m_dirty;    /**< Dirty flag */
#endif
	};
}
