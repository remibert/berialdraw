#pragma once
// System includes
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <mutex>
#include <stdexcept>
 
// Tools
#include "tool/memory_leak_tracer.hpp"
#include "tool/hsl.hpp"
#include "tool/stream.hpp"
#include "tool/text_stream.hpp"
#include "tool/string.hpp"
#include "tool/shared_ptr.hpp"
#include "tool/vector.hpp"
#include "tool/json_iterator.hpp"
#include "tool/json.hpp"
#include "tool/settings.hpp"
#include "tool/validator.hpp"
#include "tool/mask_validator.hpp"
#include "tool/singleton.hpp"
#include "tool/file_interface.hpp"
#include "tool/directory_interface.hpp"
#include "tool/local_file.hpp"
#include "tool/local_directory.hpp"
#include "tool/directory.hpp"
#include "tool/screen_crc.hpp"
#include "tool/clipboard.hpp"
#include "tool/chronometer.hpp"

/** Sets a callback function for printf output.
@param callback Function pointer to handle printf calls. */
extern "C" void bd_redirect_printf(void (*callback)(const char* format, va_list args));


// Framebuffer
#include "framebuf/framebuf.hpp"
#include "framebuf/argb8888.hpp"

// Vector
#include "vector/align.hpp"
#include "vector/extend.hpp"
#include "vector/size_policy.hpp"
#include "vector/orientation.hpp"
#include "vector/coord.hpp"
#include "vector/dim.hpp"
#include "vector/size.hpp"
#include "vector/point.hpp"
#include "vector/margin.hpp"
#include "vector/linear.hpp"
#include "vector/area.hpp"
#include "vector/border.hpp"

// Font
#include "font/utf8.hpp"
#include "font/font.hpp"
#include "font/fonts.hpp"

// Image
#include "image/image_fit.hpp"
#include "image/image_decoder.hpp"
#include "image/png_decoder.hpp"
#include "image/jpeg_decoder.hpp"
#include "image/image_processor.hpp"
#include "image/image_cache.hpp"
#include "image/image_item.hpp"

// Styles
#include "styles/color.hpp"
#include "styles/invalidator.hpp"
#include "styles/style.hpp"
#include "styles/line_style.hpp"
#include "styles/common_style.hpp"
#include "styles/edit_style.hpp"
#include "styles/widget_style.hpp"
#include "styles/text_style.hpp"
#include "styles/border_style.hpp"
#include "styles/round_style.hpp"
#include "styles/slider_style.hpp"
#include "styles/progress_bar_style.hpp"
#include "styles/pie_style.hpp"
#include "styles/switch_style.hpp"
#include "styles/radio_style.hpp"
#include "styles/checkbox_style.hpp"
#include "styles/mappings.hpp"
#include "styles/keys.hpp"
#include "styles/icon_style.hpp"
#include "styles/picture_style.hpp"
#include "styles/colors.hpp"
#include "styles/scroll_view_style.hpp"
#include "styles/table_view_style.hpp"
#include "styles/grid_style.hpp"
#include "styles/cell_style.hpp"
#include "styles/cells_style.hpp"
#include "styles/styles.hpp"
#include "styles/style_names.hpp"
#include "styles/style_item.hpp"
#include "vector/scroll_direction.hpp"

// Event 
#include "event/event.hpp"
#include "event/key_event.hpp"
#include "event/check_event.hpp"
#include "event/touch_event.hpp"
#include "event/scroll_event.hpp"
#include "event/click_event.hpp"
#include "event/select_event.hpp"
#include "event/slide_event.hpp"
#include "event/focus_event.hpp"
#include "event/caller.hpp"
#include "event/notifier.hpp"

