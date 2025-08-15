cmake_minimum_required(VERSION 3.18)

project(berialdraw)

set(BD_LIB         berialdraw)
set(BD_APP_TEST    app_unitary_test)
set(BD_APP_SAMPLE  app_sample)
set(BD_SAMPLE      sample)

set(COMPILE_DEF
	"$<$<CONFIG:Debug>:_DEBUG>"
	"$<$<CONFIG:Release>:NDEBUG>"
	"$<$<PLATFORM_ID:Linux>:LINUX>"
	"$<$<PLATFORM_ID:Darwin>:OSX>"
)

if(APPLE)
	execute_process(COMMAND uname -m OUTPUT_VARIABLE ARCHITECTURE OUTPUT_STRIP_TRAILING_WHITESPACE)
	set(CMAKE_OSX_ARCHITECTURES ${ARCHITECTURE})
	message(STATUS "Architecture selected (${ARCHITECTURE})")

	if (${ARCHITECTURE} STREQUAL "arm64")
		set(SDL3_INCLUDE_DIRS "/opt/homebrew/opt/sdl3/include")
	else()
		set(SDL3_INCLUDE_DIRS "/usr/local/opt/sdl3/include")
	endif()
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIND32 OR APPLE)
	set(SDL3_STATIC ON)
	find_package(SDL3 REQUIRED)
	set(SRCS_DEVICE src/device/device_sdl.cpp)

	message(STATUS "SDL3_LIBRARIES    = ${SDL3_LIBRARIES}")
	message(STATUS "SDL3_INCLUDE_DIRS = ${SDL3_INCLUDE_DIRS}")
else()
	set(SDL3_INCLUDE_DIRS "")
	set(SRCS_DEVICE src/device/device_win32.cpp)
	add_definitions(-DUNICODE -D_UNICODE)
endif()


set(SRCS_LIB
	src/widget/edit.cpp
	src/widget/desktop.cpp
	src/widget/grid.cpp
	src/widget/cells.cpp
	src/widget/window.cpp
	src/widget/widget.cpp
	src/widget/button.cpp
	src/widget/canvas.cpp
	src/widget/entry.cpp
	src/widget/label.cpp
	src/widget/pane.cpp
	src/widget/keyboard.cpp
	src/widget/switch.cpp
	src/widget/scroll_view.cpp
	src/widget/slider.cpp
	src/widget/row.cpp
	src/widget/column.cpp
	src/widget/progress_bar.cpp
	src/widget/icon.cpp
	src/widget/uimanager.cpp

	src/styles/invalidator.cpp
	src/styles/border_style.cpp
	src/styles/keys.cpp
	src/styles/mappings.cpp
	src/styles/pie_style.cpp
	src/styles/round_style.cpp
	src/styles/line_style.cpp
	src/styles/styles.cpp
	src/styles/text_style.cpp
	src/styles/widget_style.cpp
	src/styles/common_style.cpp
	src/styles/edit_style.cpp
	src/styles/switch_style.cpp
	src/styles/slider_style.cpp
	src/styles/progress_bar_style.cpp
	src/styles/icon_style.cpp
	src/styles/colors.cpp

	src/shape/shape.cpp
	src/shape/circle.cpp
	src/shape/line.cpp
	src/shape/pie.cpp
	src/shape/polygon.cpp
	src/shape/sketch.cpp
	src/shape/cross.cpp
	src/shape/marker.cpp
	src/shape/outline.cpp
	src/shape/text.cpp
	src/shape/triangle.cpp
	src/shape/vectors_script.cpp
	src/shape/square.cpp
	src/shape/compass.cpp
	src/shape/renderer.cpp
	src/shape/rect.cpp
	src/shape/poly_lines.cpp
	src/shape/poly_points.cpp
	src/shape/text_box.cpp
	src/shape/marker.cpp

	src/vector/area.cpp
	src/vector/region.cpp
	src/vector/linear.cpp
	src/vector/point.cpp
	src/vector/size.cpp
	src/vector/margin.cpp
	src/vector/align.cpp
	src/vector/size_policy.cpp
	src/vector/extend.cpp
	src/vector/dim.cpp
	src/vector/border.cpp

	src/font/utf8.cpp
	src/font/font.cpp
	src/font/fonts.cpp
	src/font/glyphs.cpp
	src/font/glyph.cpp
	src/font/font_face.cpp

	src/framebuf/argb8888.cpp
	src/framebuf/framebuf.cpp

	src/tool/bd_system.cpp
	src/tool/tools.cpp
	src/tool/string.cpp
	src/tool/hsl.cpp
	src/tool/svg_out.cpp
	src/tool/file.cpp
	src/tool/item_string.cpp
	src/tool/item_int.cpp
	src/tool/item_bool.cpp
	src/tool/item_null.cpp
	src/tool/item_array.cpp
	src/tool/item_object.cpp
	src/tool/item.cpp
	src/tool/json_iterator.cpp
	src/tool/json.cpp
	src/tool/text_stream.cpp
	src/tool/memory_leak_tracer.cpp
	src/tool/mask_validator.cpp
	src/tool/directory.cpp
	src/tool/screen_crc.cpp
	src/tool/arc_cache.cpp

	src/event/event.cpp
	src/event/key_event.cpp
	src/event/touch_event.cpp
	src/event/click_event.cpp
	src/event/check_event.cpp
	src/event/select_event.cpp
	src/event/scroll_event.cpp
	src/event/slide_event.cpp
	src/event/focus_event.cpp
	src/event/notifier.cpp

	src/chart/line_chart.cpp
	src/chart/data_set.cpp

	../freetype/src/base/ftbase.c
	../freetype/src/base/ftbitmap.c
	../freetype/src/base/ftdebug.c
	../freetype/src/base/ftglyph.c
	../freetype/src/base/ftinit.c
	../freetype/src/base/ftmm.c
	../freetype/src/sfnt/sfdriver.c
	../freetype/src/sfnt/sfobjs.c
	../freetype/src/sfnt/ttcmap.c
	../freetype/src/sfnt/ttkern.c
	../freetype/src/sfnt/ttload.c
	../freetype/src/sfnt/ttmtx.c
	../freetype/src/truetype/truetype.c
)

