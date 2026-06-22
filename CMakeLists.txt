cmake_minimum_required(VERSION 3.18)

project(berialdraw)

set(BD_LIB         berialdraw)
set(BD_APP_TEST    app_unitary_test)
set(BD_APP_SAMPLE  app_sample)
set(BD_SAMPLE_LIB  berialdraw_samples)
set(BD_PY11_LIB    pyberialdraw)

set(COMPILE_DEF
	"$<$<CONFIG:Debug>:_DEBUG>"
	"$<$<CONFIG:Release>:NDEBUG>"
	"$<$<PLATFORM_ID:Linux>:LINUX>"
	"$<$<PLATFORM_ID:Darwin>:OSX>"
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
	message(STATUS "Architecture Windows")
	set(SRCS_DEVICE src/device/device_win32.cpp src/device/clipboard_win32.cpp)
	set(DEVICE_LIBRARIES "")
	add_definitions(-DUNICODE -D_UNICODE)
elseif(APPLE)
	execute_process(COMMAND uname -m OUTPUT_VARIABLE ARCHITECTURE OUTPUT_STRIP_TRAILING_WHITESPACE)
	# Universal binary only for Release builds (not for Debug)
	if(CMAKE_BUILD_TYPE STREQUAL "Release")
		set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64")
		message(STATUS "Architecture Apple (Universal: x86_64 + arm64) [Release]")
	else()
		# For Debug or other, use native architecture
		unset(CMAKE_OSX_ARCHITECTURES)
		message(STATUS "Architecture Apple (Native arch only) [Debug]")
	endif()

	# Use native Cocoa device implementation on macOS
	set(SRCS_DEVICE src/device/device_cocoa.mm src/device/clipboard_cocoa.mm)
	set(DEVICE_LIBRARIES "-framework Cocoa -framework CoreGraphics")

	# Set proper compilation flags for .mm files
	set_source_files_properties(src/device/device_cocoa.mm PROPERTIES COMPILE_FLAGS "-fobjc-arc")
	set_source_files_properties(src/device/clipboard_cocoa.mm PROPERTIES COMPILE_FLAGS "-fobjc-arc")

	# Add the preprocessor definition for pybind code
	add_definitions(-DUSE_COCOA_DEVICE=1)
else()
	message(STATUS "Architecture Linux")
	
	# Option to use SDL instead of XCB (OFF by default)
	option(USE_SDL "Use SDL3 instead of XCB for Linux backend" OFF)
	
	if(USE_SDL)
		message(STATUS "Using SDL3 backend")
		set(SDL3_STATIC ON)
		find_package(SDL3 REQUIRED)
		set(SRCS_DEVICE src/device/device_sdl.cpp src/device/clipboard_sdl.cpp)
		set(DEVICE_LIBRARIES ${SDL3_LIBRARIES})
		add_definitions(-DUSE_SDL_DEVICE=1)
	else()
		message(STATUS "Using XCB (X11) backend")
		find_package(PkgConfig REQUIRED)
		pkg_check_modules(XCB REQUIRED xcb xcb-image xcb-keysyms xcb-icccm)
		set(SRCS_DEVICE src/device/device_xcb.cpp src/device/clipboard_xcb.cpp)
		set(DEVICE_LIBRARIES ${XCB_LIBRARIES})
		include_directories(${XCB_INCLUDE_DIRS})
		add_definitions(-DUSE_XCB_DEVICE=1)
	endif()
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
	src/widget/checkbox.cpp
	src/widget/radio.cpp
	src/widget/scrollable_content.cpp
	src/widget/scroll_view.cpp
	src/widget/slider.cpp
	src/widget/row.cpp
	src/widget/column.cpp
	src/widget/progress_bar.cpp
	src/widget/icon.cpp
	src/widget/picture.cpp
	src/widget/uimanager.cpp
	src/widget/table_view.cpp
	src/widget/list.cpp

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
	src/styles/radio_style.cpp
	src/styles/checkbox_style.cpp
	src/styles/slider_style.cpp
	src/styles/progress_bar_style.cpp
	src/styles/icon_style.cpp
	src/styles/picture_style.cpp
	src/styles/scroll_view_style.cpp
	src/styles/scrollbar_style.cpp
	src/styles/list_style.cpp
	src/styles/table_view_style.cpp
	src/styles/grid_style.cpp
	src/styles/cell_style.cpp
	src/styles/cells_style.cpp
	src/styles/style_item.cpp
	src/styles/colors.cpp

	src/image/image_decoder.cpp
	src/image/image_cache.cpp
	src/image/image_item.cpp
	src/image/image_fit.cpp
	src/image/png_decoder.cpp
	src/image/jpeg_decoder.cpp
	src/image/image_processor.cpp

	src/shape/shape.cpp
	src/shape/circle.cpp
	src/shape/line.cpp
	src/shape/pie.cpp
	src/shape/polygon.cpp
	src/shape/sketch.cpp
	src/shape/image.cpp
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
	src/shape/rich_text.cpp
	src/shape/marker.cpp

	src/vector/area.cpp
	src/vector/region.cpp
	src/vector/linear.cpp
	src/vector/point.cpp
	src/vector/size.cpp
	src/vector/margin.cpp
	src/vector/align.cpp
	src/vector/scroll_direction.cpp
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
	$<$<PLATFORM_ID:Windows>:src/tool/bd_system_win32.cpp>
	$<$<OR:$<PLATFORM_ID:Linux>,$<PLATFORM_ID:Darwin>>:src/tool/bd_system_posix.cpp>
	src/tool/tools.cpp
	src/tool/chronometer.cpp
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
	src/tool/clipboard.cpp
	src/tool/settings.cpp
	src/tool/local_file.cpp
	src/tool/local_directory.cpp
	src/tool/zip_file.cpp
	src/tool/zip_directory.cpp
	src/tool/zip_archive.cpp
	src/tool/file_tools.cpp

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

	external/freetype/src/base/ftbase.c
	external/freetype/src/base/ftbitmap.c
	external/freetype/src/base/ftdebug.c
	external/freetype/src/base/ftglyph.c
	external/freetype/src/base/ftinit.c
	external/freetype/src/base/ftmm.c
	external/freetype/src/sfnt/sfdriver.c
	external/freetype/src/sfnt/sfobjs.c
	external/freetype/src/sfnt/ttcmap.c
	external/freetype/src/sfnt/ttkern.c
	external/freetype/src/sfnt/ttload.c
	external/freetype/src/sfnt/ttmtx.c
	external/freetype/src/truetype/truetype.c

	# zlib compression library
	external/zlib/adler32.c
	external/zlib/compress.c
	external/zlib/crc32.c
	external/zlib/deflate.c
	external/zlib/infback.c
	external/zlib/inffast.c
	external/zlib/inflate.c
	external/zlib/inftrees.c
	external/zlib/trees.c
	external/zlib/uncompr.c
	external/zlib/zutil.c

	# minizip archive library
	external/minizip/mz_crypt.c
	external/minizip/mz_os.c
	external/minizip/mz_strm.c
	external/minizip/mz_strm_buf.c
	external/minizip/mz_strm_mem.c
	external/minizip/mz_strm_split.c
	external/minizip/mz_strm_zlib.c
	external/minizip/mz_strm_pkcrypt.c
	external/minizip/mz_zip.c
	external/minizip/mz_zip_rw.c
	external/minizip/compat/unzip.c
	external/minizip/compat/ioapi.c
	src/tool/mz_stream_os_berialdraw.c

	# libpng
	external/libpng/png.c
	external/libpng/pngerror.c
	external/libpng/pngget.c
	external/libpng/pngmem.c
	external/libpng/pngpread.c
	external/libpng/pngread.c
	external/libpng/pngrio.c
	external/libpng/pngrtran.c
	external/libpng/pngrutil.c
	external/libpng/pngset.c
	external/libpng/pngsimd.c
	external/libpng/pngtrans.c
	external/libpng/pngwio.c
	external/libpng/pngwrite.c
	external/libpng/pngwtran.c
	external/libpng/pngwutil.c

	# libjpeg-turbo (decode only)
	external/libjpeg-turbo/jcapimin.c
	external/libjpeg-turbo/jcapistd.c
	external/libjpeg-turbo/jccoefct.c
	external/libjpeg-turbo/jccolor.c
	external/libjpeg-turbo/jcdctmgr.c
	external/libjpeg-turbo/jchuff.c
	external/libjpeg-turbo/jcicc.c
	external/libjpeg-turbo/jcinit.c
	external/libjpeg-turbo/jcmainct.c
	external/libjpeg-turbo/jcmarker.c
	external/libjpeg-turbo/jcmaster.c
	external/libjpeg-turbo/jcomapi.c
	external/libjpeg-turbo/jcparam.c
	external/libjpeg-turbo/jcphuff.c
	external/libjpeg-turbo/jcprepct.c
	external/libjpeg-turbo/jcsample.c
	external/libjpeg-turbo/jctrans.c
	external/libjpeg-turbo/jdapimin.c
	external/libjpeg-turbo/jdapistd.c
	external/libjpeg-turbo/jdatadst.c
	external/libjpeg-turbo/jdatasrc.c
	external/libjpeg-turbo/jdcoefct.c
	external/libjpeg-turbo/jdcolor.c
	external/libjpeg-turbo/jddctmgr.c
	external/libjpeg-turbo/jdhuff.c
	external/libjpeg-turbo/jdicc.c
	external/libjpeg-turbo/jdinput.c
	external/libjpeg-turbo/jdmainct.c
	external/libjpeg-turbo/jdmarker.c
	external/libjpeg-turbo/jdmaster.c
	external/libjpeg-turbo/jdmerge.c
	external/libjpeg-turbo/jdphuff.c
	external/libjpeg-turbo/jdpostct.c
	external/libjpeg-turbo/jdsample.c
	external/libjpeg-turbo/jdtrans.c
	external/libjpeg-turbo/jerror.c
	external/libjpeg-turbo/jfdctflt.c
	external/libjpeg-turbo/jfdctfst.c
	external/libjpeg-turbo/jfdctint.c
	external/libjpeg-turbo/jidctflt.c
	external/libjpeg-turbo/jidctfst.c
	external/libjpeg-turbo/jidctint.c
	external/libjpeg-turbo/jidctred.c
	external/libjpeg-turbo/jmemmgr.c
	external/libjpeg-turbo/jmemnobs.c
	external/libjpeg-turbo/jquant1.c
	external/libjpeg-turbo/jquant2.c
	external/libjpeg-turbo/jsimd_none.c
	external/libjpeg-turbo/jutils.c
)

