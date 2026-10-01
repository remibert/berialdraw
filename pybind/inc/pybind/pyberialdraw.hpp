#pragma once
// Berialdraw Python Bindings - Master Header
// This header contains all binding declarations for the berialdraw framework

// Include Python headers
#include <Python.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <functional>
#include <cmath>
#include <cstdarg>
#include <cstdio>
#include <string>
#include "berialdraw.hpp"

namespace py = pybind11;

// ============================================================================
// GIL Management Macro
// ============================================================================
// Release the GIL during C++ calls to allow other Python threads to run
// Usage: .def("method", &Class::method, PYBIND11_RELEASE_GIL)
#define PYBIND11_RELEASE_GIL py::call_guard<py::gil_scoped_release>()

// ============================================================================
// Helper Templates
// ============================================================================

// Generic helper templates to factorize repeated property bindings
// These avoid lambda capture issues by using member pointers directly and a simple setter lambda

// Python values: int = pixels, float = precise value (stored in 64th of a pixel)

// Convert a float to a value in 64th of a unit
inline int32_t to_q6(double value) {
    return static_cast<int32_t>(std::lround(value * 64.0));
}

// Convert a Python int/float to a value in 64th of a unit
inline int32_t py_to_q6(const py::handle& value) {
    return to_q6(value.cast<double>());
}

