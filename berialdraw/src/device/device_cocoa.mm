#import <Cocoa/Cocoa.h>
#import <CoreGraphics/CoreGraphics.h>

#include "berialdraw.hpp"
#include "device/device_cocoa.hpp"
#include "device/clipboard_cocoa.hpp"

using namespace berialdraw;

// macOS Key Code Constants (from Carbon/HIToolbox)
// These are the virtual key codes used by NSEvent.keyCode
namespace MacOSKeyCode {
	// Letter keys (alphabetical order)
	// constexpr uint16_t KEY_A               = 12;     // A key
	constexpr uint16_t KEY_B               = 11;    // B key
	constexpr uint16_t KEY_C               = 8;     // C key (Copy)
	constexpr uint16_t KEY_D               = 2;     // D key
	constexpr uint16_t KEY_E               = 14;    // E key
	constexpr uint16_t KEY_F               = 3;     // F key
	constexpr uint16_t KEY_G               = 5;     // G key
	constexpr uint16_t KEY_H               = 4;     // H key
	constexpr uint16_t KEY_I               = 34;    // I key
	constexpr uint16_t KEY_J               = 38;    // J key
	constexpr uint16_t KEY_K               = 40;    // K key
	constexpr uint16_t KEY_L               = 37;    // L key
	constexpr uint16_t KEY_M               = 46;    // M key
	constexpr uint16_t KEY_N               = 45;    // N key
	constexpr uint16_t KEY_O               = 31;    // O key
	constexpr uint16_t KEY_P               = 35;    // P key
	constexpr uint16_t KEY_Q               = 12;    // Q key
	constexpr uint16_t KEY_R               = 15;    // R key
	constexpr uint16_t KEY_S               = 1;     // S key
	constexpr uint16_t KEY_T               = 17;    // T key
	constexpr uint16_t KEY_U               = 32;    // U key
	constexpr uint16_t KEY_V               = 9;     // V key (Paste)
	constexpr uint16_t KEY_W               = 13;    // W key
	constexpr uint16_t KEY_X               = 7;     // X key (Cut)
	constexpr uint16_t KEY_Y               = 16;    // Y key
	constexpr uint16_t KEY_Z               = 6;     // Z key
	
	// Special keys
	constexpr uint16_t KEY_TAB             = 48;    // Tab key
	constexpr uint16_t KEY_SPACE           = 49;    // Space bar
	constexpr uint16_t KEY_BACKSPACE       = 51;    // Backspace/Delete
	constexpr uint16_t KEY_ESCAPE          = 53;    // Escape key
	constexpr uint16_t KEY_RETURN          = 36;    // Return/Enter key
	constexpr uint16_t KEY_DELETE          = 117;   // Delete (Forward Delete)
	constexpr uint16_t KEY_LEFT_ARROW      = 123;   // Left arrow
	constexpr uint16_t KEY_RIGHT_ARROW     = 124;   // Right arrow
	constexpr uint16_t KEY_DOWN_ARROW      = 125;   // Down arrow
	constexpr uint16_t KEY_UP_ARROW        = 126;   // Up arrow
	constexpr uint16_t KEY_HOME            = 115;   // Home key
	constexpr uint16_t KEY_END             = 119;   // End key
}

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

	DeviceCocoaImpl(const char* title, Dim width, Dim height, Coord x, Coord y);
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
	void position(Coord x, Coord y);
	berialdraw::Point position() const;
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
@property (nonatomic, strong) id globalEventMonitor;
- (void)updateBuffer:(uint8_t*)buffer width:(Dim)width height:(Dim)height;
- (void)keyDown:(NSEvent *)event;
- (void)keyUp:(NSEvent *)event;
- (wchar_t)convertKeyFromEvent:(NSEvent*)event;
- (KeyEvent::Modifier)getModifierFromEvent:(NSEvent*)event;
// Private helper methods
- (berialdraw::Point)getMousePositionFromEvent:(NSEvent *)event;
- (void)sendMouseEventWithType:(int)touchType position:(berialdraw::Point)position;
- (void)sendKeyEventWithKey:(wchar_t)key keyEventType:(int)keyEventType modifier:(berialdraw::KeyEvent::Modifier)modifier;
- (void)startGlobalMouseTracking;
- (void)stopGlobalMouseTracking;
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
		// Start tracking mouse movements outside the window
		[self startGlobalMouseTracking];
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
		// Stop tracking global mouse movements
		[self stopGlobalMouseTracking];
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

