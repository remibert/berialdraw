#import <Cocoa/Cocoa.h>
#import <CoreGraphics/CoreGraphics.h>

#include "berialdraw.hpp"
#include "device/device_cocoa.hpp"

using namespace berialdraw;

// Forward declare
@class BerialView;

// PIMPL Implementation class - contains all Cocoa-specific code
class berialdraw::DeviceCocoaImpl
{
public:
	NSWindow*     m_window;
	void*         m_view;        // BerialView* stored as void*
	NSEvent*      m_event;
	bool          m_dirty;
	Dim           m_width;
	Dim           m_height;
	uint8_t*      m_buffer;
	CGFloat       m_scale_factor;
	String        m_title;
	bool          m_app_initialized;

	DeviceCocoaImpl(const char* title, Dim width, Dim height);
	~DeviceCocoaImpl();
	bool initialize_app();
	void open_window();
	void close_window();
	void clear();
	bool dispatch(bool blocking);
	void copy(const uint8_t* buffer, Dim width, Dim height);
	void blit();
	void refresh();
	void size(Dim width, Dim height);
	berialdraw::Size size() const;
	void quit();
};

// Custom view class for handling Cocoa events
@interface BerialView : NSView<NSWindowDelegate>
@property (nonatomic, assign) berialdraw::DeviceCocoaImpl* device;
@property (nonatomic, assign) uint8_t* buffer;
@property (nonatomic, assign) Dim bufferWidth;
@property (nonatomic, assign) Dim bufferHeight;
@property (nonatomic, assign) CGFloat scaleFactor;
@property (nonatomic, assign) BOOL mouseDown;
@property (nonatomic, assign) BOOL appStarted;
- (void)updateBuffer:(uint8_t*)buffer width:(Dim)width height:(Dim)height;
- (void)keyDown:(NSEvent *)event;
- (void)keyUp:(NSEvent *)event;
- (wchar_t)convertKeyFromEvent:(NSEvent*)event;
- (KeyEvent::Modifier)getModifierFromEvent:(NSEvent*)event;
// Private helper methods
- (berialdraw::Point)getMousePositionFromEvent:(NSEvent *)event;
- (void)sendMouseEventWithType:(int)touchType position:(berialdraw::Point)position;
- (void)sendKeyEventWithKey:(wchar_t)key keyEventType:(int)keyEventType modifier:(berialdraw::KeyEvent::Modifier)modifier;
@end

@implementation BerialView

// Initialize the view with tracking area for mouse events
- (instancetype)initWithFrame:(NSRect)frameRect
{
	self = [super initWithFrame:frameRect];
	if (self)
	{
		self.scaleFactor = 1.0;
		self.buffer = nullptr;
		self.bufferWidth = 0;
		self.bufferHeight = 0;
		self.mouseDown = NO;
		self.appStarted = NO;

		// Enable mouse tracking for all mouse events in this view
		NSTrackingAreaOptions options = NSTrackingMouseEnteredAndExited |
										NSTrackingActiveAlways |
										NSTrackingMouseMoved |
										NSTrackingInVisibleRect;
		
		// Create tracking area with __strong attribute to manage ownership properly under ARC
		__strong NSTrackingArea *trackingArea = [[NSTrackingArea alloc] initWithRect:NSZeroRect
																	options:options
																		owner:self
																	userInfo:nil];
		[self addTrackingArea:trackingArea];
		 // ARC will automatically release trackingArea when it goes out of scope
	}
	return self;
}

// Declare that this view can become first responder for keyboard events
- (BOOL)acceptsFirstResponder
{
	return YES;
}

// Handle window close event by sending exit key event
- (void)windowWillClose:(NSNotification *)notification
{
	if (self.device && self.appStarted)
	{
		if (UIManager::notifier())
		{
			UIManager::notifier()->push_event(new KeyEvent((wchar_t)ReservedKey::KEY_EXIT, KeyEvent::KEY_DOWN));
		}
		self.device->quit();
	}
}