// Shape
#include "widget/entry.hpp"
#include "shape/shape.hpp"
#include "shape/rich_text.hpp"
#include "shape/text_box.hpp"
#include "shape/polygon.hpp"
#include "shape/poly_points.hpp"
#include "shape/line.hpp"
#include "shape/poly_lines.hpp"
#include "shape/rect.hpp"
#include "shape/pie.hpp"
#include "shape/vectors_script.hpp"
#include "shape/compass.hpp"
#include "shape/marker.hpp"
#include "shape/circle.hpp"
#include "shape/square.hpp"
#include "shape/triangle.hpp"
#include "shape/cross.hpp"
#include "shape/text.hpp"
#include "shape/sketch.hpp"
#include "shape/image.hpp"

// Chart
#include "chart/data_set.hpp"
#include "chart/line_chart.hpp"

// Tool
#include "tool/test.hpp"
#include "tool/exporter.hpp"
#include "tool/svg_out.hpp"
#include "tool/file.hpp"

// Device
#include "device/device.hpp"

// Widget
#include "tool/arc_cache.hpp"
#include "widget/uimanager.hpp"
#include "widget/desktop.hpp"
#include "widget/cells.hpp"
#include "widget/widget.hpp"
#include "widget/label.hpp"
#include "widget/window.hpp"
#include "widget/pane.hpp"
#include "widget/canvas.hpp"
#include "widget/grid.hpp"
#include "widget/column.hpp"
#include "widget/row.hpp"
#include "widget/button.hpp"
#include "widget/edit.hpp"
#include "widget/keyboard.hpp"
#include "widget/scrollable_content.hpp"
#include "widget/scroll_view.hpp"
#include "widget/switch.hpp"
#include "widget/radio.hpp"
#include "widget/checkbox.hpp"
#include "widget/slider.hpp"
#include "widget/progress_bar.hpp"
#include "widget/icon.hpp"
#include "widget/picture.hpp"
#include "widget/table_view.hpp"

#if defined(WIN32)
	#define Polygon Polygon_
	#define Pie Pie_
	#define Rectangle Rectangle_
	#define sep sep_
	#include <Windows.h>
	#include <Winuser.h>
	#undef IN
	#undef OUT
	#undef TRANSPARENT
	#undef min
	#undef max
	#undef Polygon
	#undef Pie
	#undef Pane
	#undef sep
	#include "device/device_win32.hpp"
	#define DeviceScreen DeviceWin32
#elif defined(OSX) && defined(USE_COCOA_DEVICE)
	#include "device/device_cocoa.hpp"
	#define DeviceScreen DeviceCocoa
#else
	//#include <SDL3/SDL.h> 
	#include "device/device_sdl.hpp"
	#define DeviceScreen DeviceSdl
#endif