// True if the Python value is a float
inline bool py_is_float(const py::handle& value) {
    return py::isinstance<py::float_>(value);
}

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
            if (py::isinstance<berialdraw::Point>(value)) {
                const auto& p = value.cast<const berialdraw::Point&>();
                (self.*setter)(p.x(), p.y());
            } else if (py::isinstance<berialdraw::Size>(value)) {
                // Size -> Point implicit conversion (width/height as x/y), like C++'s Point(const Size&)
                berialdraw::Point p(value.cast<const berialdraw::Size&>());
                (self.*setter)(p.x(), p.y());
            } else if (py::isinstance<py::tuple>(value) || py::isinstance<py::list>(value)) {
                auto seq = value.cast<py::sequence>();
                if (py::len(seq) == 2) {
                    (self.*setter)(seq[0].cast<berialdraw::Coord>(), seq[1].cast<berialdraw::Coord>());
                } else {
                    throw std::invalid_argument("Point property must be tuple/list of 2 values (x, y)");
                }
            } else {
                throw std::invalid_argument("Point property must be a Point, a Size, or tuple/list of 2 values");
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

// Helper: Point property with precise (float) support
// A float in the tuple selects the 64th of a pixel setter
template<typename C, typename... Extra>
void bind_point_property(pybind11::class_<C, Extra...>& cls, const char* name,
                         const berialdraw::Point& (C::*getter)() const,
                         void (C::*setter)(berialdraw::Coord, berialdraw::Coord),
                         void (C::*setter_q6)(berialdraw::Coord, berialdraw::Coord),
                         const char* doc) {
    cls.def_property(name,
        [getter](C& self) -> py::tuple {
            const auto& p = (self.*getter)();
            return py::make_tuple(p.x(), p.y());
        },
        [setter, setter_q6](C& self, py::object value) {
            if (py::isinstance<berialdraw::Point>(value)) {
                const auto& p = value.cast<const berialdraw::Point&>();
                (self.*setter_q6)(p.x_q6(), p.y_q6());
            } else if (py::isinstance<berialdraw::Size>(value)) {
                berialdraw::Point p(value.cast<const berialdraw::Size&>());
                (self.*setter_q6)(p.x_q6(), p.y_q6());
            } else if (py::isinstance<py::tuple>(value) || py::isinstance<py::list>(value)) {
                auto seq = value.cast<py::sequence>();
                if (py::len(seq) != 2) {
                    throw std::invalid_argument("Point property must be tuple/list of 2 values (x, y)");
                }
                if (py_is_float(seq[0]) || py_is_float(seq[1])) {
                    (self.*setter_q6)(py_to_q6(seq[0]), py_to_q6(seq[1]));
                } else {
                    (self.*setter)(seq[0].cast<berialdraw::Coord>(), seq[1].cast<berialdraw::Coord>());
                }
            } else {
                throw std::invalid_argument("Point property must be a Point, a Size, or tuple/list of 2 values");
            }
        }, doc);
}

// Helper: Size property with precise (float) support
// A float selects the 64th of a pixel setter
template<typename C, typename... Extra>
void bind_size_property(pybind11::class_<C, Extra...>& cls, const char* name,
                        const berialdraw::Size& (C::*getter)() const,
                        void (C::*setter)(berialdraw::Dim, berialdraw::Dim),
                        void (C::*setter_q6)(berialdraw::Dim, berialdraw::Dim),
                        const char* doc) {
    cls.def_property(name,
        [getter](C& self) -> py::tuple {
            const auto& s = (self.*getter)();
            return py::make_tuple(s.width(), s.height());
        },
        [setter, setter_q6](C& self, py::object value) {
            if (py::isinstance<berialdraw::Size>(value)) {
                const auto& s = value.cast<const berialdraw::Size&>();
                (self.*setter_q6)(s.width_q6(), s.height_q6());
            } else if (py::isinstance<py::float_>(value)) {
                auto dim = static_cast<berialdraw::Dim>(py_to_q6(value));
                (self.*setter_q6)(dim, dim);
            } else if (py::isinstance<py::int_>(value)) {
                auto dim = value.cast<berialdraw::Dim>();
                (self.*setter)(dim, dim);
            } else if (py::isinstance<py::tuple>(value) || py::isinstance<py::list>(value)) {
                auto seq = value.cast<py::sequence>();
                if (py::len(seq) != 2) {
                    throw std::invalid_argument("Size property tuple/list must have 2 values (width, height)");
                }
                if (py_is_float(seq[0]) || py_is_float(seq[1])) {
                    (self.*setter_q6)(static_cast<berialdraw::Dim>(py_to_q6(seq[0])), static_cast<berialdraw::Dim>(py_to_q6(seq[1])));
                } else {
                    (self.*setter)(seq[0].cast<berialdraw::Dim>(), seq[1].cast<berialdraw::Dim>());
                }
            } else {
                throw std::invalid_argument("Size property must be int/float, Size or tuple/list of 2 values");
            }
        }, doc);
}

// Helper: Margin property with precise (float) support
// A float selects the 64th of a pixel setter; 2 values are (horizontal, vertical), 4 values are (top, left, bottom, right)
template<typename C, typename... Extra>
void bind_margin_property(pybind11::class_<C, Extra...>& cls, const char* name,
                          const berialdraw::Margin& (C::*getter)() const,
                          void (C::*setter1)(berialdraw::Dim),
                          void (C::*setter2)(berialdraw::Dim, berialdraw::Dim),
                          void (C::*setter4)(berialdraw::Dim, berialdraw::Dim, berialdraw::Dim, berialdraw::Dim),
                          void (C::*setter4_q6)(berialdraw::Dim, berialdraw::Dim, berialdraw::Dim, berialdraw::Dim),
                          const char* doc) {
    cls.def_property(name,
        [getter](C& self) -> py::tuple {
            const auto& m = (self.*getter)();
            return py::make_tuple(m.top(), m.right(), m.bottom(), m.left());
        },
        [setter1, setter2, setter4, setter4_q6](C& self, py::object value) {
            using Dim = berialdraw::Dim;
            if (py::isinstance<py::float_>(value)) {
                auto v = static_cast<Dim>(py_to_q6(value));
                (self.*setter4_q6)(v, v, v, v);
            } else if (py::isinstance<py::int_>(value)) {
                (self.*setter1)(value.cast<Dim>());
            } else if (py::isinstance<py::tuple>(value) || py::isinstance<py::list>(value)) {
                auto seq = value.cast<py::sequence>();
                auto len = py::len(seq);
                bool precise = false;
                for (size_t i = 0; i < len; i++) {
                    precise = precise || py_is_float(seq[i]);
                }
                if (len == 2) {
                    if (precise) {
                        auto h = static_cast<Dim>(py_to_q6(seq[0]));
                        auto v = static_cast<Dim>(py_to_q6(seq[1]));
                        (self.*setter4_q6)(v, h, v, h);
                    } else {
                        (self.*setter2)(seq[0].cast<Dim>(), seq[1].cast<Dim>());
                    }
                } else if (len == 4) {
                    if (precise) {
                        (self.*setter4_q6)(static_cast<Dim>(py_to_q6(seq[0])), static_cast<Dim>(py_to_q6(seq[1])),
                                           static_cast<Dim>(py_to_q6(seq[2])), static_cast<Dim>(py_to_q6(seq[3])));
                    } else {
                        (self.*setter4)(seq[0].cast<Dim>(), seq[1].cast<Dim>(), seq[2].cast<Dim>(), seq[3].cast<Dim>());
                    }
                } else {
                    throw std::invalid_argument("Margin property tuple/list must have 2 or 4 values");
                }
            } else {
                throw std::invalid_argument("Margin property must be int/float or tuple/list");
            }
        }, doc);
}

// Helper: bind a property that accepts int/float or tuple/list and creates a Size object
// Used when setter takes a Size object instead of (Dim, Dim)
template<typename C, typename... Extra>
void bind_size_from_values_property(pybind11::class_<C, Extra...>& cls, const char* name,
                                    const berialdraw::Size& (C::*getter)() const,
                                    void (C::*setter)(const berialdraw::Size&),
                                    const char* doc) {
    cls.def_property(name,
        [getter](C& self) -> py::tuple {
            const auto& s = (self.*getter)();
            return py::make_tuple(s.width(), s.height());
        },
        [setter](C& self, py::object value) {
            if (py::isinstance<py::int_>(value) || py::isinstance<py::float_>(value)) {
                auto dim = value.cast<berialdraw::Dim>();
                (self.*setter)(berialdraw::Size(dim, dim));
            } else if (py::isinstance<py::tuple>(value) || py::isinstance<py::list>(value)) {
                auto seq = value.cast<py::sequence>();
                if (py::len(seq) == 2) {
                    (self.*setter)(berialdraw::Size(seq[0].cast<berialdraw::Dim>(), seq[1].cast<berialdraw::Dim>()));
                } else {
                    throw std::invalid_argument("Size property tuple/list must have 2 values (width, height)");
                }
            } else {
                throw std::invalid_argument("Size property must be int/float or tuple/list of 2 values");
            }
        }, doc);
}

// Helper: bind a property that accepts tuple/list and creates a Point object
// Used when setter takes a Point object instead of (Coord, Coord)
template<typename C, typename... Extra>
void bind_point_from_values_property(pybind11::class_<C, Extra...>& cls, const char* name,
                                     const berialdraw::Point& (C::*getter)() const,
                                     void (C::*setter)(const berialdraw::Point&),
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
                    (self.*setter)(berialdraw::Point(seq[0].cast<berialdraw::Coord>(), seq[1].cast<berialdraw::Coord>()));
                } else {
                    throw std::invalid_argument("Point property must be tuple/list of 2 values (x, y)");
                }
            } else {
                throw std::invalid_argument("Point property must be tuple/list of 2 values");
            }
        }, doc);
}