// Update the buffer and request redraw
- (void)updateBuffer:(uint8_t*)buffer width:(Dim)width height:(Dim)height
{
	self.buffer = buffer;
	self.bufferWidth = width;
	self.bufferHeight = height;
	[self setNeedsDisplay:YES];
}

// Draw the buffer content to the view
- (void)drawRect:(NSRect)dirtyRect
{
	// Check if buffer is valid
	if (!self.buffer || self.bufferWidth == 0 || self.bufferHeight == 0)
	{
		[[NSColor whiteColor] setFill];
		NSRectFill(dirtyRect);
	}
	else
	{
		// Get the graphics context for drawing
		CGContextRef context = [[NSGraphicsContext currentContext] CGContext];
		if (context)
		{
			NSRect bounds = [self bounds];
			CGFloat viewWidth = bounds.size.width;
			CGFloat viewHeight = bounds.size.height;
            
			// Create a data provider from the buffer
			// Use CGDataProviderCreateWithData with proper cleanup callback
			CGDataProviderRef provider = CGDataProviderCreateWithData(
				NULL,
				self.buffer,
				self.bufferWidth * self.bufferHeight * 3,
				NULL  // No cleanup callback needed since buffer is managed elsewhere
			);
			
			if (provider)
			{
				// Create RGB color space
				CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
				if (colorSpace)
				{
					// Create image from buffer data
					CGImageRef image = CGImageCreate(
						self.bufferWidth,
						self.bufferHeight,
						8,  // bits per component
						24,  // bits per pixel
						self.bufferWidth * 3,  // bytes per row
						colorSpace,
						kCGBitmapByteOrderDefault,
						provider,
						NULL,  // decode array
						false,  // should interpolate
						kCGRenderingIntentDefault
					);
                    
					if (image)
					{
						// Draw image scaled to view size
						CGRect drawRect = CGRectMake(0, 0, viewWidth, viewHeight);
						CGContextDrawImage(context, drawRect, image);
						CGImageRelease(image);
					}
					// Always release colorSpace, even if CGImageCreate fails
					CGColorSpaceRelease(colorSpace);
				}
				// Always release provider, even if colorSpace creation fails
				CGDataProviderRelease(provider);
			}
		}
	}
}

// Handle mouse down event
- (void)mouseDown:(NSEvent *)event
{
	if (self.device)
	{
		self.mouseDown = YES;
		berialdraw::Point position = [self getMousePositionFromEvent:event];
		[self sendMouseEventWithType:TouchEvent::TOUCH_DOWN position:position];
	}
}

// Handle mouse up event
- (void)mouseUp:(NSEvent *)event
{
	if (self.device)
	{
		self.mouseDown = NO;
		berialdraw::Point position = [self getMousePositionFromEvent:event];
		[self sendMouseEventWithType:TouchEvent::TOUCH_UP position:position];
	}
}

// Handle mouse move event (when button is not pressed)
- (void)mouseMoved:(NSEvent *)event
{
	if (!self.device || self.mouseDown)
	{
		// Ignore if device not ready or button is already pressed
	}
	else
	{
		berialdraw::Point position = [self getMousePositionFromEvent:event];
		[self sendMouseEventWithType:TouchEvent::TOUCH_MOVE position:position];
	}
}

// Handle mouse drag event (when button is pressed and moving)
- (void)mouseDragged:(NSEvent *)event
{
	if (!self.device || !self.mouseDown)
	{
		// Ignore if device not ready or button is not pressed
	}
	else
	{
		berialdraw::Point position = [self getMousePositionFromEvent:event];
		[self sendMouseEventWithType:TouchEvent::TOUCH_MOVE position:position];
	}
}

