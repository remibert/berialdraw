#include "berialdraw_imp.hpp"
#include "device/clipboard_win32.hpp"
#include <windows.h>

using namespace berialdraw;

/** Constructor */
berialdraw::ClipboardProviderWin32::ClipboardProviderWin32()
{
	m_last_sequence_number = GetClipboardSequenceNumber();
	// Initialize content cache
	get_text(m_last_content);
}

/** Destructor */
berialdraw::ClipboardProviderWin32::~ClipboardProviderWin32()
{
}

/** Set text to clipboard */
void berialdraw::ClipboardProviderWin32::set_text(const String & text)
{
	if (!OpenClipboard(nullptr))
	{
		return;
	}

	EmptyClipboard();

	// Convert string to UTF-16 for Windows
	const char * utf8_str = (const char *)text;
	int length = MultiByteToWideChar(CP_UTF8, 0, utf8_str, -1, nullptr, 0);
	
	if (length > 0)
	{
		// Allocate memory for clipboard (Unicode)
		HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, length * sizeof(wchar_t));
		if (hMem != nullptr)
		{
			wchar_t * pMem = (wchar_t*)GlobalLock(hMem);
			if (pMem != nullptr)
			{
				MultiByteToWideChar(CP_UTF8, 0, utf8_str, -1, pMem, length);
				GlobalUnlock(hMem);
				
				// Set Unicode version
				HANDLE hResult = SetClipboardData(CF_UNICODETEXT, hMem);
				if (hResult == nullptr)
				{
					GlobalFree(hMem);
				}
			}
			else
			{
				GlobalFree(hMem);
			}
		}
	}

	CloseClipboard();
}

/** Get text from clipboard */
bool berialdraw::ClipboardProviderWin32::get_text(String & text) const
{
	bool success = false;

	if (OpenClipboard(nullptr))
	{
		HANDLE hData = GetClipboardData(CF_UNICODETEXT);
		if (hData != nullptr)
		{
			wchar_t * pszText = (wchar_t*)GlobalLock(hData);
			if (pszText != nullptr)
			{
				// Convert from UTF-16 to UTF-8
				int length = WideCharToMultiByte(CP_UTF8, 0, pszText, -1, nullptr, 0, nullptr, nullptr);
				if (length > 0)
				{
					char * utf8_buffer = new char[length];
					WideCharToMultiByte(CP_UTF8, 0, pszText, -1, utf8_buffer, length, nullptr, nullptr);
					text = utf8_buffer;
					delete[] utf8_buffer;
					success = true;
				}
				GlobalUnlock(hData);
			}
		}
		CloseClipboard();
	}

	return success;
}

/** Check if clipboard has changed */
bool berialdraw::ClipboardProviderWin32::has_changed() const
{
	// Check sequence number first (faster)
	DWORD current_sequence = GetClipboardSequenceNumber();
	if (current_sequence != m_last_sequence_number)
	{
		return true;
	}
	
	// Fallback: compare actual content to catch missed updates
	String current_content;
	get_text(current_content);
	return current_content != m_last_content;
}

/** Update clipboard listener */
void berialdraw::ClipboardProviderWin32::update_listener()
{
	// Update the last known sequence number
	m_last_sequence_number = GetClipboardSequenceNumber();
	// Update content cache
	get_text(m_last_content);
}
