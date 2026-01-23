#pragma once
#include <SDL3/SDL.h> 

namespace berialdraw
{
	/** SDL implementation of the Device class */
	class DeviceSdl : public Device
	{
	public:
		/** Constructor
		@param title window title
		@param width The width of the window
		@param height The height of the window
		@param x The x position of the window
		@param y The y position of the window */
		DeviceSdl(const char * title, Dim width=0, Dim height=0, Coord x=0, Coord y=0);

		/** Destructor */
		virtual ~DeviceSdl();

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
	private:
/// @cond DOXYGEN_IGNORE
		void open_window();
		void close_window();
		/** Adapt the ratio of coordinates */
		void adapt_ratio(Point & position);

		/** Convert sdl key sim into berialdraw key */
		wchar_t get_key(SDL_Scancode sim);

		/** Get key modifier (Ctrl,Shift or Alt)*/
		KeyEvent::Modifier get_modifier();

		SDL_Window*   m_window=0;   /**< SDL window */
		SDL_Renderer* m_renderer=0; /**< SDL renderer */
		SDL_Event     m_event;    /**< SDL event */
		SDL_Texture*  m_texture=0;  /**< SDL texture */
		bool          m_dirty;    /**< Dirty flag */
		Dim      m_width;    /**< Width of the window */
		Dim      m_height;   /**< Height of the window */
		uint8_t*      m_buffer=0;   /**< Buffer */
		uint8_t       m_pixel_ratio;
		String        m_title;
		bool          m_mouse_down=false; /**< Track mouse button state for capture */
/// @endcond
	};

/// @cond DOXYGEN_IGNORE
	/** Get the current clock time
	@return The current clock time nano second */
	uint64_t get_clock();

	/** Get the elapsed time in milliseconds
	@param begin The start time
	@param end The end time
	@return The elapsed time in milliseconds */
	uint32_t get_elapsed_ms(uint64_t begin, uint64_t end);
/// @endcond
}