// Helper method: Extract mouse position from event and convert to buffer coordinates
- (berialdraw::Point)getMousePositionFromEvent:(NSEvent *)event
{
	// Convert event location to view coordinates
	NSPoint location = [self convertPoint:event.locationInWindow fromView:nil];
	location.y = self.bounds.size.height - location.y;
    
	// Calculate scaling factors for coordinate conversion
	float scaleX = (float)self.bufferWidth / self.bounds.size.width;
	float scaleY = (float)self.bufferHeight / self.bounds.size.height;

	// Create and return position with proper scaling
	berialdraw::Point position(location.x * scaleX, location.y * scaleY);
	position.adapt_scale();

	return position;
}

// Helper method: Send a mouse/touch event to the notifier
- (void)sendMouseEventWithType:(int)touchType position:(berialdraw::Point)position
{
	if (UIManager::notifier())
	{
		// Send the touch event and refresh display
		UIManager::notifier()->push_event(new TouchEvent(position, (TouchEvent::TouchState)touchType));
		self.device->refresh();
		self.device->blit();
	}
}

// Handle mouse exit event (when cursor leaves the view while button pressed)
- (void)mouseExited:(NSEvent *)event
{
	// If mouse exits with button down, force a touch up event
	if (self.mouseDown && self.device && UIManager::notifier())
	{
		berialdraw::Point position = [self getMousePositionFromEvent:event];
		[self sendMouseEventWithType:TouchEvent::TOUCH_UP position:position];
		self.mouseDown = NO;
	}
}

// Handle key down event
- (void)keyDown:(NSEvent *)event
{
	if (self.device && UIManager::notifier())
	{
		wchar_t key = [self convertKeyFromEvent:event];
		KeyEvent::Modifier modifier = [self getModifierFromEvent:event];
		[self sendKeyEventWithKey:key keyEventType:KeyEvent::KEY_DOWN modifier:modifier];
	}
}

// Handle key up event
- (void)keyUp:(NSEvent *)event
{
	if (self.device && UIManager::notifier())
	{
		wchar_t key = [self convertKeyFromEvent:event];
		KeyEvent::Modifier modifier = [self getModifierFromEvent:event];
		[self sendKeyEventWithKey:key keyEventType:KeyEvent::KEY_UP modifier:modifier];
	}
}

// Convert key code to special key constant
- (wchar_t)convertKeyFromEvent:(NSEvent*)event
{
	wchar_t result = 0;

	switch(event.keyCode)
	{
	case 36:
		result = (wchar_t)ReservedKey::KEY_ENTER;
		break;
	case 53:
		result = (wchar_t)ReservedKey::KEY_ESCAPE;
		break;
	case 51:
		result = (wchar_t)ReservedKey::KEY_BACKSPACE;
		break;
	case 117:
		result = (wchar_t)ReservedKey::KEY_DELETE;
		break;
	case 124:
		result = (wchar_t)ReservedKey::KEY_RIGHT;
		break;
	case 123:
		result = (wchar_t)ReservedKey::KEY_LEFT;
		break;
	case 125:
		result = (wchar_t)ReservedKey::KEY_DOWN;
		break;
	case 126:
		result = (wchar_t)ReservedKey::KEY_UP;
		break;
	case 115:
		result = (wchar_t)ReservedKey::KEY_HOME;
		break;
	case 119:
		result = (wchar_t)ReservedKey::KEY_END;
		break;
	case 49:
		result = ' ';  // Space
		break;
	case 48:
		result = '\t'; // Tab
		break;
	default:
		result = 0;
		break;
	}

	return result;
}

