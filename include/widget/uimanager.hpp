#pragma once
namespace berialdraw
{
	class Window;
	class Renderer;
	class Settings;
	class TimerManager;
	class Language;

	/** The UIManager class coordinates the display and behavior of widgets. 
	It handles events, manages widget lifecycles, and ensures consistent rendering across the application. */
	class UIManager
	{
	public:
		/** Temporarily route renderer and framebuffer access to a render target.
		Scopes may be nested and must be destroyed in reverse construction order.
		This context is global and intended for the single UI rendering thread. */
		class RenderContextScope
		{
		public:
			/** Activate a renderer and its framebuffer for the current scope
			@param renderer Renderer to use while this scope is active
			@param framebuf Framebuffer written by the renderer */
			RenderContextScope(Renderer& renderer, Framebuf& framebuf);

			/** Restore the previously active render target */
			~RenderContextScope();

			/** Copying a render context scope is not supported. */
			RenderContextScope(const RenderContextScope&) = delete;

			/** Copy assignment is not supported. */
			RenderContextScope& operator=(const RenderContextScope&) = delete;

			/** Moving a render context scope is not supported. */
			RenderContextScope(RenderContextScope&&) = delete;

			/** Move assignment is not supported. */
			RenderContextScope& operator=(RenderContextScope&&) = delete;

		private:
			Renderer* m_previous_renderer;
			Framebuf* m_previous_framebuf;
		};

		/** Initialize the ui manager with device
		@param device screen device
		@param width width in pixels on device
		@param height height in pixels on device
		@param type type of framebuffer supported by the display
		@param scale rendering scale (a number less than 31 is direct, a number greater than 31 is equivalent to a floating point of divide by 64) 
		@param root_dir root directory
		@param window_mode Whether the window is fixed or resizable
		@param window_state Initial normal, maximized, or fullscreen state */
		static void init(Device * device, Dim width, Dim height, enum Framebuf::Type type,
			uint32_t scale=1, const berialdraw::String & root_dir="",
			WindowMode window_mode=WindowMode::FIXED, WindowState window_state=WindowState::NORMAL);

		/** Resize the screen framebuffer and renderer
		@param width The new width in pixels
		@param height The new height in pixels */
		static void resize(Dim width, Dim height);

		/** Deinitialize the ui manager */
		static void deinit();

		/** Start the export to SVG
		@param svg Pointer to the Exporter object */
		static void exporter(Exporter * svg);

		/** Check if export to SVG is in progress
		@return Pointer to the Exporter object if export is in progress, otherwise null pointer */
		static Exporter * exporter();

		/** Return the active framebuffer.
		@return Current render-context framebuffer, or the UI framebuffer */
		static Framebuf * framebuf();

		/** Return the event notifier.
		@return UI notifier, or null before initialization */
		static Notifier * notifier();

		/** Return the active renderer.
		@return Current render-context renderer, or the UI renderer */
		static Renderer * renderer();

		/** Return the screen device.
		@return Device owned by the UI manager */
		static Device * device();

		/** Return the fonts manager.
		@return Font manager owned by the UI manager */
		static Fonts * fonts();

		/** Return the invalidation manager.
		@return Invalidator owned by the UI manager */
		static Invalidator * invalidator();

		/** Return the styles manager.
		@return Style manager owned by the UI manager */
		static Styles * styles();

		/** Return the colors manager.
		@return Color manager owned by the UI manager */
		static Colors * colors();

		/** Return the desktop widget.
		@return Desktop owned by the UI manager */
		static Desktop * desktop();

		/** Return the screen CRC manager.
		@return CRC manager owned by the UI manager */
		static ScreenCrc * screen_crc();

		/** Return the arc cache.
		@return Arc cache owned by the UI manager */
		static ArcCache * arc_cache();

		/** Return the image cache.
		@return Image cache owned by the UI manager */
		static ImageCache * image_cache();

		/** Return the settings manager.
		@return Settings owned by the UI manager */
		static Settings * settings();

		/** Return the language catalog manager.
		@details The manager is created during init() and destroyed during deinit().
		@return Language manager, or null before initialization */
		static Language * language();

		/** Return the clipboard manager.
		@return Clipboard owned by the UI manager */
		static Clipboard * clipboard();

		/** Return the timer manager.
		@return Timer manager owned by the UI manager */
		static TimerManager * timer_manager();

		/** Check whether the UI manager is initialized. */
		static bool is_initialized();

		/** Check whether UIManager::deinit() is in progress. */
		static bool is_shutting_down();
	protected:
/// @cond DOXYGEN_IGNORE
		/** Remove operator = */
		UIManager& operator=(const UIManager& other) = delete;
		static bool          m_initialized;
		static bool          m_is_shutting_down;
		static Device      * m_device;
		static std::unique_ptr<Renderer>    m_renderer;
		static std::unique_ptr<Framebuf>    m_framebuf;
		static Renderer*                    m_context_renderer;
		static Framebuf*                    m_context_framebuf;
		static Exporter    * m_exporter;
		static std::unique_ptr<Notifier>    m_notifier;
		static std::unique_ptr<Invalidator> m_invalidator;
		static std::unique_ptr<Styles>      m_styles;
		static std::unique_ptr<Colors>      m_colors;
		static std::unique_ptr<Fonts>       m_fonts;
		static std::unique_ptr<Desktop>     m_desktop;
		static std::unique_ptr<ScreenCrc>   m_screen_crc;
		static std::unique_ptr<ArcCache>    m_arc_cache;
		static std::unique_ptr<Settings>    m_settings;
		static std::unique_ptr<Language>    m_language;
		static std::unique_ptr<Clipboard>   m_clipboard;
		static std::unique_ptr<ImageCache>  m_image_cache;	static std::unique_ptr<TimerManager> m_timer_manager;/// @endcond
	};
}