// Helper: bind a color property accepting int, Color enum, or a (color, alpha) tuple/list
// e.g. widget.color = Color.RED, 0xFFFF0000, or (Color.RED, 127)
template<typename C, typename... Extra>
void bind_color_property(pybind11::class_<C, Extra...>& cls, const char* name,
                         uint32_t (C::*getter)() const,
                         void (C::*setter)(uint32_t),
                         void (C::*setter_alpha)(uint32_t, uint8_t),
                         const char* doc) {
    auto to_color = [](const pybind11::object& v) -> uint32_t {
        uint32_t result = 0;
        if (pybind11::isinstance<pybind11::int_>(v)) {
            result = pybind11::cast<uint32_t>(v);
        } else {
            try {
                result = static_cast<uint32_t>(pybind11::cast<berialdraw::Color>(v));
            } catch (const pybind11::cast_error&) {
                throw std::invalid_argument("Color property must be an integer or Color enum value");
            }
        }
        return result;
    };
    cls.def_property(name,
        [getter](C& self) -> uint32_t { return (self.*getter)(); },
        [setter, setter_alpha, to_color](C& self, const pybind11::object& value) {
            if (pybind11::isinstance<pybind11::tuple>(value) || pybind11::isinstance<pybind11::list>(value)) {
                auto seq = value.cast<pybind11::sequence>();
                if (pybind11::len(seq) != 2 || setter_alpha == nullptr) {
                    throw std::invalid_argument("Color property tuple must be (color, alpha)");
                }
                (self.*setter_alpha)(to_color(seq[0]), seq[1].cast<uint8_t>());
            } else {
                (self.*setter)(to_color(value));
            }
        }, doc);
}