// Start global mouse tracking to track dragging outside the window
- (void)startGlobalMouseTracking
{
	// Stop any existing monitor first
	[self stopGlobalMouseTracking];
	
	// Create a global event monitor for mouse moved events
	self.globalEventMonitor = [NSEvent addGlobalMonitorForEventsMatchingMask:NSEventMaskMouseMoved | NSEventMaskLeftMouseDragged
		handler:^(NSEvent *event) {
			if (self.mouseDown && self.device && UIManager::notifier())
			{
				// Get mouse position in window coordinates
				NSWindow *window = [self window];
				if (window)
				{
					NSPoint screenLocation = [NSEvent mouseLocation];
					NSPoint windowLocation = [window convertPointFromScreen:screenLocation];
					NSPoint viewLocation = [self convertPoint:windowLocation fromView:nil];
					
					// Convert to buffer coordinates
					viewLocation.y = self.bounds.size.height - viewLocation.y;
					float scaleX = (float)self.bufferWidth / self.bounds.size.width;
					float scaleY = (float)self.bufferHeight / self.bounds.size.height;
					
					berialdraw::Point position(viewLocation.x * scaleX, viewLocation.y * scaleY);
					position.adapt_scale();
					
					// Send touch move event
					[self sendMouseEventWithType:TouchEvent::TOUCH_MOVE position:position];
				}
			}
		}];
}

// Stop global mouse tracking
- (void)stopGlobalMouseTracking
{
	if (self.globalEventMonitor)
	{
		[NSEvent removeMonitor:self.globalEventMonitor];
		self.globalEventMonitor = nil;
	}
}

// Handle mouse exit event (when cursor leaves the view)
- (void)mouseExited:(NSEvent *)event
{
	// Don't send TOUCH_UP here - keep tracking the drag outside the window
	// The drag will continue to be tracked by OS-level mouse tracking
	// Only release tracking when the button is actually released
}