// Extract modifier keys from event
- (KeyEvent::Modifier)getModifierFromEvent:(NSEvent*)event
{
	KeyEvent::Modifier modifier = KeyEvent::MODIFIER_NONE;
	NSEventModifierFlags flags = event.modifierFlags;
    
	if (flags & NSEventModifierFlagShift)
	{
		modifier = (KeyEvent::Modifier)(modifier | KeyEvent::MODIFIER_SHIFT);
	}
    
	if (flags & NSEventModifierFlagControl)
	{
		modifier = (KeyEvent::Modifier)(modifier | KeyEvent::MODIFIER_CONTROL);
	}
    
	if (flags & NSEventModifierFlagOption)
	{
		modifier = (KeyEvent::Modifier)(modifier | KeyEvent::MODIFIER_ALT);
	}
    
	if (flags & NSEventModifierFlagCommand)
	{
		modifier = (KeyEvent::Modifier)(modifier | KeyEvent::MODIFIER_CONTROL);
	}
    
	return modifier;
}

// Helper method: Send a key event with proper key type handling
- (void)sendKeyEventWithKey:(wchar_t)key keyEventType:(int)keyEventType modifier:(berialdraw::KeyEvent::Modifier)modifier
{
	if (UIManager::notifier())
	{
		if (key)
		{
			// If it's a special key (arrow, enter, etc.), send with modifier
			UIManager::notifier()->push_event(new KeyEvent(key, (KeyEvent::KeyState)keyEventType, modifier));
		}
		else
		{
			// If not a special key, check for printable character
			NSEvent *event = [NSApplication sharedApplication].currentEvent;
			if (event)
			{
				NSString *chars = [event characters];
				if (chars && chars.length > 0)
				{
					unichar keyChar = [chars characterAtIndex:0];

					// Only send event for printable characters (ASCII 32 and above)
					if (keyChar >= 32)
					{
						UIManager::notifier()->push_event(new KeyEvent(keyChar, (KeyEvent::KeyState)keyEventType, KeyEvent::MODIFIER_NONE));
					}
				}
			}
		}
	}
}

@end

// DeviceCocoaImpl method implementations
berialdraw::DeviceCocoaImpl::DeviceCocoaImpl(const char* title, Dim width, Dim height)
	: m_window(nullptr)
	, m_view(nullptr)
	, m_event(nullptr)
	, m_dirty(false)
	, m_width(width)
	, m_height(height)
	, m_buffer(nullptr)
	, m_scale_factor(1.0)
	, m_title(title)
	, m_app_initialized(false)
{
	if (initialize_app())
	{
		open_window();
	}
}

berialdraw::DeviceCocoaImpl::~DeviceCocoaImpl()
{
	close_window();
}

