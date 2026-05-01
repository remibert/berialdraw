#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
Clipboard::Clipboard()
{
	m_type = ClipboardType::EMPTY;
	m_data = std::make_unique<String>();
	m_provider = nullptr;
}

/** Destructor */
Clipboard::~Clipboard()
{
	// m_data and m_provider are automatically deleted via unique_ptr
}

/** Set clipboard provider for system clipboard operations */
void Clipboard::set_provider(std::unique_ptr<ClipboardProvider> provider)
{
	m_provider = std::move(provider);
	
	// If provider is set and internal clipboard is empty, try to sync from system
	if (m_provider)
	{
		if (is_empty())
		{
			String system_text;
			if (m_provider->get_text(system_text) && system_text.count() > 0)
			{
				// Update internal clipboard with system clipboard content
				m_type = ClipboardType::TEXT;
				*m_data = system_text;
			
				// Update listener to track this initial sync
				m_provider->update_listener();
			}
		}
		else
		{
			m_provider->set_text(*m_data);
			m_provider->update_listener();
		}
	}
}

/** Get clipboard provider */
ClipboardProvider * Clipboard::provider() const
{
	return m_provider.get();
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
	bool result = false;

	// Sync from system first to ensure we have the latest clipboard content
	sync_from_system();

	// Try to get from system clipboard first if provider available
	if (m_provider && !is_empty())
	{
		if (m_provider->get_text(text))
		{
			m_type = ClipboardType::TEXT;
			*m_data = text;
			result = true;
		}
	}

	if (result == false)
	{
		// Fall back to internal clipboard
		if (m_type == ClipboardType::TEXT && m_data->size() > 0)
		{
			text = *m_data;
			result = true;
		}
	}

	return result;
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
	bool result = false;
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
				result = true;
			}
		}
	}
	return result;
}
