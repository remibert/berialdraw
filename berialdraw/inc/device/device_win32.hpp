#pragma once
namespace berialdraw
{
	/** Win32 implementation of the Device class */
	class DeviceWin32 : public Device
	{
	public:
		/** Constructor
		@param title window title 
		@param width The width of the window
		@param height The height of the window */
		DeviceWin32(const char * title, Dim width=0, Dim height=0);

		/** Destructor */
		virtual ~DeviceWin32();

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

		/** Show the console */
		static void show_console();
#ifndef SWIG
	private:
/// @cond DOXYGEN_IGNORE
		void*     m_instance; /**< Handle to the instance */
		int       m_cmdShow;  /**< Command show flag */
		void*     m_hwnd;     /**< Handle to the window */
		bool      m_dirty;    /**< Dirty flag */
		Dim  m_width;    /**< Width of the window */
		Dim  m_height;   /**< Height of the window */
		uint8_t*  m_buffer;   /**< Buffer */
/// @endcond
#endif
	};

#ifndef SWIG
/// @cond DOXYGEN_IGNORE
	/** Get the current clock time
	@return The current clock time nano seconds */
	uint64_t get_clock();

	/** Get the elapsed time in milliseconds
	@param begin The start time
	@param end The end time
	@return The elapsed time in milliseconds */
	uint32_t get_elapsed_ms(uint64_t begin, uint64_t end);
/// @endcond
#endif
}