set(INCLUDES_LIB 
	"${CMAKE_CURRENT_SOURCE_DIR}/external/freetype/include;"
	"${CMAKE_CURRENT_SOURCE_DIR}/external/freetype/include/freetype;"
	"${CMAKE_CURRENT_SOURCE_DIR}/external/zlib;"
	"${CMAKE_CURRENT_SOURCE_DIR}/external/minizip;"
	"${CMAKE_CURRENT_SOURCE_DIR}/external/minizip/compat;"
	"${CMAKE_CURRENT_SOURCE_DIR}/external/libpng;"
	"${CMAKE_CURRENT_SOURCE_DIR}/external/libjpeg-turbo;"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/external/libpng;"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/external/libjpeg-turbo;"
	"${CMAKE_CURRENT_SOURCE_DIR}/include;"
	"${CMAKE_CURRENT_SOURCE_DIR};"
)


# Static library
add_library                (${BD_LIB} STATIC  ${SRCS_DEVICE} ${SRCS_LIB} )
target_include_directories (${BD_LIB} PRIVATE ${INCLUDES_LIB})
target_compile_definitions (${BD_LIB} PRIVATE ${COMPILE_DEF} 
	-DPACKAGE=1
	-DHAVE_PTHREAD_SETSPECIFIC=1
	-DFT2_BUILD_LIBRARY=1
	-DFT_ERR_PREFIX=FT_Err_
	-D_DEFAULT_SOURCE
	-DHAVE_ZLIB
	-DZLIB_COMPAT)

