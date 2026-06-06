#pragma once

namespace berialdraw
{
	/** XCB (X11) implementation of ClipboardProvider */
	class ClipboardProviderXcb : public ClipboardProvider
	{
	public:
		/** Constructor
		@param connection XCB connection pointer
		@param window XCB window ID */
		ClipboardProviderXcb(void* connection, uint32_t window);

		/** Destructor */
		virtual ~ClipboardProviderXcb();

		/** Set text to clipboard */
		virtual void set_text(const String & text);

		/** Get text from clipboard */
		virtual bool get_text(String & text) const;

		/** Check if clipboard has changed */
		virtual bool has_changed() const;

		/** Update clipboard listener */
		virtual void update_listener();

	private:
		void*    m_connection;         /**< XCB connection */
		uint32_t m_window;             /**< XCB window ID */
		uint32_t m_clipboard_atom;     /**< CLIPBOARD atom */
		uint32_t m_utf8_string_atom;   /**< UTF8_STRING atom */
		uint32_t m_targets_atom;       /**< TARGETS atom */
		uint32_t m_property_atom;      /**< Property atom for data transfer */
		String   m_last_content;       /**< Cache last clipboard content for change detection */
		String   m_owned_content;      /**< Content we own (when we're the clipboard owner) */
		bool     m_owns_clipboard;     /**< Whether we own the clipboard */
	};
}
