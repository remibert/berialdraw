#pragma once

namespace berialdraw
{
	/** Cocoa implementation of ClipboardProvider */
	class ClipboardProviderCocoa : public ClipboardProvider
	{
	public:
		/** Constructor */
		ClipboardProviderCocoa();

		/** Destructor */
		virtual ~ClipboardProviderCocoa();

		/** Set text to clipboard */
		virtual void set_text(const String & text);

		/** Get text from clipboard */
		virtual bool get_text(String & text) const;

		/** Check if clipboard has changed */
		virtual bool has_changed() const;

		/** Update clipboard listener */
		virtual void update_listener();

	private:
		long m_last_change_count;	/** Track NSPasteboard change count for change detection */
		String m_last_content;		/** Cache last known content as fallback for change detection */
	};
}
