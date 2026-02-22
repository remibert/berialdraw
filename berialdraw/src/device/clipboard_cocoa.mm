#include <uuid/uuid.h>
#include "berialdraw_imp.hpp"
#include "device/clipboard_cocoa.hpp"

// Undefine the _ macro that conflicts with Cocoa headers
#ifdef _
#undef _
#endif

#import <Cocoa/Cocoa.h>

using namespace berialdraw;

// Initialize static members
void* ClipboardProviderCocoa::m_accepted_types = nullptr;
int ClipboardProviderCocoa::m_instance_count = 0;

/** Static method to safely initialize and get accepted types */
void* ClipboardProviderCocoa::getAcceptedTypes()
{
	@synchronized([NSObject class])
	{
		if (m_accepted_types == nullptr) {
			m_accepted_types = (void*)CFBridgingRetain(@[NSPasteboardTypeString]);
		}
	}
	return ClipboardProviderCocoa::m_accepted_types;
}

/** Clean up accepted types when no more instances exist */
void ClipboardProviderCocoa::cleanupAcceptedTypes()
{
	@synchronized([NSObject class])
	{
		if (--m_instance_count <= 0 && m_accepted_types != nullptr) {
			CFBridgingRelease(m_accepted_types);
			m_accepted_types = nullptr;
		}
	}
}

/** Constructor */
berialdraw::ClipboardProviderCocoa::ClipboardProviderCocoa()
{
	@autoreleasepool
	{
		@synchronized([NSObject class])
		{
			m_instance_count++;
		}
		// Ensure accepted types are initialized
		getAcceptedTypes();
		NSPasteboard * pasteboard = [NSPasteboard generalPasteboard];
		m_last_change_count = [pasteboard changeCount];
		// Initialize content cache
		get_text(m_last_content);
	}
}

/** Destructor */
berialdraw::ClipboardProviderCocoa::~ClipboardProviderCocoa()
{
	cleanupAcceptedTypes();
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
		NSArray * acceptedTypes = (__bridge NSArray*)getAcceptedTypes();
		NSPasteboardType availableType = [pasteboard availableTypeFromArray:acceptedTypes];
		if (availableType != nil)
		{
			NSString * nsText = [pasteboard stringForType:NSPasteboardTypeString];
			if (nsText != nil)
			{
				const char * cStr = [nsText UTF8String];
				if (cStr != nullptr)
				{
					text = String(cStr);
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
	bool changed = false;
	
	@autoreleasepool
	{
		NSPasteboard * pasteboard = [NSPasteboard generalPasteboard];
		NSInteger current_count = [pasteboard changeCount];
		
		// Quick check: if count changed, clipboard changed
		if (current_count != m_last_change_count)
		{
			changed = true;
		}
		else
		{
			// Fallback: compare actual content only if count hasn't changed
			String current_content;
			get_text(current_content);
			changed = (current_content != m_last_content);
		}
	}
	
	return changed;
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
