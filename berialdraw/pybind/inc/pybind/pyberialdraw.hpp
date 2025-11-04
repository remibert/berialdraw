#pragma once
// Berialdraw Python Bindings - Master Header
// This header contains all binding declarations for the berialdraw framework

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "berialdraw.hpp"

namespace py = pybind11;

// Font bindings
void bind_font(py::module& m);
void bind_fonts(py::module& m);

// Tool bindings
void bind_string(py::module& m);
void bind_file(py::module& m);
void bind_directory(py::module& m);


// Vector bindings
void bind_point(py::module& m);
void bind_size(py::module& m);
void bind_area(py::module& m);
void bind_margin(py::module& m);
void bind_size_policy(py::module& m);

// Style bindings
void bind_style(py::module& m);
void bind_styles(py::module& m);
void bind_color(py::module& m);
void bind_colors(py::module& m);
void bind_common_style(pybind11::module_& m);
void bind_widget_style(pybind11::module_& m);
void bind_text_style(pybind11::module_& m);
void bind_border_style(pybind11::module_& m);
void bind_line_style(py::module& m);
void bind_icon_style(py::module& m);
void bind_round_style(py::module& m);
void bind_slider_style(py::module& m);
void bind_switch_style(py::module& m);
void bind_progress_bar_style(py::module& m);
void bind_edit_style(py::module& m);
void bind_pie_style(py::module& m);

// Framebuf bindings
void bind_framebuf(py::module& m);
void bind_argb8888(py::module& m);

// Device bindings
void bind_device(py::module& m);
void bind_device_screen(py::module& m);

// Shape bindings
void bind_shape(py::module& m);
void bind_polygon(py::module& m);
void bind_marker(py::module& m);
void bind_rect(py::module& m);
void bind_line(py::module& m);
void bind_circle(py::module& m);
void bind_triangle(py::module& m);
void bind_square(py::module& m);
void bind_cross(py::module& m);
void bind_text(py::module& m);
void bind_pie(py::module& m);
void bind_compass(py::module& m);
void bind_poly_lines(py::module& m);
void bind_poly_points(py::module& m);
void bind_sketch(py::module& m);

// Event bindings
void bind_event(pybind11::module_& m);
void bind_click_event(pybind11::module_& m);
void bind_key_event(pybind11::module_& m);
void bind_check_event(pybind11::module_& m);
void bind_select_event(pybind11::module_& m);
void bind_slide_event(pybind11::module_& m);
void bind_scroll_event(pybind11::module_& m);
void bind_focus_event(pybind11::module_& m);
void bind_touch_event(pybind11::module_& m);
void bind_event_managers(pybind11::module_& m);
void bind_notifier(pybind11::module_& m);

// Widget bindings
void bind_widget(pybind11::module_& m);
void bind_button(pybind11::module_& m);
void bind_label(pybind11::module_& m);
void bind_window(pybind11::module_& m);
void bind_canvas(pybind11::module_& m);
void bind_entry(pybind11::module_& m);
void bind_edit(pybind11::module_& m);
void bind_slider(pybind11::module_& m);
void bind_progress_bar(pybind11::module_& m);
void bind_row(pybind11::module_& m);
void bind_column(pybind11::module_& m);
void bind_switch(pybind11::module_& m);
void bind_grid(pybind11::module_& m);
void bind_pane(pybind11::module_& m);
void bind_scroll_view(pybind11::module_& m);
void bind_icon(pybind11::module_& m);
void bind_keyboard(pybind11::module_& m);
void bind_desktop(pybind11::module_& m);
void bind_uimanager(pybind11::module_& m);