// Same as above for colors without alpha setter
template<typename C, typename... Extra>
void bind_color_property(pybind11::class_<C, Extra...>& cls, const char* name,
                         uint32_t (C::*getter)() const,
                         void (C::*setter)(uint32_t),
                         const char* doc) {
    bind_color_property(cls, name, getter, setter, static_cast<void (C::*)(uint32_t, uint8_t)>(nullptr), doc);
}

// Helper: bind a property that accepts int (normal precision) or float (1/64th precision)
// Used for Dim and Coord types where float * 64 gives high precision
template<typename C, typename ValueType, typename... Extra>
void bind_precision_property(pybind11::class_<C, Extra...>& cls, const char* name,
                             ValueType (C::*getter)() const,
                             void (C::*setter_normal)(ValueType),
                             void (C::*setter_precision)(ValueType),
                             const char* doc) {
    cls.def_property(name,
        [getter](C& self) -> ValueType { return (self.*getter)(); },
        [setter_normal, setter_precision](C& self, pybind11::object value) {
            if (pybind11::isinstance<pybind11::int_>(value)) {
                (self.*setter_normal)(value.cast<ValueType>());
            } else if (pybind11::isinstance<pybind11::float_>(value)) {
                (self.*setter_precision)(static_cast<ValueType>(py_to_q6(value)));
            } else {
                throw std::invalid_argument("Property must be int (normal) or float (high precision)");
            }
        }, doc);
}

// Helper: bind a simple scalar property (getter/setter with same type)
// Used for properties like Dim, int32_t, uint32_t, bool, etc.
template<typename C, typename ValueType, typename... Extra>
void bind_scalar_property(pybind11::class_<C, Extra...>& cls, const char* name,
                         ValueType (C::*getter)() const,
                         void (C::*setter)(ValueType),
                         const char* doc) {
    cls.def_property(name,
        [getter](C& self) -> ValueType { return (self.*getter)(); },
        [setter](C& self, ValueType value) { (self.*setter)(value); },
        doc);
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
void bind_clipboard(py::module& m);

// Vector bindings
void bind_point(py::module& m);
void bind_size(py::module& m);
void bind_area(py::module& m);
void bind_margin(py::module& m);
void bind_size_policy(py::module& m);
void bind_extend(py::module& m);
void bind_align(py::module& m);
void bind_image_fit_mode(py::module& m);
void bind_scroll_direction(py::module& m);

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
void bind_checkbox_style(pybind11::module_& m);
void bind_radio_style(pybind11::module_& m);
void bind_progress_bar_style(pybind11::module_& m);
void bind_edit_style(pybind11::module_& m);
void bind_pie_style(pybind11::module_& m);
void bind_scroll_view_style(pybind11::module_& m);
void bind_scrollbar_style(pybind11::module_& m);
void bind_table_view_style(pybind11::module_& m);
void bind_grid_style(pybind11::module_& m);
void bind_cell_style(pybind11::module_& m);
void bind_cells_style(pybind11::module_& m);
void bind_picture_style(pybind11::module_& m);
void bind_timer_style(pybind11::module_& m);
void bind_list_style(pybind11::module_& m);
void bind_list_selection_mode(py::module_& m);
void bind_padding_style(pybind11::module_& m);
void bind_list_item_style(pybind11::module_& m);

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
void bind_star(py::module& m);
void bind_text(py::module& m);
void bind_pie(py::module& m);
void bind_compass(py::module& m);
void bind_poly_lines(py::module& m);
void bind_poly_points(py::module& m);
void bind_sketch(py::module& m);
void bind_image(py::module& m);

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
void bind_timer_event(pybind11::module_& m);
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
void bind_checkbox(pybind11::module_& m);
void bind_radio(pybind11::module_& m);
void bind_grid(pybind11::module_& m);
void bind_pane(pybind11::module_& m);
void bind_scrollable_content(pybind11::module_& m);
void bind_scroll_view(pybind11::module_& m);
void bind_table_view(pybind11::module_& m);
void bind_list(pybind11::module_& m);
void bind_list_item(pybind11::module_& m);
void bind_icon(pybind11::module_& m);
void bind_picture(pybind11::module_& m);
void bind_keyboard(pybind11::module_& m);
void bind_desktop(pybind11::module_& m);
void bind_timer(pybind11::module_& m);
void bind_uimanager(pybind11::module_& m);
void redirect_print();
