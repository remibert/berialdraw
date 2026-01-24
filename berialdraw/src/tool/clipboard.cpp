#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
Clipboard::Clipboard()
{
	m_type = ClipboardType::EMPTY;
	m_data = new String();
	m_provider = nullptr;
}

/** Destructor */
Clipboard::~Clipboard()
{
	delete m_data;
	// Don't delete m_provider - it's managed by the device
	m_provider = nullptr;
}

/** Set clipboard provider for system clipboard operations */
void Clipboard::set_provider(ClipboardProvider * provider)
{
	m_provider = provider;
}

/** Get clipboard provider */
ClipboardProvider * Clipboard::provider() const
{
	return m_provider;
}

/** Copy text to internal clipboard */
void Clipboard::copy_text(const String & text)
{
	m_type = ClipboardType::TEXT;
	*m_data = text;

	// Sync with system clipboard if provider available
	if (m_provider)
	{
		m_provider->set_text(text);
		// Update the listener so we don't detect our own write as a change
		m_provider->update_listener();
	}
}

/** Cut text from internal clipboard */
void Clipboard::cut_text(const String & text)
{
	copy_text(text);
}

/** Paste text from internal clipboard */
bool Clipboard::paste_text(String & text)
{
	// Sync from system first to ensure we have the latest clipboard content
	sync_from_system();

	// Try to get from system clipboard first if provider available
	if (m_provider)
	{
		if (m_provider->get_text(text))
		{
			m_type = ClipboardType::TEXT;
			*m_data = text;
			return true;
		}
	}

	// Fall back to internal clipboard
	if (m_type == ClipboardType::TEXT && m_data->size() > 0)
	{
		text = *m_data;
		return true;
	}

	return false;
}

/** Get clipboard content type */
ClipboardType Clipboard::type() const
{
	return m_type;
}

/** Clear clipboard content */
void Clipboard::clear()
{
	m_type = ClipboardType::EMPTY;
	m_data->clear();
}

/** Check if clipboard is empty */
bool Clipboard::is_empty() const
{
	return m_type == ClipboardType::EMPTY || m_data->size() == 0;
}

/** Get text content from clipboard */
const String & Clipboard::text() const
{
	return *m_data;
}

/** Set text content of clipboard */
void Clipboard::text(const String & str)
{
	m_type = ClipboardType::TEXT;
	*m_data = str;

	// Sync with system clipboard if provider available
	if (m_provider)
	{
		m_provider->set_text(str);
		m_provider->update_listener();
	}
}

/** Synchronize clipboard from system (for external changes) */
bool Clipboard::sync_from_system()
{
	// Check if system clipboard has changed
	if (m_provider)
	{
		if (m_provider->has_changed())
		{
			String system_text;
			if (m_provider->get_text(system_text))
			{
				// System clipboard content has changed, update internal clipboard
				m_type = ClipboardType::TEXT;
				*m_data = system_text;
				
				// Update listener AFTER we've successfully synced
				m_provider->update_listener();
				return true;
			}
		}
	}

	return false;
}