// Handle mouse enter event (when cursor returns to the view while dragging)
- (void)mouseEntered:(NSEvent *)event
{
	// If we're in a drag operation, continue sending move events
	if (self.mouseDown && self.device)
	{
		berialdraw::Point position = [self getMousePositionFromEvent:event];
		[self sendMouseEventWithType:TouchEvent::TOUCH_MOVE position:position];
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
	NSEventModifierFlags flags = event.modifierFlags;
	BOOL hasCmd = (flags & NSEventModifierFlagCommand) != 0;
	BOOL hasCtrl = (flags & NSEventModifierFlagControl) != 0;
	BOOL hasShift = (flags & NSEventModifierFlagShift) != 0;

	// First switch: handle special keys (arrows, Enter, etc.) that are always recognized
	switch(event.keyCode)
	{
	case MacOSKeyCode::KEY_RETURN:
		result = (wchar_t)ReservedKey::KEY_ENTER;
		break;
	case MacOSKeyCode::KEY_ESCAPE:
		result = (wchar_t)ReservedKey::KEY_ESCAPE;
		break;
	case MacOSKeyCode::KEY_BACKSPACE:
		result = (wchar_t)ReservedKey::KEY_BACKSPACE;
		break;
	case MacOSKeyCode::KEY_DELETE:
		result = (wchar_t)ReservedKey::KEY_DELETE;
		break;
	case MacOSKeyCode::KEY_RIGHT_ARROW:
		result = (wchar_t)ReservedKey::KEY_RIGHT;
		break;
	case MacOSKeyCode::KEY_LEFT_ARROW:
		result = (wchar_t)ReservedKey::KEY_LEFT;
		break;
	case MacOSKeyCode::KEY_DOWN_ARROW:
		result = (wchar_t)ReservedKey::KEY_DOWN;
		break;
	case MacOSKeyCode::KEY_UP_ARROW:
		result = (wchar_t)ReservedKey::KEY_UP;
		break;
	case MacOSKeyCode::KEY_HOME:
		result = (wchar_t)ReservedKey::KEY_HOME;
		break;
	case MacOSKeyCode::KEY_END:
		result = (wchar_t)ReservedKey::KEY_END;
		break;
	case MacOSKeyCode::KEY_SPACE:
		result = ' ';  // Space
		break;
	case MacOSKeyCode::KEY_TAB:
		result = '\t'; // Tab
		break;
	}

	// Second switch: handle Cmd/Ctrl key combinations
	// Use the actual character from the keyboard layout (handles AZERTY, QWERTY, etc.)
	if ((hasCmd || hasCtrl) && !result)
	{
		NSString *chars = [event charactersIgnoringModifiers];
		if (chars && chars.length > 0)
		{
			unichar ch = [[chars lowercaseString] characterAtIndex:0];
			
			// Map character to ReservedKey
			switch (ch)
			{
			case 'a': result = (wchar_t)ReservedKey::KEY_CTRL_A; break;
			case 'b': result = (wchar_t)ReservedKey::KEY_CTRL_B; break;
			case 'c': result = (wchar_t)ReservedKey::KEY_CTRL_C; break;
			case 'd': result = (wchar_t)ReservedKey::KEY_CTRL_D; break;
			case 'e': result = (wchar_t)ReservedKey::KEY_CTRL_E; break;
			case 'f': result = (wchar_t)ReservedKey::KEY_CTRL_F; break;
			case 'g': result = (wchar_t)ReservedKey::KEY_CTRL_G; break;
			case 'h': result = (wchar_t)ReservedKey::KEY_CTRL_H; break;
			case 'i': result = (wchar_t)ReservedKey::KEY_CTRL_I; break;
			case 'j': result = (wchar_t)ReservedKey::KEY_CTRL_J; break;
			case 'k': result = (wchar_t)ReservedKey::KEY_CTRL_K; break;
			case 'l': result = (wchar_t)ReservedKey::KEY_CTRL_L; break;
			case 'm': result = (wchar_t)ReservedKey::KEY_CTRL_M; break;
			case 'n': result = (wchar_t)ReservedKey::KEY_CTRL_N; break;
			case 'o': result = (wchar_t)ReservedKey::KEY_CTRL_O; break;
			case 'p': result = (wchar_t)ReservedKey::KEY_CTRL_P; break;
			case 'q': result = (wchar_t)ReservedKey::KEY_CTRL_Q; break;
			case 'r': result = (wchar_t)ReservedKey::KEY_CTRL_R; break;
			case 's': result = (wchar_t)ReservedKey::KEY_CTRL_S; break;
			case 't': result = (wchar_t)ReservedKey::KEY_CTRL_T; break;
			case 'u': result = (wchar_t)ReservedKey::KEY_CTRL_U; break;
			case 'v': result = (wchar_t)ReservedKey::KEY_CTRL_V; break;
			case 'w': result = (wchar_t)ReservedKey::KEY_CTRL_W; break;
			case 'x': result = (wchar_t)ReservedKey::KEY_CTRL_X; break;
			case 'y': result = (wchar_t)ReservedKey::KEY_CTRL_Y; break;
			case 'z': result = (wchar_t)ReservedKey::KEY_CTRL_Z; break;
			}
		}
	}

	//printf("[KEY] keyCode=%u, cmd=%d, ctrl=%d, shift=%d -> %d\n", event.keyCode, hasCmd, hasCtrl, hasShift, result);

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

// Cleanup when view is deallocated
- (void)dealloc
{
	[self stopGlobalMouseTracking];
	// ARC will automatically call [super dealloc] with automatic reference counting
}

@end

// DeviceCocoaImpl method implementations
berialdraw::DeviceCocoaImpl::DeviceCocoaImpl(const char* title, Dim width, Dim height, Coord x, Coord y)
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
		// Set initial position after window creation
		if (x != 0 || y != 0)
		{
			position(x, y);
		}
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

	// Register Cocoa clipboard provider with UIManager (only once)
	static bool clipboard_initialized = false;
	if (!clipboard_initialized && UIManager::is_initialized())
	{
		UIManager::clipboard()->set_provider(new ClipboardProviderCocoa());
		clipboard_initialized = true;
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
        
		// Sync clipboard from system (bidirectional clipboard support)
		if (UIManager::clipboard())
		{
			UIManager::clipboard()->sync_from_system();
		}

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

// Get window position
berialdraw::Point berialdraw::DeviceCocoaImpl::position() const
{
	if (!m_window) {
		return berialdraw::Point(0, 0, true);
	}
	
	NSRect frame = [m_window frame];
	// Cocoa coordinates: origin at bottom-left, need to convert
	NSScreen* screen = [m_window screen];
	if (!screen) {
		screen = [NSScreen mainScreen];
	}
	
	NSRect screenFrame = [screen frame];
	Coord x = static_cast<Coord>(frame.origin.x);
	Coord y = static_cast<Coord>(screenFrame.size.height - frame.origin.y - frame.size.height);
	
	return berialdraw::Point(x, y, true);
}

// Set window position
void berialdraw::DeviceCocoaImpl::position(Coord x, Coord y)
{
	if (!m_window) return;
	
	NSScreen* screen = [m_window screen];
	if (!screen) {
		screen = [NSScreen mainScreen];
	}
	
	NSRect screenFrame = [screen frame];
	// Convert from standard coordinates (origin at top-left) to Cocoa (origin at bottom-left)
	CGFloat cocoa_y = screenFrame.size.height - (y + m_height);
	
	NSRect newFrame = [m_window frame];
	newFrame.origin.x = x;
	newFrame.origin.y = cocoa_y;
	
	[m_window setFrame:newFrame display:YES animate:NO];
}

// DeviceCocoa public interface implementation
// Constructor: create the implementation
DeviceCocoa::DeviceCocoa(const char* title, Dim width, Dim height, Coord x, Coord y)
	: m_impl(new DeviceCocoaImpl(title, width, height, x, y))
{
}

// Destructor
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

// Get window position
berialdraw::Point DeviceCocoa::position() const
{
	return m_impl->position();
}

// Set window position from Point object
void DeviceCocoa::position(const berialdraw::Point& p)
{
	position(p.x(), p.y());
}

// Set window position from x and y
void DeviceCocoa::position(Coord x, Coord y)
{
	m_impl->position(x, y);
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
