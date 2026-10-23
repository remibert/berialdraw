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
	set(SRCS_DEVICE src/device/system_time.cpp src/device/timer_manager_base.cpp src/device/device_win32.cpp src/device/clipboard_win32.cpp src/device/timer_manager_win32.cpp)
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
	set(SRCS_DEVICE src/device/system_time.cpp src/device/timer_manager_base.cpp src/device/device_cocoa.mm src/device/clipboard_cocoa.mm)
	set(DEVICE_LIBRARIES "-framework Cocoa -framework CoreGraphics")

	# Set proper compilation flags for .mm files
	set_source_files_properties(src/device/device_cocoa.mm PROPERTIES COMPILE_FLAGS "-fobjc-arc")
	set_source_files_properties(src/device/clipboard_cocoa.mm PROPERTIES COMPILE_FLAGS "-fobjc-arc")

	# Add the preprocessor definition for pybind code
	add_definitions(-DUSE_COCOA_DEVICE=1)
else()
	message(STATUS "Architecture Linux")
	message(STATUS "Using XCB (X11) backend")
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(XCB REQUIRED xcb xcb-image xcb-keysyms xcb-icccm)
	set(SRCS_DEVICE src/device/system_time.cpp src/device/timer_manager_base.cpp src/device/device_xcb.cpp src/device/clipboard_xcb.cpp)
	set(DEVICE_LIBRARIES ${XCB_LIBRARIES})
	include_directories(${XCB_INCLUDE_DIRS})
	add_definitions(-DUSE_XCB_DEVICE=1)
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
	src/widget/list_item.cpp
	src/widget/timer.cpp

	src/styles/invalidator.cpp
	src/styles/border_style.cpp
	src/styles/keys.cpp
	src/styles/mappings.cpp
	src/styles/padding_style.cpp
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
	src/styles/list_item_style.cpp
	src/styles/list_style.cpp
	src/styles/list_selection_mode.cpp
	src/styles/table_view_style.cpp
	src/styles/grid_style.cpp
	src/styles/cell_style.cpp
	src/styles/cells_style.cpp
	src/styles/timer_style.cpp
	src/styles/style_item.cpp
	src/styles/colors.cpp

	src/image/image_decoder.cpp
	src/image/image_cache.cpp
	src/image/image_item.cpp
	src/image/image_fit.cpp
	src/image/png_decoder.cpp
	src/image/jpeg_decoder.cpp
	src/image/png_encoder.cpp
	src/image/jpeg_encoder.cpp
	src/image/image_processor.cpp

	src/shape/shape.cpp
	src/shape/circle.cpp
	src/shape/line.cpp
	src/shape/pie.cpp
	src/shape/polygon.cpp
	src/shape/vector_image.cpp
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
	src/shape/rect_renderer.cpp
	src/shape/poly_lines.cpp
	src/shape/poly_points.cpp
	src/shape/text_box.cpp
	src/shape/rich_text.cpp
	src/shape/marker.cpp
	src/shape/star.cpp

	src/vector/area.cpp
	src/vector/region.cpp
	src/vector/clip_mask.cpp
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
	src/tool/timer_manager.cpp

	src/event/event.cpp
	src/event/key_event.cpp
	src/event/touch_event.cpp
	src/event/click_event.cpp
	src/event/check_event.cpp
	src/event/select_event.cpp
	src/event/scroll_event.cpp
	src/event/slide_event.cpp
	src/event/focus_event.cpp
	src/event/timer_event.cpp
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
	external/zlib/zutil.c
	external/libjpeg-turbo/src/jcapimin.c
	external/libjpeg-turbo/src/wrapper/jcapistd-8.c
	external/libjpeg-turbo/src/wrapper/jcapistd-12.c
	external/libjpeg-turbo/src/wrapper/jcapistd-16.c
	external/libjpeg-turbo/src/wrapper/jccoefct-8.c
	external/libjpeg-turbo/src/wrapper/jccoefct-12.c
	external/libjpeg-turbo/src/wrapper/jccolor-8.c
	external/libjpeg-turbo/src/wrapper/jccolor-12.c
	external/libjpeg-turbo/src/wrapper/jccolor-16.c
	external/libjpeg-turbo/src/wrapper/jcdctmgr-8.c
	external/libjpeg-turbo/src/wrapper/jcdctmgr-12.c
	external/libjpeg-turbo/src/wrapper/jcdiffct-8.c
	external/libjpeg-turbo/src/wrapper/jcdiffct-12.c
	external/libjpeg-turbo/src/wrapper/jcdiffct-16.c
	external/libjpeg-turbo/src/jchuff.c
	external/libjpeg-turbo/src/jcicc.c
	external/libjpeg-turbo/src/jcinit.c
	external/libjpeg-turbo/src/jclhuff.c
	external/libjpeg-turbo/src/wrapper/jclossls-8.c
	external/libjpeg-turbo/src/wrapper/jclossls-12.c
	external/libjpeg-turbo/src/wrapper/jclossls-16.c
	external/libjpeg-turbo/src/wrapper/jcmainct-8.c
	external/libjpeg-turbo/src/wrapper/jcmainct-12.c
	external/libjpeg-turbo/src/wrapper/jcmainct-16.c
	external/libjpeg-turbo/src/jcmarker.c
	external/libjpeg-turbo/src/jcmaster.c
	external/libjpeg-turbo/src/jcomapi.c
	external/libjpeg-turbo/src/jcparam.c
	external/libjpeg-turbo/src/jcphuff.c
	external/libjpeg-turbo/src/wrapper/jcprepct-8.c
	external/libjpeg-turbo/src/wrapper/jcprepct-12.c
	external/libjpeg-turbo/src/wrapper/jcprepct-16.c
	external/libjpeg-turbo/src/wrapper/jcsample-8.c
	external/libjpeg-turbo/src/wrapper/jcsample-12.c
	external/libjpeg-turbo/src/wrapper/jcsample-16.c
	external/libjpeg-turbo/src/jctrans.c
	external/libjpeg-turbo/src/jdapimin.c
	external/libjpeg-turbo/src/wrapper/jdapistd-8.c
	external/libjpeg-turbo/src/wrapper/jdapistd-12.c
	external/libjpeg-turbo/src/wrapper/jdapistd-16.c
	external/libjpeg-turbo/src/jdatadst.c
	external/libjpeg-turbo/src/jdatasrc.c
	external/libjpeg-turbo/src/wrapper/jdcoefct-8.c
	external/libjpeg-turbo/src/wrapper/jdcoefct-12.c
	external/libjpeg-turbo/src/wrapper/jdcolor-8.c
	external/libjpeg-turbo/src/wrapper/jdcolor-12.c
	external/libjpeg-turbo/src/wrapper/jdcolor-16.c
	external/libjpeg-turbo/src/wrapper/jddctmgr-8.c
	external/libjpeg-turbo/src/wrapper/jddctmgr-12.c
	external/libjpeg-turbo/src/wrapper/jddiffct-8.c
	external/libjpeg-turbo/src/wrapper/jddiffct-12.c
	external/libjpeg-turbo/src/wrapper/jddiffct-16.c
	external/libjpeg-turbo/src/jdhuff.c
	external/libjpeg-turbo/src/jdicc.c
	external/libjpeg-turbo/src/jdinput.c
	external/libjpeg-turbo/src/jdlhuff.c
	external/libjpeg-turbo/src/wrapper/jdlossls-8.c
	external/libjpeg-turbo/src/wrapper/jdlossls-12.c
	external/libjpeg-turbo/src/wrapper/jdlossls-16.c
	external/libjpeg-turbo/src/wrapper/jdmainct-8.c
	external/libjpeg-turbo/src/wrapper/jdmainct-12.c
	external/libjpeg-turbo/src/wrapper/jdmainct-16.c
	external/libjpeg-turbo/src/jdmarker.c
	external/libjpeg-turbo/src/jdmaster.c
	external/libjpeg-turbo/src/wrapper/jdmerge-8.c
	external/libjpeg-turbo/src/wrapper/jdmerge-12.c
	external/libjpeg-turbo/src/jdphuff.c
	external/libjpeg-turbo/src/wrapper/jdpostct-8.c
	external/libjpeg-turbo/src/wrapper/jdpostct-12.c
	external/libjpeg-turbo/src/wrapper/jdpostct-16.c
	external/libjpeg-turbo/src/wrapper/jdsample-8.c
	external/libjpeg-turbo/src/wrapper/jdsample-12.c
	external/libjpeg-turbo/src/wrapper/jdsample-16.c
	external/libjpeg-turbo/src/jdtrans.c
	external/libjpeg-turbo/src/jerror.c
	external/libjpeg-turbo/src/jfdctflt.c
	external/libjpeg-turbo/src/wrapper/jfdctfst-8.c
	external/libjpeg-turbo/src/wrapper/jfdctfst-12.c
	external/libjpeg-turbo/src/wrapper/jfdctint-8.c
	external/libjpeg-turbo/src/wrapper/jfdctint-12.c
	external/libjpeg-turbo/src/wrapper/jidctflt-8.c
	external/libjpeg-turbo/src/wrapper/jidctflt-12.c
	external/libjpeg-turbo/src/wrapper/jidctfst-8.c
	external/libjpeg-turbo/src/wrapper/jidctfst-12.c
	external/libjpeg-turbo/src/wrapper/jidctint-8.c
	external/libjpeg-turbo/src/wrapper/jidctint-12.c
	external/libjpeg-turbo/src/wrapper/jidctred-8.c
	external/libjpeg-turbo/src/wrapper/jidctred-12.c
	external/libjpeg-turbo/src/jmemmgr.c
	external/libjpeg-turbo/src/jmemnobs.c
	external/libjpeg-turbo/src/jpeg_nbits.c
	external/libjpeg-turbo/src/wrapper/jquant1-8.c
	external/libjpeg-turbo/src/wrapper/jquant1-12.c
	external/libjpeg-turbo/src/wrapper/jquant2-8.c
	external/libjpeg-turbo/src/wrapper/jquant2-12.c
	external/libjpeg-turbo/src/wrapper/jutils-8.c
	external/libjpeg-turbo/src/wrapper/jutils-12.c
	external/libjpeg-turbo/src/wrapper/jutils-16.c
)

