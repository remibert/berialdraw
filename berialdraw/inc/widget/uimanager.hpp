#ifndef UIManager_hpp_INCLUDED
#define UIManager_hpp_INCLUDED
namespace berialdraw
{
	class Window;
	class Renderer;

	/** The UIManager class coordinates the display and behavior of widgets. 
	It handles events, manages widget lifecycles, and ensures consistent rendering across the application. */
	class UIManager
	{
	public:
		/** Initialize the ui manager with device
		@param device screen device
		@param width width in pixels on device
		@param height height in pixels on device
		@param type type of framebuffer supported by the display
		@param scale rendering scale (a number less than 10 is direct, a number greater than 10 is equivalent to a floating point of divide by 64) */
		static void init(Device * device, Dim width, Dim height, enum Framebuf::Type type, uint32_t scale=1);

		/** Deinitialize the ui manager */
		static void deinit();

		/** Start the export to SVG
		@param svg Pointer to the Exporter object */
		static void exporter(Exporter * svg);

		/** Check if export to SVG is in progress
		@return Pointer to the Exporter object if export is in progress, otherwise null pointer */
		static Exporter * exporter();

		/** Return the pointer on internal framebuf */
		static Framebuf * framebuf();

		/** Return the notifier */
		static Notifier * notifier();

		/** Return the renderer */
		static Renderer * renderer();

		/** Return the screen device */
		static Device * device();

		/** Return the fonts manager */
		static Fonts * fonts();

		/** Return the dirty manager */
		static Invalidator * invalidator();

		/** Return the styles manager */
		static Styles * styles();

		/** Return the colors manager */
		static Colors * colors();

		/** Return the desktop */
		static Desktop * desktop();

		/** Add screen crc */
		static ScreenCrc * screen_crc();

		/** Indicates if the uimanager is initialized or not */
		static bool is_initialized();

	protected:
/// @cond DOXYGEN_IGNORE
		static bool          m_initialized;
		static Device      * m_device;
		static Renderer    * m_renderer;
		static Framebuf    * m_framebuf;
		static Exporter    * m_exporter;
		static Notifier    * m_notifier;
		static Invalidator * m_invalidator;
		static Styles      * m_styles;
		static Colors      * m_colors;
		static Fonts       * m_fonts;
		static Desktop     * m_desktop;
		static ScreenCrc   * m_screen_crc;
/// @endcond
	};
}
#endif