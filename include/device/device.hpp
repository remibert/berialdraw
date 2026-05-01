#pragma once
namespace berialdraw
{
	class Event;
	class Desktop;

	/** Abstract device class to be implemented for different platforms (e.g., Win32, SDL) */
	class Device
	{
	public:
		/** Destructor */
		virtual ~Device() {}

		/** Dispatch events
		@param blocking If true, the call blocks until an event is dispatched
		@return True if an event was dispatched, false otherwise */
		virtual bool dispatch(bool blocking = true) = 0;

		/** Enter the main event loop */
		virtual void mainloop() = 0;

		/** Get the size of screen device in pixels 
		@return the size */
		virtual Size size() const = 0;

		/** Set the size of screen device in pixels
		@param s size of screen */
		virtual void size(const Size & s) = 0;

		/** Resize the window
		@param width The new width of the window
		@param height The new height of the window */
		virtual void size(Dim width, Dim height) = 0;

		/** Get the position of the window
		@return the position as a Point */
		virtual Point position() const = 0;

		/** Set the position of the window
		@param p position of the window */
		virtual void position(const Point & p) = 0;

		/** Move the window
		@param x The x position of the window
		@param y The y position of the window */
		virtual void position(Coord x, Coord y) = 0;

		/** Clear the device */
		virtual void clear() = 0;

		/** Copy a buffer to the device
		@param buffer The buffer to copy from
		@param width The width of the buffer
		@param height The height of the buffer */
		virtual void copy(const uint8_t* buffer = 0, uint32_t width = 0, uint32_t height = 0) = 0;

		/** Refresh the device display */
		virtual void refresh() = 0;
	};
}