// Initialize the NSApplication if not already initialized
bool berialdraw::DeviceCocoaImpl::initialize_app()
{
	bool result = false;

	MemoryLeakTracer::suspend();

	if (![NSThread isMainThread])
	{
		NSLog(@"Warning: DeviceCocoa should be created on the main thread");
	}

	if (!NSApp)
	{
		[NSApplication sharedApplication];
		
		if (!NSApp)
		{
			NSLog(@"Failed to create NSApplication");
		}
	}
    
	if (NSApp)
	{
		if ([NSApp activationPolicy] == NSApplicationActivationPolicyProhibited)
		{
			[NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
		}

		// CRITICAL: Force application activation on ARM64/M1
		[NSApp finishLaunching];
		[NSApp activateIgnoringOtherApps:YES];

		m_app_initialized = true;
		result = true;
	}

	MemoryLeakTracer::resume();

	return result;
}

// Create and configure the window on the main thread
void berialdraw::DeviceCocoaImpl::open_window()
{
	MemoryLeakTracer::suspend();

	if (m_app_initialized)
	{
		// Allocate and clear buffer with proper ARM64 alignment
		size_t buffer_size = m_width * m_height * 3;
		// Ensure 16-byte alignment for ARM64 SIMD operations
		posix_memalign((void**)&m_buffer, 16, ((buffer_size + 15) / 16) * 16);
		memset(m_buffer, 0xff, buffer_size);
        
		if ([NSThread isMainThread])
		{
			// Get display scale factor with safety check for ARM64
			m_scale_factor = [[NSScreen mainScreen] backingScaleFactor];
			if (m_scale_factor <= 0.0 || !isfinite(m_scale_factor))
			{
				NSLog(@"Warning: Invalid scale factor %.2f, using 1.0", m_scale_factor);
				m_scale_factor = 1.0; // Fallback for ARM64 edge cases
			}

			// Ensure minimum window size to avoid division issues
			CGFloat display_width = fmax(m_width / m_scale_factor, 100.0);
			CGFloat display_height = fmax(m_height / m_scale_factor, 100.0);
			NSRect frame = NSMakeRect(100, 100, display_width, display_height);

			// Create window with title bar, close, and minimize buttons
			m_window = [[NSWindow alloc] initWithContentRect:frame
												styleMask:(NSWindowStyleMaskTitled | 
															NSWindowStyleMaskClosable | 
															NSWindowStyleMaskMiniaturizable)
													backing:NSBackingStoreBuffered
													defer:NO];

			if (m_window)
			{
				[m_window setTitle:[NSString stringWithUTF8String:m_title.c_str()]];
				
				// Position window in top-left corner respecting menu bar and dock
				NSRect screenFrame = [[NSScreen mainScreen] visibleFrame];
				CGFloat x = screenFrame.origin.x;
				CGFloat y = NSMaxY(screenFrame) - display_height;
				[m_window setFrameOrigin:NSMakePoint(x, y)];

				// Create and configure the view
				BerialView* view = [[BerialView alloc] initWithFrame:frame];
				[view setDevice:this];
				m_view = (__bridge void*)view;

				[m_window setContentView:view];
				[m_window setDelegate:view];

				// CRITICAL: Force window visibility on ARM64/M1
				[m_window makeKeyAndOrderFront:nil];
				[m_window orderFrontRegardless];
				[NSApp activateIgnoringOtherApps:YES];

				// Update scale factor from window with validation
				CGFloat window_scale = [m_window backingScaleFactor];
				if (window_scale > 0.0 && isfinite(window_scale)) {
					m_scale_factor = window_scale;
				}
				[view setScaleFactor:m_scale_factor];
			}
			else
			{
				NSLog(@"Failed to create NSWindow");
			}
		}
		else
		{
			// Use async dispatch instead of sync to avoid deadlocks on ARM64
			__block BOOL window_created = NO;
			__block NSException* creation_exception = nil;

			dispatch_async(dispatch_get_main_queue(), ^{
				@try {
					m_scale_factor = [[NSScreen mainScreen] backingScaleFactor];
					if (m_scale_factor <= 0.0 || !isfinite(m_scale_factor)) {
						NSLog(@"Warning: Invalid scale factor %.2f, using 1.0", m_scale_factor);
						m_scale_factor = 1.0;
					}
                
					CGFloat display_width = fmax(m_width / m_scale_factor, 100.0);
					CGFloat display_height = fmax(m_height / m_scale_factor, 100.0);
					NSRect frame = NSMakeRect(0, 0, display_width, display_height);

					m_window = [[NSWindow alloc] initWithContentRect:frame
															styleMask:(NSWindowStyleMaskTitled | 
																	NSWindowStyleMaskClosable | 
																	NSWindowStyleMaskMiniaturizable)
															backing:NSBackingStoreBuffered
																defer:NO];

					if (m_window)
					{
						[m_window setTitle:[NSString stringWithUTF8String:m_title.c_str()]];
						
						// Position window in top-left corner respecting menu bar and dock
						NSRect screenFrame = [[NSScreen mainScreen] visibleFrame];
						CGFloat x = screenFrame.origin.x;
						CGFloat y = NSMaxY(screenFrame) - display_height;
						[m_window setFrameOrigin:NSMakePoint(x, y)];

						BerialView* view = [[BerialView alloc] initWithFrame:frame];
						[view setDevice:this];
						m_view = (__bridge void*)view;

						[m_window setContentView:view];
						[m_window setDelegate:view];

						// CRITICAL: Force window visibility on ARM64/M1
						[m_window makeKeyAndOrderFront:nil];
						[m_window orderFrontRegardless];
						[NSApp activateIgnoringOtherApps:YES];

						CGFloat window_scale = [m_window backingScaleFactor];
						if (window_scale > 0.0 && isfinite(window_scale)) {
							m_scale_factor = window_scale;
						}
						[view setScaleFactor:m_scale_factor];
					}
					window_created = YES;
				}
				@catch (NSException *exception) {
					creation_exception = exception;
					window_created = YES; // Set to YES to exit the wait loop
				}
			});

			// Wait for completion with timeout to avoid infinite blocking on ARM64
			int timeout_count = 0;
			while (!window_created && timeout_count < 100) {
				usleep(10000); // 10ms
				timeout_count++;
			}

			if (creation_exception)
			{
				NSLog(@"Exception during window creation: %@", creation_exception);
			}

			if (timeout_count >= 100)
			{
				NSLog(@"Timeout waiting for window creation on ARM64");
			}
		}
	}

	MemoryLeakTracer::resume();
}

// Close and cleanup the window
void berialdraw::DeviceCocoaImpl::close_window()
{
	if (m_view)
	{
		// Properly release the BerialView before closing window
		BerialView* view = (__bridge BerialView*)m_view;
		[view setDevice:nullptr];  // Nullify device reference to break cycles
		view = nil;
		m_view = nullptr;
	}

	if (m_window)
	{
		if ([m_window isKindOfClass:[NSWindow class]])
		{
			// Set delegate to nil BEFORE closing to break retain cycle
			[m_window setDelegate:nil];
			[m_window setContentView:nil];  // Release content view explicitly
			
			if ([NSThread isMainThread])
			{
				[(id)m_window close];
			}
			else
			{
				dispatch_sync(dispatch_get_main_queue(), ^{
					[m_window setDelegate:nil];
					[m_window setContentView:nil];
					[(id)m_window close];
				});
			}
		}
		m_window = nullptr;
	}

	if (m_buffer)
	{
		// Use free() instead of delete[] for posix_memalign() memory on ARM64
		free(m_buffer);
		m_buffer = nullptr;
	}
}

// Signal the device to quit
void berialdraw::DeviceCocoaImpl::quit()
{
	if (UIManager::desktop())
	{
		UIManager::desktop()->quit();
	}
}

// Process all available events without blocking, or sleep if none available
bool berialdraw::DeviceCocoaImpl::dispatch(bool blocking)
{
	bool result = false;

	// Set appStarted to YES on the view (Objective-C)
	if (m_view) {
		BerialView* view = (__bridge BerialView*)m_view;
		view.appStarted = YES;
	}

	if (!m_app_initialized || !NSApp)
	{
		return result;
	}

	@try
	{
		uint32_t count = 0;
		bool has_more_events = true;
        
		// Process all available events in a non-blocking manner
		while (has_more_events)
		{
			NSEvent* event = [NSApp nextEventMatchingMask:NSEventMaskAny
												untilDate:[NSDate distantPast]  // Non-blocking
													inMode:NSDefaultRunLoopMode
												dequeue:YES];
            
			if (event)
			{
				count++;
				[NSApp sendEvent:event];
			}
			else
			{
				has_more_events = false;
			}
		}
        
		[NSApp updateWindows];
        
		// If no events were processed and blocking is requested, sleep briefly
		if (count == 0 && blocking)
		{
			usleep(1000); // 1ms pause to avoid busy-waiting
		}
        
		// Return whether the application loop should continue
		if (UIManager::desktop())
		{
			result = UIManager::desktop()->is_loop();
		}
	}
	@catch (NSException *exception)
	{
		NSLog(@"Exception in dispatch: %@", exception);
		result = false;
	}
    
	return result;
}

// Clear the buffer to white
void berialdraw::DeviceCocoaImpl::clear()
{
	if (m_buffer)
	{
		memset(m_buffer, 0xFF, m_width * m_height * 3);
		m_dirty = true;
	}
}

// Copy pixel data from source buffer to internal buffer
void berialdraw::DeviceCocoaImpl::copy(const uint8_t* buffer, Dim width, Dim height)
{
	m_dirty = true;

	if (m_buffer && buffer)
	{
		// Use optimized memory copy for ARM64 instead of pixel-by-pixel
		size_t source_size = width * height * 3;
		size_t dest_size = m_width * m_height * 3;

		if (width == m_width && height == m_height)
		{
			// Direct memcpy for same size buffers - much faster on ARM64
			memcpy(m_buffer, buffer, source_size);
		}
		else
		{
			// Fallback to row-by-row copy for different sizes
			Dim copy_width = (width < m_width) ? width : m_width;
			Dim copy_height = (height < m_height) ? height : m_height;
            
			for (Dim y = 0; y < copy_height; y++)
			{
				// Copy entire rows at once - more ARM64 SIMD friendly
				const uint8_t* src_row = buffer + (y * width * 3);
				uint8_t* dst_row = m_buffer + (y * m_width * 3);
				memcpy(dst_row, src_row, copy_width * 3);
			}
		}
	}
}

// Send the buffer to the view for display
void berialdraw::DeviceCocoaImpl::blit()
{
	if (m_dirty && m_buffer && m_view)
	{
		BerialView* view = (__bridge BerialView*)m_view;
		[view updateBuffer:m_buffer width:m_width height:m_height];
		m_dirty = false;
	}
}

// Refresh the display from the framebuffer
void berialdraw::DeviceCocoaImpl::refresh()
{
	Framebuf* framebuf = UIManager::framebuf();
	if (framebuf)
	{
		if (framebuf->dirty())
		{
			copy(framebuf->buffer(), framebuf->width(), framebuf->height());
			framebuf->dirty(false);
		}
	}
}

// Resize the window and internal buffer
void berialdraw::DeviceCocoaImpl::size(Dim width, Dim height)
{
	m_width = width;
	m_height = height;
	close_window();
	open_window();
}

// Get current window size
berialdraw::Size berialdraw::DeviceCocoaImpl::size() const
{
	return berialdraw::Size(m_width, m_height);
}

// DeviceCocoa public interface implementation
// Constructor: create the implementation
DeviceCocoa::DeviceCocoa(const char* title, Dim width, Dim height)
	: m_impl(new DeviceCocoaImpl(title, width, height))
{
}

// Destructor: cleanup implementation
DeviceCocoa::~DeviceCocoa()
{
	delete m_impl;
}

// Dispatch events and update display
bool DeviceCocoa::dispatch(bool blocking)
{
	refresh();
	blit();
	return m_impl->dispatch(blocking);
}

// Main event loop
void DeviceCocoa::mainloop()
{
	while (dispatch(true))
	{
		// Event loop continues until quit
	}
}

// Get window size
berialdraw::Size DeviceCocoa::size() const
{
	return m_impl->size();
}

// Set window size from Size object
void DeviceCocoa::size(const berialdraw::Size& s)
{
	size(s.width(), s.height());
}

// Set window size from width and height
void DeviceCocoa::size(Dim width, Dim height)
{
	m_impl->size(width, height);
}

// Clear the display
void DeviceCocoa::clear()
{
	m_impl->clear();
}

// Copy buffer to display
void DeviceCocoa::copy(const uint8_t* buffer, Dim width, Dim height)
{
	m_impl->copy(buffer, width, height);
}

// Refresh display from framebuffer
void DeviceCocoa::refresh()
{
	m_impl->refresh();
}

// Blit buffer to screen
void DeviceCocoa::blit()
{
	m_impl->blit();
}

// Request application quit
void DeviceCocoa::quit()
{
	m_impl->quit();
}