set(SRCS_SAMPLE
	samples/sample_speedometer.cpp
	samples/dialog.cpp
	samples/icon_menu_dialog.cpp
	samples/menu_dialog.cpp
	samples/samples.cpp
	samples/sample_button.cpp
	samples/sample_column.cpp
	samples/sample_row.cpp
	samples/sample_canvas.cpp
	samples/sample_progress_bar.cpp
	samples/sample_slider.cpp
	samples/sample_grid.cpp
	samples/sample_label.cpp
	samples/sample_edit.cpp
	samples/sample_icon.cpp
	samples/sample_pane.cpp
	samples/sample_switch.cpp
	samples/sample_keyboard.cpp
	samples/sample_scrollview.cpp
	samples/sample_theme.cpp
	samples/sample_forms.cpp
	samples/sample_window.cpp
	samples/sample_hello_world.cpp
)

set(SRCS_APP_TEST
	src/main.cpp
	src/tool/test.cpp
)

set(SRCS_APP_SAMPLE
	samples/main.cpp
)

set(INCLUDES_LIB 
	${SDL3_INCLUDE_DIRS}
	"${CMAKE_CURRENT_SOURCE_DIR}/../freetype/include;"
	"${CMAKE_CURRENT_SOURCE_DIR}/../freetype/include/freetype;"
	"${CMAKE_CURRENT_SOURCE_DIR}/inc;"
	"${CMAKE_CURRENT_SOURCE_DIR};"
	"${CMAKE_CURRENT_SOURCE_DIR}/..;"
)

# Static library
add_library                (${BD_LIB} STATIC  ${SRCS_DEVICE} ${SRCS_LIB} )
target_include_directories (${BD_LIB} PRIVATE ${INCLUDES_LIB})
target_compile_definitions (${BD_LIB} PRIVATE ${COMPILE_DEF} 
	-DPACKAGE=1
	-DHAVE_PTHREAD_SETSPECIFIC=1
	-DFT2_BUILD_LIBRARY=1
	-DFT_ERR_PREFIX=FT_Err_)
target_link_libraries      (${BD_LIB} ${SDL3_LIBRARIES})

# Unitary test application
add_executable             (${BD_APP_TEST} ${SRCS_APP_TEST} ${SRCS_SAMPLE})
target_include_directories (${BD_APP_TEST} PRIVATE ${INCLUDES_LIB})
target_compile_definitions (${BD_APP_TEST} PRIVATE ${COMPILE_DEF})
target_link_libraries      (${BD_APP_TEST} PRIVATE ${BD_LIB} ${SDL3_LIBRARIES})

# Unitary sample application
add_executable            (${BD_APP_SAMPLE} ${SRCS_APP_SAMPLE} ${SRCS_SAMPLE})
target_include_directories(${BD_APP_SAMPLE} PRIVATE ${INCLUDES_LIB})
target_compile_definitions(${BD_APP_SAMPLE} PRIVATE ${COMPILE_DEF})
target_link_libraries     (${BD_APP_SAMPLE} PRIVATE ${BD_LIB} ${SDL3_LIBRARIES})

file(COPY "${CMAKE_SOURCE_DIR}/resources" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/")
# file(GLOB FILES_TO_DELETE "${DIR_TO_CLEAN}/*.tmp" "${DIR_TO_CLEAN}/logs/*.log")