set(INCLUDES_LIB 
	"${CMAKE_CURRENT_SOURCE_DIR}/include/external/freetype;"
	"${CMAKE_CURRENT_SOURCE_DIR}/external/freetype/include;"
	"${CMAKE_CURRENT_SOURCE_DIR}/external/freetype/include/freetype;"
	"${CMAKE_CURRENT_SOURCE_DIR}/external/zlib;"
	"${CMAKE_CURRENT_SOURCE_DIR}/external/minizip;"
	"${CMAKE_CURRENT_SOURCE_DIR}/external/minizip/compat;"
	"${CMAKE_CURRENT_SOURCE_DIR}/external/libpng;"
	"${CMAKE_CURRENT_SOURCE_DIR}/external/libjpeg-turbo/src;"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/external/libpng;"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/external/libjpeg-turbo;"
	"${CMAKE_CURRENT_SOURCE_DIR}/include;"
	"${CMAKE_CURRENT_SOURCE_DIR};"
)

# Build the vendored PNG library and minizip compatibility API.
set(PNG_SHARED OFF CACHE BOOL "Build libpng as a shared library" FORCE)
set(PNG_STATIC ON CACHE BOOL "Build libpng as a static library" FORCE)
set(PNG_FRAMEWORK OFF CACHE BOOL "Build libpng as a framework" FORCE)
set(PNG_TESTS OFF CACHE BOOL "Build libpng tests" FORCE)
set(PNG_TOOLS OFF CACHE BOOL "Build libpng tools" FORCE)
set(PNG_EXECUTABLES OFF CACHE BOOL "Build libpng executables" FORCE)
add_subdirectory(external/libpng EXCLUDE_FROM_ALL)

