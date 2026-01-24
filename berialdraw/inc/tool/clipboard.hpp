#pragma once
namespace berialdraw
{
	class ClipboardProvider;

	/** Clipboard data types */
	enum class ClipboardType
	{
		TEXT,	/** Text data type */
		EMPTY	/** Empty clipboard */
	};

	/** Clipboard management for text and future media types */
	class Clipboard
	{
	public:
		/** Constructor */
		Clipboard();

		/** Destructor */
		virtual ~Clipboard();

		/** Set clipboard provider for system clipboard operations
		@param provider Pointer to ClipboardProvider implementation (device)
		@note Takes ownership of the pointer */
		void set_provider(ClipboardProvider * provider);

		/** Get clipboard provider
		@return Pointer to ClipboardProvider or nullptr */
		ClipboardProvider * provider() const;

		/** Copy text to internal clipboard
		@param text Text to copy */
		void copy_text(const String & text);

		/** Cut text from internal clipboard (copy then clear)
		@param text Text to cut */
		void cut_text(const String & text);

		/** Paste text from internal clipboard
		@param text Output parameter containing pasted text
		@return True if clipboard contains text, false if empty */
		bool paste_text(String & text);

		/** Get clipboard content type
		@return Clipboard type */
		ClipboardType type() const;

		/** Clear clipboard content */
		void clear();

		/** Check if clipboard is empty
		@return True if empty */
		bool is_empty() const;

		/** Get/Set text content of clipboard
		@overload Get text content from clipboard
		@return Const reference to text (may be empty if clipboard is empty) */
		const String & text() const;

		/** Set text content of clipboard
		@param str Text to set in clipboard */
		void text(const String & str);

		/** Synchronize clipboard from system (for external changes)
		@return True if clipboard was updated from system, false if no change
		@note Call this periodically to detect external clipboard changes */
		bool sync_from_system();

	protected:
/// @cond DOXYGEN_IGNORE
		ClipboardType m_type;
		String * m_data;
		ClipboardProvider * m_provider;
/// @endcond
	};

	/** Abstract interface for device to provide system clipboard access */
	class ClipboardProvider
	{
	public:
		/** Destructor */
		virtual ~ClipboardProvider() {}

		/** Set text to clipboard
		@param text Text to set in clipboard */
		virtual void set_text(const String & text) = 0;

		/** Get text from clipboard
		@param text Output parameter containing clipboard text
		@return True if clipboard contains text, false otherwise */
		virtual bool get_text(String & text) const = 0;

		/** Check if clipboard has changed since last read
		@return True if clipboard content has changed, false otherwise
		@note Used for synchronization with external clipboard changes */
		virtual bool has_changed() const { return false; }

		/** Update clipboard listener (called periodically)
		@note Default implementation does nothing - override for platforms that support it */
		virtual void update_listener() {}
	};
}
