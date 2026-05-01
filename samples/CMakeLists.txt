# CMakeLists.txt for samples

set(SRCS_SAMPLE
	sample_speedometer.cpp
	dialog.cpp
	icon_menu_dialog.cpp
	menu_dialog.cpp
	samples.cpp
	sample_button.cpp
	sample_column.cpp
	sample_row.cpp
	sample_canvas.cpp
	sample_progress_bar.cpp
	sample_slider.cpp
	sample_grid.cpp
	sample_label.cpp
	sample_edit.cpp
	sample_icon.cpp
	sample_pane.cpp
	sample_switch.cpp
	sample_keyboard.cpp
	sample_scrollview.cpp
	sample_theme.cpp
	sample_forms.cpp
	sample_window.cpp
	sample_hello_world.cpp
)

# Static library for samples (used by pybind)
add_library(${BD_SAMPLE_LIB} STATIC ${SRCS_SAMPLE})
target_include_directories(${BD_SAMPLE_LIB} PRIVATE ${INCLUDES_LIB})
target_compile_definitions(${BD_SAMPLE_LIB} PRIVATE ${COMPILE_DEF})
target_link_libraries(${BD_SAMPLE_LIB} PRIVATE ${BD_LIB} ${DEVICE_LIBRARIES})

# Unitary test application
add_executable(${BD_APP_TEST} ../src/main.cpp ../src/tool/test.cpp ${SRCS_SAMPLE})
target_include_directories(${BD_APP_TEST} PRIVATE ${INCLUDES_LIB})
target_compile_definitions(${BD_APP_TEST} PRIVATE ${COMPILE_DEF})
target_link_libraries(${BD_APP_TEST} PRIVATE ${BD_LIB} ${DEVICE_LIBRARIES})

# Sample application
add_executable(${BD_APP_SAMPLE} main.cpp ${SRCS_SAMPLE})
target_include_directories(${BD_APP_SAMPLE} PRIVATE ${INCLUDES_LIB})
target_compile_definitions(${BD_APP_SAMPLE} PRIVATE ${COMPILE_DEF})
target_link_libraries(${BD_APP_SAMPLE} PRIVATE ${BD_LIB} ${DEVICE_LIBRARIES})
