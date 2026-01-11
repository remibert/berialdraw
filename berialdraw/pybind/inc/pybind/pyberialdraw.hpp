#pragma once
// Berialdraw Python Bindings - Master Header
// This header contains all binding declarations for the berialdraw framework

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <functional>
#include "berialdraw.hpp"

namespace py = pybind11;

// Generic helper templates to factorize repeated property bindings
// These avoid lambda capture issues by using member pointers directly and a simple setter lambda

// Helper: bind a property that returns a 2-element tuple (x, y) from a Point-like getter
// and accepts tuple/list setter
template<typename C, typename... Extra>
void bind_point_property(pybind11::class_<C, Extra...>& cls, const char* name,
                         const berialdraw::Point& (C::*getter)() const,
                         void (C::*setter)(berialdraw::Coord, berialdraw::Coord),
                         const char* doc) {
    cls.def_property(name,
        [getter](C& self) -> py::tuple {
            const auto& p = (self.*getter)();
            return py::make_tuple(p.x(), p.y());
        },
        [setter](C& self, py::object value) {
            if (py::isinstance<py::tuple>(value) || py::isinstance<py::list>(value)) {
                auto seq = value.cast<py::sequence>();
                if (py::len(seq) == 2) {
                    (self.*setter)(seq[0].cast<berialdraw::Coord>(), seq[1].cast<berialdraw::Coord>());
                } else {
                    throw std::invalid_argument("Point property must be tuple/list of 2 values (x, y)");
                }
            } else {
                throw std::invalid_argument("Point property must be tuple/list of 2 values");
            }
        }, doc);
}

// Helper: bind a property that returns a 2-element tuple (width, height) from a Size-like getter
// and accepts tuple/list setter or single value
template<typename C, typename... Extra>
void bind_size_property(pybind11::class_<C, Extra...>& cls, const char* name,
                        const berialdraw::Size& (C::*getter)() const,
                        void (C::*setter)(berialdraw::Dim, berialdraw::Dim),
                        const char* doc) {
    cls.def_property(name,
        [getter](C& self) -> py::tuple {
            const auto& s = (self.*getter)();
            return py::make_tuple(s.width(), s.height());
        },
        [setter](C& self, py::object value) {
            if (py::isinstance<py::int_>(value) || py::isinstance<py::float_>(value)) {
                auto dim = value.cast<berialdraw::Dim>();
                (self.*setter)(dim, dim);
            } else if (py::isinstance<py::tuple>(value) || py::isinstance<py::list>(value)) {
                auto seq = value.cast<py::sequence>();
                if (py::len(seq) == 2) {
                    (self.*setter)(seq[0].cast<berialdraw::Dim>(), seq[1].cast<berialdraw::Dim>());
                } else {
                    throw std::invalid_argument("Size property tuple/list must have 2 values (width, height)");
                }
            } else {
                throw std::invalid_argument("Size property must be int/float or tuple/list of 2 values");
            }
        }, doc);
}

// Helper: bind a property that returns a 4-element tuple (top,right,bottom,left) from a Margin-like getter
// and accepts single value or tuple/list setter
template<typename C, typename... Extra>
void bind_margin_property(pybind11::class_<C, Extra...>& cls, const char* name,
                          const berialdraw::Margin& (C::*getter)() const,
                          void (C::*setter1)(berialdraw::Dim),
                          void (C::*setter2)(berialdraw::Dim, berialdraw::Dim),
                          void (C::*setter4)(berialdraw::Dim, berialdraw::Dim, berialdraw::Dim, berialdraw::Dim),
                          const char* doc) {
    cls.def_property(name,
        [getter](C& self) -> py::tuple {
            const auto& m = (self.*getter)();
            return py::make_tuple(m.top(), m.right(), m.bottom(), m.left());
        },
        [setter1, setter2, setter4](C& self, py::object value) {
            if (py::isinstance<py::int_>(value) || py::isinstance<py::float_>(value)) {
                auto dim = value.cast<berialdraw::Dim>();
                (self.*setter1)(dim);
            } else if (py::isinstance<py::tuple>(value) || py::isinstance<py::list>(value)) {
                auto seq = value.cast<py::sequence>();
                auto len = py::len(seq);
                if (len == 2) {
                    (self.*setter2)(seq[0].cast<berialdraw::Dim>(), seq[1].cast<berialdraw::Dim>());
                } else if (len == 4) {
                    (self.*setter4)(seq[0].cast<berialdraw::Dim>(), seq[1].cast<berialdraw::Dim>(),
                                   seq[2].cast<berialdraw::Dim>(), seq[3].cast<berialdraw::Dim>());
                } else {
                    throw std::invalid_argument("Margin property tuple/list must have 2 or 4 values");
                }
            } else {
                throw std::invalid_argument("Margin property must be int/float or tuple/list");
            }
        }, doc);
}

// Helper: bind a property that accepts both uint32_t color values and Color enum
// Accepts both 0xFFFFFFFF and Color.RED style values
template<typename C, typename... Extra>
void bind_color_property(pybind11::class_<C, Extra...>& cls, const char* name,
                         uint32_t (C::*getter)() const,
                         void (C::*setter)(uint32_t),
                         const char* doc) {
    cls.def_property(name,
        [getter](C& self) -> uint32_t { return (self.*getter)(); },
        [setter](C& self, const pybind11::object& value) {
            if (pybind11::isinstance<pybind11::int_>(value)) {
                (self.*setter)(pybind11::cast<uint32_t>(value));
            } else {
                try {
                    auto color_enum = pybind11::cast<berialdraw::Color>(value);
                    (self.*setter)(static_cast<uint32_t>(color_enum));
                } catch (const pybind11::cast_error&) {
                    throw std::invalid_argument("Color property must be an integer or Color enum value");
                }
            }
        }, doc);
}

// Helper: convert Python string to berialdraw::String
inline berialdraw::String py_to_string(const pybind11::object& value) {
    if (pybind11::isinstance<pybind11::str>(value)) {
        std::string str = pybind11::cast<std::string>(value);
        return berialdraw::String(str.c_str());
    } else if (pybind11::isinstance<berialdraw::String>(value)) {
        return pybind11::cast<berialdraw::String>(value);
    } else {
        throw std::invalid_argument("Expected string or berialdraw.String");
    }
}

// Font bindings 
void bind_font(py::module& m);
void bind_fonts(py::module& m);

// Tool bindings
void bind_string(py::module& m);
void bind_file(py::module& m);
void bind_directory(py::module& m);
void bind_json(py::module& m);
void bind_settings(py::module& m);

// Vector bindings
void bind_point(py::module& m);
void bind_size(py::module& m);
void bind_area(py::module& m);
void bind_margin(py::module& m);
void bind_size_policy(py::module& m);
void bind_extend(py::module& m);
void bind_align(py::module& m);

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
void bind_icon_style(pybind11::module_& m);
void bind_round_style(py::module& m);
void bind_slider_style(pybind11::module_& m);
void bind_switch_style(pybind11::module_& m);
void bind_progress_bar_style(pybind11::module_& m);
void bind_edit_style(pybind11::module_& m);
void bind_pie_style(pybind11::module_& m);
void bind_scroll_view_style(pybind11::module_& m);

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
void bind_canvas(pybind11::module& m);
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