# Add subdirectories for modular build
add_subdirectory(pybind)
add_subdirectory(samples)

# Documentation target
option(BUILD_DOCS "Build Doxygen documentation" OFF)
if(BUILD_DOCS)
	add_subdirectory(doc)
endif()

# Use Python delivery script from builds/cmake/
# Configure it into the build directory at every configure step
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/builds/cmake/deliver.py"
    "${CMAKE_CURRENT_BINARY_DIR}/deliver.py"
    @ONLY
)

# Always remove the stamp file at configure time to force the copy at every build
file(REMOVE "${CMAKE_CURRENT_BINARY_DIR}/delivery/.copy_delivery_stamp")

add_custom_command(
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/delivery/.copy_delivery_stamp"
    COMMAND python3 "${CMAKE_CURRENT_BINARY_DIR}/deliver.py"
    #COMMAND ${CMAKE_COMMAND} -E touch "${CMAKE_CURRENT_BINARY_DIR}/delivery/.copy_delivery_stamp"
    #COMMAND ${CMAKE_COMMAND} -E rm -f "${CMAKE_CURRENT_BINARY_DIR}/deliver.py"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    DEPENDS ${BD_LIB} ${BD_PY11_LIB}
    COMMENT "Copying resources and testing ARM64 after build completion"
    VERBATIM
)

add_custom_target(copy_delivery ALL
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/delivery/.copy_delivery_stamp"
)

add_dependencies(copy_delivery ${BD_PY11_LIB})
if(TARGET ${BD_APP_SAMPLE})
    add_dependencies(copy_delivery ${BD_APP_SAMPLE})
endif()