set(MZ_COMPAT ON CACHE BOOL "Enable minizip compatibility API" FORCE)
set(MZ_ZLIB ON CACHE BOOL "Enable zlib compression in minizip" FORCE)
set(MZ_ZLIB_FLAVOR "zlib" CACHE STRING "Use zlib with minizip" FORCE)
set(MZ_LIBCOMP OFF CACHE BOOL "Disable Apple Compression for minizip" FORCE)
set(MZ_BZIP2 OFF CACHE BOOL "Disable BZip2 in minizip" FORCE)
set(MZ_LZMA OFF CACHE BOOL "Disable LZMA in minizip" FORCE)
set(MZ_PPMD OFF CACHE BOOL "Disable PPMd in minizip" FORCE)
set(MZ_ZSTD OFF CACHE BOOL "Disable Zstandard in minizip" FORCE)
set(MZ_OPENSSL OFF CACHE BOOL "Disable OpenSSL in minizip" FORCE)
set(MZ_LIBBSD OFF CACHE BOOL "Disable libbsd in minizip" FORCE)
set(MZ_ICONV OFF CACHE BOOL "Disable iconv in minizip" FORCE)
set(MZ_PKCRYPT OFF CACHE BOOL "Disable PKWARE encryption in minizip" FORCE)
set(MZ_WZAES OFF CACHE BOOL "Disable WinZIP AES in minizip" FORCE)
set(MZ_FETCH_LIBS OFF CACHE BOOL "Do not fetch minizip dependencies" FORCE)
set(MZ_FORCE_FETCH_LIBS OFF CACHE BOOL "Do not force-fetch minizip dependencies" FORCE)
set(MZ_BUILD_TESTS OFF CACHE BOOL "Do not build minizip tests" FORCE)
set(MZ_BUILD_UNIT_TESTS OFF CACHE BOOL "Do not build minizip unit tests" FORCE)
set(MZ_BUILD_FUZZ_TESTS OFF CACHE BOOL "Do not build minizip fuzz tests" FORCE)
add_subdirectory(external/minizip EXCLUDE_FROM_ALL)

# Static library
add_library                (${BD_LIB} STATIC  ${SRCS_DEVICE} ${SRCS_LIB} )
target_include_directories (${BD_LIB} PRIVATE ${INCLUDES_LIB})
target_include_directories (${BD_LIB} BEFORE PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/external/libpng")
target_compile_definitions (${BD_LIB} PRIVATE ${COMPILE_DEF}
	-DPACKAGE=1
	-DHAVE_PTHREAD_SETSPECIFIC=1
	-DFT2_BUILD_LIBRARY=1
	-DFT_ERR_PREFIX=FT_Err_
	-D_DEFAULT_SOURCE
	-DHAVE_ZLIB
	-DZLIB_COMPAT)
target_link_libraries       (${BD_LIB} PRIVATE png_static MINIZIP::minizip)

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