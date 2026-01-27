# CMakeLists.txt for pybind11 Python wrapper

# Python setup for pybind11
if(WIN32)
	find_package(Python COMPONENTS Interpreter Development REQUIRED)
	set(PYTHON_INCLUDE_PATH "C:/Development/Python314/include")
	set(PYTHON_LIBRARIES "C:/Development/Python314/libs/python314.lib")
elseif(APPLE)
	find_package(Python COMPONENTS Interpreter Development REQUIRED)
	set(PYTHON_INCLUDE_PATH ${Python_INCLUDE_DIRS})
	set(PYTHON_LIBRARIES ${Python_LIBRARIES})
else()
	find_package(Python COMPONENTS Interpreter Development REQUIRED)
	set(PYTHON_INCLUDE_PATH ${Python_INCLUDE_DIRS})
	set(PYTHON_LIBRARIES ${Python_LIBRARIES})
endif()

set(SRCS_PYBIND11_LIB
	src/pyberialdraw.cpp
	src/pybind/styles/colors.cpp
	src/pybind/styles/styles.cpp
	src/pybind/device/device.cpp
	src/pybind/device/device_screen.cpp
	src/pybind/event/touch_event.cpp
	src/pybind/event/event.cpp
	src/pybind/event/scroll_event.cpp
	src/pybind/event/check_event.cpp
	src/pybind/event/select_event.cpp
	src/pybind/event/notifier.cpp
	src/pybind/event/click_event.cpp
	src/pybind/event/focus_event.cpp
	src/pybind/event/slide_event.cpp
	src/pybind/event/key_event.cpp
	src/pybind/event/event_managers.cpp
	src/pybind/font/font.cpp
	src/pybind/font/fonts.cpp
	src/pybind/vector/size.cpp
	src/pybind/vector/size_policy.cpp
	src/pybind/vector/extend.cpp
	src/pybind/vector/align.cpp
	src/pybind/vector/point.cpp
	src/pybind/vector/margin.cpp
	src/pybind/vector/area.cpp
	src/pybind/framebuf/framebuf.cpp
	src/pybind/framebuf/argb8888.cpp
	src/pybind/shape/circle.cpp
	src/pybind/shape/compass.cpp
	src/pybind/shape/cross.cpp
	src/pybind/shape/line.cpp
	src/pybind/shape/marker.cpp
	src/pybind/shape/pie.cpp
	src/pybind/shape/polygon.cpp
	src/pybind/shape/poly_lines.cpp
	src/pybind/shape/poly_points.cpp
	src/pybind/shape/rect.cpp
	src/pybind/shape/shape.cpp
	src/pybind/shape/sketch.cpp
	src/pybind/shape/square.cpp
	src/pybind/shape/text.cpp
	src/pybind/shape/triangle.cpp
	src/pybind/styles/border_style.cpp
	src/pybind/styles/color.cpp
	src/pybind/styles/common_style.cpp
	src/pybind/styles/edit_style.cpp
	src/pybind/styles/icon_style.cpp
	src/pybind/styles/line_style.cpp
	src/pybind/styles/pie_style.cpp
	src/pybind/styles/progress_bar_style.cpp
	src/pybind/styles/round_style.cpp
	src/pybind/styles/scroll_view_style.cpp
	src/pybind/styles/slider_style.cpp
	src/pybind/styles/style.cpp
	src/pybind/styles/switch_style.cpp
	src/pybind/styles/checkbox_style.cpp
	src/pybind/styles/text_style.cpp
	src/pybind/styles/widget_style.cpp
	src/pybind/tool/directory.cpp
	src/pybind/tool/file.cpp
	src/pybind/tool/string.cpp
	src/pybind/tool/json.cpp
	src/pybind/tool/settings.cpp
	src/pybind/tool/bd_system.cpp
	src/pybind/widget/widget.cpp
	src/pybind/widget/button.cpp
	src/pybind/widget/canvas.cpp
	src/pybind/widget/column.cpp
	src/pybind/widget/desktop.cpp
	src/pybind/widget/edit.cpp
	src/pybind/widget/entry.cpp
	src/pybind/widget/grid.cpp
	src/pybind/widget/icon.cpp
	src/pybind/widget/keyboard.cpp
	src/pybind/widget/label.cpp
	src/pybind/widget/pane.cpp
	src/pybind/widget/progress_bar.cpp
	src/pybind/widget/row.cpp
	src/pybind/widget/scroll_view.cpp
	src/pybind/widget/slider.cpp
	src/pybind/widget/switch.cpp
	src/pybind/widget/checkbox.cpp
	src/pybind/widget/uimanager.cpp
	src/pybind/widget/window.cpp
)

# Pybind headers to ensure CMake tracks header dependencies
set(PYBIND_HEADERS
	inc/pybind/pyberialdraw.hpp
	inc/pybind/event_system.hpp
)

# Define pybind includes locally
set(INCLUDES_PYBIND11
	${SDL3_INCLUDE_DIRS}
	"${CMAKE_CURRENT_SOURCE_DIR}/../inc;"
	"${CMAKE_CURRENT_SOURCE_DIR}/../inc/freetype;"
	"${CMAKE_CURRENT_SOURCE_DIR}/../freetype/include;"
	"${CMAKE_CURRENT_SOURCE_DIR}/../freetype/include/freetype;"
	"${CMAKE_CURRENT_SOURCE_DIR}/inc;"
	"${CMAKE_CURRENT_SOURCE_DIR}/.."
	"${CMAKE_CURRENT_SOURCE_DIR}/../.."
	"${CMAKE_CURRENT_SOURCE_DIR}/../../pybind11/include;"
)

# pybind11 Python wrapper library
add_library(${BD_PY11_LIB} SHARED ${SRCS_PYBIND11_LIB} ${PYBIND_HEADERS})
target_include_directories(${BD_PY11_LIB} PRIVATE ${INCLUDES_PYBIND11} ${PYTHON_INCLUDE_PATH})
target_compile_definitions(${BD_PY11_LIB} PRIVATE ${COMPILE_DEF} 
	-DPACKAGE=1
	-DHAVE_PTHREAD_SETSPECIFIC=1
	-DFT2_BUILD_LIBRARY=1
	-DFT_ERR_PREFIX=FT_Err_)
if(APPLE)
	target_compile_definitions(${BD_PY11_LIB} PRIVATE -DVERSION_INFO=\"dev\")
	set_target_properties(${BD_PY11_LIB} PROPERTIES
		CXX_VISIBILITY_PRESET "hidden"
		VISIBILITY_INLINES_HIDDEN ON
	)
endif()

# Link against the static library
target_link_libraries(${BD_PY11_LIB} ${BD_LIB} ${DEVICE_LIBRARIES} ${PYTHON_LIBRARIES} ${BD_SAMPLE_LIB})

# Platform-specific output settings
if(WIN32)
	set_target_properties(${BD_PY11_LIB} PROPERTIES SUFFIX ".pyd")
elseif(APPLE)
	set_target_properties(${BD_PY11_LIB} PROPERTIES SUFFIX ".so" PREFIX "")
else()
	set_target_properties(${BD_PY11_LIB} PROPERTIES SUFFIX ".so" PREFIX "")
endif()

# Add Info.plist for macOS GUI app recognition
if(APPLE AND USE_COCOA_DEVICE)
	set_target_properties(${BD_PY11_LIB} PROPERTIES
		MACOSX_BUNDLE TRUE
		MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_BINARY_DIR}/Info.plist
	)
endif()
