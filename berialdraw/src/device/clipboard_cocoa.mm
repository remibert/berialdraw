#include <uuid/uuid.h>
#include "berialdraw_imp.hpp"
#include "device/clipboard_cocoa.hpp"

// Undefine the _ macro that conflicts with Cocoa headers
#ifdef _
#undef _
#endif

#import <Cocoa/Cocoa.h>

using namespace berialdraw;

/** Constructor */
berialdraw::ClipboardProviderCocoa::ClipboardProviderCocoa()
{
	@autoreleasepool
	{
		NSPasteboard * pasteboard = [NSPasteboard generalPasteboard];
		m_last_change_count = [pasteboard changeCount];
		// Initialize content cache
		get_text(m_last_content);
	}
}

/** Destructor */
berialdraw::ClipboardProviderCocoa::~ClipboardProviderCocoa()
{
}

/** Set text to clipboard */
void berialdraw::ClipboardProviderCocoa::set_text(const String & text)
{
	@autoreleasepool
	{
		NSPasteboard * pasteboard = [NSPasteboard generalPasteboard];
		[pasteboard clearContents];
		
		NSString * nsText = [NSString stringWithUTF8String:(const char*)text];
		[pasteboard setString:nsText forType:NSPasteboardTypeString];
	}
}

/** Get text from system clipboard */
bool berialdraw::ClipboardProviderCocoa::get_text(String & text) const
{
	bool success = false;

	@autoreleasepool
	{
		NSPasteboard * pasteboard = [NSPasteboard generalPasteboard];
		
		NSPasteboardType availableType = [pasteboard availableTypeFromArray:@[NSPasteboardTypeString]];
		if (availableType != nil)
		{
			NSString * nsText = [pasteboard stringForType:NSPasteboardTypeString];
			if (nsText != nil)
			{
				const char * cStr = [nsText UTF8String];
				if (cStr != nullptr)
				{
					text = cStr;
					success = true;
				}
			}
		}
	}
	
	return success;
}

/** Check if system clipboard has changed since last read */
bool berialdraw::ClipboardProviderCocoa::has_changed() const
{
	@autoreleasepool
	{
		NSPasteboard * pasteboard = [NSPasteboard generalPasteboard];
		if ([pasteboard changeCount] != m_last_change_count)
		{
			return true;
		}
		
		// Fallback: compare actual content
		String current_content;
		get_text(current_content);
		return current_content != m_last_content;
	}
}

/** Update clipboard listener */
void berialdraw::ClipboardProviderCocoa::update_listener()
{
	@autoreleasepool
	{
		NSPasteboard * pasteboard = [NSPasteboard generalPasteboard];
		m_last_change_count = [pasteboard changeCount];
		// Update content cache
		get_text(m_last_content);
	}
}