/**
@mainpage BerialDraw - C++20 UI Framework

A lightweight, cross-platform C++20 UI framework for embedded systems, desktop (Win32, macOS/Cocoa, Linux/XCB/SDL3), and headless rendering. 
Features anti-aliased vector graphics and raster image rendering via FreeType's rasterizer, without GPU acceleration.

@section widgets Widgets for user interface
<ul>
	<li>@ref berialdraw::Button - Clickable button widget</li>
	<li>@ref berialdraw::Checkbox - Checkbox widget</li>
	<li>@ref berialdraw::Edit - Text input widget</li>
	<li>@ref berialdraw::Icon - Icon/image display widget</li>
	<li>@ref berialdraw::Keyboard - On-screen keyboard widget</li>
	<li>@ref berialdraw::Label - Text label widget</li>
	<li>@ref berialdraw::Pane - Container widget with borders</li>
	<li>@ref berialdraw::Picture - Picture/image widget</li>
	<li>@ref berialdraw::ProgressBar - Progress bar widget</li>
	<li>@ref berialdraw::Radio - Radio button widget</li>
	<li>@ref berialdraw::Slider - Slider/seek bar widget</li>
	<li>@ref berialdraw::Switch - Toggle switch widget</li>
	<li>@ref berialdraw::ScrollView - Scrollable view container</li>
	<li>@ref berialdraw::TableView - Table with rows and columns</li>
	<li>@ref berialdraw::Widget - Base widget class</li>
	<li>@ref berialdraw::Window - Top-level window widget</li>
</ul>

@section layouts Layout managers
<ul>
	<li>@ref berialdraw::Column - Stack widgets vertically</li>
	<li>@ref berialdraw::Row - Stack widgets horizontally</li>
	<li>@ref berialdraw::Grid - Arrange widgets in a grid</li>
</ul>

@section drawing Drawing and Shapes
<ul>
	<li>@ref berialdraw::Canvas - Vector drawing canvas</li>
	
	<li><b>Vector Shapes:</b>
	<ul>
		<li>@ref berialdraw::Circle - Circle marker</li>
		<li>@ref berialdraw::Compass - Directional compass shape</li>
		<li>@ref berialdraw::Cross - Cross/plus marker</li>
		<li>@ref berialdraw::Line - Line shape</li>
		<li>@ref berialdraw::Pie - Pie/arc shape</li>
		<li>@ref berialdraw::PolyLines - Connected line segments</li>
		<li>@ref berialdraw::PolyPoints - Point collection</li>
		<li>@ref berialdraw::Polygon - Polygon shape</li>
		<li>@ref berialdraw::Rect - Rectangle shape</li>
		<li>@ref berialdraw::Square - Square marker</li>
		<li>@ref berialdraw::Text - Text shape</li>
		<li>@ref berialdraw::Triangle - Triangle marker</li>
	</ul>
	</li>
	
	<li><b>Raster Shapes:</b>
	<ul>
		<li>@ref berialdraw::Image - Raster image shape</li>
		<li>@ref berialdraw::Sketch - Vector graphic from file (.icn/.svg)</li>
	</ul>
	</li>
	
	<li><b>Charts:</b>
	<ul>
		<li>@ref berialdraw::LineChart - Line chart visualization</li>
	</ul>
	</li>
</ul>

@section ui UIManager and Desktop
<ul>
	<li>@ref berialdraw::UIManager - Singleton manager for rendering, events, and resources</li>
	<li>@ref berialdraw::Desktop - Manages window tree and refresh cycle</li>
	<li>@ref berialdraw::Renderer - Vector rasterizer using FreeType outlines</li>
</ul>

@section font Font Management
<ul>
	<li>@ref berialdraw::Font - Font instance</li>
	<li>@ref berialdraw::Fonts - Font cache and manager</li>
	<li>@ref berialdraw::Utf8 - UTF-8 text utilities</li>
</ul>

@section events Events
<ul>
	<li>@ref berialdraw::TouchEvent - Touch/mouse input event</li>
	<li>@ref berialdraw::KeyEvent - Keyboard event</li>
	<li>@ref berialdraw::ClickEvent - Widget click event</li>
	<li>@ref berialdraw::CheckEvent - Checkbox state change event</li>
	<li>@ref berialdraw::SelectEvent - Item selection event</li>
	<li>@ref berialdraw::SlideEvent - Slider movement event</li>
	<li>@ref berialdraw::ScrollEvent - Scroll event</li>
	<li>@ref berialdraw::FocusEvent - Focus change event</li>
	<li>@ref berialdraw::Notifier - Event dispatcher and test script player</li>
</ul>

@section platform Platform Backends
<ul>
	<li>@ref berialdraw::Device - Abstract device interface</li>
	<li>@ref berialdraw::DeviceWin32 - Windows native backend (Win32 API)</li>
	<li>@ref berialdraw::DeviceCocoa - macOS native backend (Cocoa/CoreGraphics)</li>
	<li>@ref berialdraw::DeviceSdl - SDL3 backend (Linux, cross-platform)</li>
	<li>@ref berialdraw::DeviceXcb - X11/XCB backend (Linux default)</li>
</ul>

@section image Image Processing
<ul>
	<li>@ref berialdraw::ImageProcessor - Bicubic resize and bilinear rotation</li>
	<li>@ref berialdraw::ImageCache - LRU decoded image cache</li>
	<li>@ref berialdraw::PngDecoder - PNG image decoder</li>
	<li>@ref berialdraw::JpegDecoder - JPEG image decoder</li>
</ul>

@section example Quick Start
@include sample_hello_world.cpp

*/
