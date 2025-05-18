#ifndef berialdraw_hpp_INCLUDED
#define berialdraw_hpp_INCLUDED

// System includes
#include <stdint.h>
#include <stdarg.h>
#include <vector>
#include <mutex>

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
#include "tool/validator.hpp"
#include "tool/mask_validator.hpp"
#include "tool/singleton.hpp"
#include "tool/directory.hpp"
#include "tool/screen_crc.hpp"

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
#include "vector/side.hpp"

// Font
#include "font/utf8.hpp"
#include "font/font.hpp"
#include "font/fonts.hpp"

// Styles
#include "styles/color.hpp"
#include "styles/invalidator.hpp"
#include "styles/style.hpp"
#include "styles/common_style.hpp"
#include "styles/edit_style.hpp"
#include "styles/widget_style.hpp"
#include "styles/text_style.hpp"
#include "styles/border_style.hpp"
#include "styles/shape_style.hpp"
#include "styles/slider_style.hpp"
#include "styles/progress_bar_style.hpp"
#include "styles/pie_style.hpp"
#include "styles/switch_style.hpp"
#include "styles/mappings.hpp"
#include "styles/keys.hpp"
#include "styles/icon_style.hpp"
#include "styles/colors.hpp"
#include "styles/styles.hpp"

// Event 
#include "event/event.hpp"
#include "event/key_event.hpp"
#include "event/check_event.hpp"
#include "event/touch_event.hpp"
#include "event/scroll_event.hpp"
#include "event/click_event.hpp"
#include "event/select_event.hpp"
#include "event/slide_event.hpp"
#include "event/caller.hpp"
#include "event/notifier.hpp"

// Shape
#include "widget/entry.hpp"
#include "shape/shape.hpp"
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
#include "widget/scroll_view.hpp"
#include "widget/switch.hpp"
#include "widget/slider.hpp"
#include "widget/progress_bar.hpp"
#include "widget/icon.hpp"

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
#else
	#include <SDL2/SDL.h> 
	#include "device/device_sdl.hpp"
	#define DeviceScreen DeviceSdl
#endif


/**
@mainpage

Programme simple hello world

@include sample_hello_world.cpp


Widgets used for user interface :
<ul>
	<li>@ref berialdraw::Button</li>
	<li>@ref berialdraw::Edit</li>
	<li>@ref berialdraw::Icon</li>
	<li>@ref berialdraw::Keyboard</li>
	<li>@ref berialdraw::Label</li>
	<li>@ref berialdraw::Pane</li>
	<li>@ref berialdraw::ProgressBar</li>
	<li>@ref berialdraw::Slider</li>
	<li>@ref berialdraw::Switch</li>
	<li>@ref berialdraw::ScrollView</li>
	<li>@ref berialdraw::Widget</li>
	<li>@ref berialdraw::Window</li>
</ul>

Layout to place elements in the window :
<ul>
	<li>@ref berialdraw::Column : Place widgets in a column</li>
	<li>@ref berialdraw::Row : Place widgets in a row</li>
	<li>@ref berialdraw::Grid : Place widgets in a grid</li>
</ul>

Classes used for drawing :
<ul>
	<li>@ref berialdraw::Canvas : </li>

	Shapes :
	<ul>
		<li>@ref berialdraw::Line</li>
		<li>@ref berialdraw::Pie</li>
		<li>@ref berialdraw::PolyLines</li>
		<li>@ref berialdraw::PolyPoints</li>
		<li>@ref berialdraw::Polygon</li>
		<li>@ref berialdraw::Rect</li>
		<li>@ref berialdraw::Text</li>
	</ul>

	Markers :
	<ul>
		<li>@ref berialdraw::Circle</li>
		<li>@ref berialdraw::Cross</li>
		<li>@ref berialdraw::Square</li>
		<li>@ref berialdraw::Triangle</li>
	</ul>
</ul>

Desktop and ui manager :
<ul>
	<li>@ref berialdraw::Desktop : manages refresh and windows</li>
	<li>@ref berialdraw::UIManager : </li>
</ul>

Font :
<ul>
	<li>@ref berialdraw::Font</li>
	<li>@ref berialdraw::Fonts</li>
	<li>@ref berialdraw::FontFace</li>
	<li>@ref berialdraw::Utf8</li>
</ul>

Events :
<ul>
	<li>@ref berialdraw::CheckEvent : </li>
	<li>@ref berialdraw::ClickEvent</li>
	<li>@ref berialdraw::KeyEvent</li>
	<li>@ref berialdraw::ScrollEvent</li>
	<li>@ref berialdraw::SelectEvent</li>
	<li>@ref berialdraw::SlideEvent</li>
	<li>@ref berialdraw::TouchEvent</li>
	<li>@ref berialdraw::Notifier : handles events, calls callbacks, logs events and play tests events scripts</li>
</ul>

OS interface class for rendering and events :
<ul>
	<li>@ref berialdraw::Device : abstract class </li>
	<li>@ref berialdraw::DeviceSdl : interfacing with SDL</li>
	<li>@ref berialdraw::DeviceWin32 : win32 interface</li>
</ul>

*/
#endif // berialdraw_hpp_INCLUDED
