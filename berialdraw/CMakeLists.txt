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
	set(SDL3_STATIC ON)
	find_package(SDL3 REQUIRED)
	set(SRCS_DEVICE src/device/device_sdl.cpp src/device/clipboard_sdl.cpp)
	set(DEVICE_LIBRARIES ${SDL3_LIBRARIES})
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
	src/styles/checkbox_style.cpp
	src/styles/slider_style.cpp
	src/styles/progress_bar_style.cpp
	src/styles/icon_style.cpp
	src/styles/scroll_view_style.cpp
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

	# zlib compression library
	../zlib/adler32.c
	../zlib/compress.c
	../zlib/crc32.c
	../zlib/deflate.c
	../zlib/infback.c
	../zlib/inffast.c
	../zlib/inflate.c
	../zlib/inftrees.c
	../zlib/trees.c
	../zlib/uncompr.c
	../zlib/zutil.c

	# minizip archive library
	../minizip/mz_crypt.c
	../minizip/mz_os.c
	../minizip/mz_strm.c
	../minizip/mz_strm_buf.c
	../minizip/mz_strm_mem.c
	../minizip/mz_strm_split.c
	../minizip/mz_strm_zlib.c
	../minizip/mz_strm_pkcrypt.c
	../minizip/mz_zip.c
	../minizip/mz_zip_rw.c
	../minizip/compat/unzip.c
	../minizip/compat/ioapi.c
	src/tool/mz_stream_os_berialdraw.c
)

set(INCLUDES_LIB 
	${SDL3_INCLUDE_DIRS}
	"${CMAKE_CURRENT_SOURCE_DIR}/../freetype/include;"
	"${CMAKE_CURRENT_SOURCE_DIR}/../freetype/include/freetype;"
	"${CMAKE_CURRENT_SOURCE_DIR}/../zlib;"
	"${CMAKE_CURRENT_SOURCE_DIR}/../minizip;"
	"${CMAKE_CURRENT_SOURCE_DIR}/../minizip/compat;"
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
	-DFT_ERR_PREFIX=FT_Err_
	-D_DEFAULT_SOURCE
	-DHAVE_ZLIB
	-DZLIB_COMPAT)

# Add subdirectories for modular build
add_subdirectory(pybind)
add_subdirectory(samples)

# Write the Python script as a template in the source directory
set(COPY_DELIVERY_PY_IN "${CMAKE_CURRENT_SOURCE_DIR}/copy_delivery.py.in")
file(WRITE "${COPY_DELIVERY_PY_IN}" "
import os
import shutil
import glob
import platform
import stat

copy_jobs = [
    ('@CMAKE_SOURCE_DIR@/resources',              'delivery/resources', '**/*', lambda path, name: 'test' not in path.split(os.sep) and not name.startswith('test_')),
    ('@CMAKE_CURRENT_SOURCE_DIR@/inc',            'delivery/inc', '**/*', None),
    ('@CMAKE_CURRENT_BINARY_DIR@/lib@BD_LIB@.a',  'delivery/lib/%s/' % (platform.system().lower().replace('darwin','macos')), None, None),
    ('@CMAKE_CURRENT_BINARY_DIR@/samples',        'delivery/app', '*', lambda path, name: os.access(os.path.join(path, name), os.X_OK) and os.path.isfile(os.path.join(path, name))),
    # Only copy .so files, skip CMakeFiles directory
    ('@CMAKE_CURRENT_BINARY_DIR@/pybind',         'delivery/python', 'pyberialdraw*.so', lambda path, name: 'CMakeFiles' not in path.split(os.sep)),
    ('@CMAKE_CURRENT_SOURCE_DIR@/pybind/test.py', 'delivery/python/test.py', None, None),
]

def copy_with_filter(src, dst, wildcard=None, filter_func=None):
    if os.path.isdir(src):
        if wildcard is None:
            wildcard = '**/*'
        for root, dirs, files in os.walk(src):
            # Skip CMakeFiles directory for all jobs
            dirs[:] = [d for d in dirs if d != 'CMakeFiles']
            rel_root = os.path.relpath(root, src)
            for name in files:
                src_file = os.path.join(root, name)
                rel_path = os.path.normpath(os.path.join(rel_root, name))
                dst_file = os.path.join(dst, rel_path)
                if filter_func is None or filter_func(root, name):
                    os.makedirs(os.path.dirname(dst_file), exist_ok=True)
                    shutil.copy2(src_file, dst_file)
    elif wildcard:
        for src_file in glob.glob(os.path.join(src, wildcard), recursive=True):
            # Skip any file inside CMakeFiles
            if 'CMakeFiles' in src_file.split(os.sep):
                continue
            if os.path.isfile(src_file):
                rel_path = os.path.relpath(src_file, src)
                dst_file = os.path.join(dst, rel_path)
                os.makedirs(os.path.dirname(dst_file), exist_ok=True)
                shutil.copy2(src_file, dst_file)
    else:
        os.makedirs(os.path.dirname(dst), exist_ok=True)
        shutil.copy2(src, dst)

for job in copy_jobs:
    src, dst, wildcard, filter_func = job
    if os.path.exists(src):
        copy_with_filter(src, dst, wildcard, filter_func)
")

# Configure the script into the build directory at every configure step
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/copy_delivery.py.in"
    "${CMAKE_CURRENT_BINARY_DIR}/copy_delivery.py"
    @ONLY
)

# Always remove the stamp file at configure time to force the copy at every build
file(REMOVE "${CMAKE_CURRENT_BINARY_DIR}/delivery/.copy_delivery_stamp")

add_custom_command(
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/delivery/.copy_delivery_stamp"
    COMMAND python3 "${CMAKE_CURRENT_BINARY_DIR}/copy_delivery.py"
    #COMMAND ${CMAKE_COMMAND} -E touch "${CMAKE_CURRENT_BINARY_DIR}/delivery/.copy_delivery_stamp"
    #COMMAND ${CMAKE_COMMAND} -E rm -f "${CMAKE_CURRENT_BINARY_DIR}/copy_delivery.py"
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