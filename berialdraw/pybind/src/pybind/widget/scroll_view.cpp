#include "pybind/pyberialdraw.hpp"
#include "pybind/event_system.hpp"

void bind_scroll_view(pybind11::module_& m) {
    pybind11::class_<berialdraw::ScrollView, berialdraw::ScrollableContent> cls(m, "ScrollView");

    cls.def(pybind11::init<berialdraw::Widget*>(), pybind11::return_value_policy::reference_internal, pybind11::keep_alive<1, 2>());

    // Propriétés avec support tuples - définies directement sans helpers
    cls.def_property("scroll_size",
        [](berialdraw::ScrollView& self) -> py::tuple {
            const auto& s = self.scroll_size();
            return py::make_tuple(s.width(), s.height());
        },
        [](berialdraw::ScrollView& self, py::object value) {
            if (py::isinstance<py::tuple>(value) || py::isinstance<py::list>(value)) {
                auto seq = value.cast<py::sequence>();
                if (py::len(seq) == 2) {
                    self.scroll_size(seq[0].cast<berialdraw::Dim>(), seq[1].cast<berialdraw::Dim>());
                } else {
                    throw std::invalid_argument("Size property must be tuple/list of 2 values");
                }
            } else if (py::isinstance<py::int_>(value) || py::isinstance<py::float_>(value)) {
                auto d = value.cast<berialdraw::Dim>();
                self.scroll_size(d, d);
            } else {
                throw std::invalid_argument("Size property must be int or tuple/list of 2 values");
            }
        },
        "Scroll size: int (square) or (width, height)");

    cls.def_property("scroll_position",
        [](berialdraw::ScrollView& self) -> py::tuple {
            const auto& p = self.scroll_position();
            return py::make_tuple(p.x(), p.y());
        },
        [](berialdraw::ScrollView& self, py::object value) {
            if (py::isinstance<py::tuple>(value) || py::isinstance<py::list>(value)) {
                auto seq = value.cast<py::sequence>();
                if (py::len(seq) == 2) {
                    self.scroll_position(seq[0].cast<berialdraw::Coord>(), seq[1].cast<berialdraw::Coord>());
                } else {
                    throw std::invalid_argument("Point property must be tuple/list of 2 values");
                }
            } else {
                throw std::invalid_argument("Point property must be tuple/list of 2 values");
            }
        },
        "Scroll position as (x, y) tuple");

    cls.def_property("viewport_size",
        [](berialdraw::ScrollView& self) -> py::tuple {
            const auto& s = self.viewport_size();
            return py::make_tuple(s.width(), s.height());
        },
        [](berialdraw::ScrollView& self, py::object value) {
            if (py::isinstance<py::tuple>(value) || py::isinstance<py::list>(value)) {
                auto seq = value.cast<py::sequence>();
                if (py::len(seq) == 2) {
                    self.viewport_size(seq[0].cast<berialdraw::Dim>(), seq[1].cast<berialdraw::Dim>());
                } else {
                    throw std::invalid_argument("Size property must be tuple/list of 2 values");
                }
            } else if (py::isinstance<py::int_>(value) || py::isinstance<py::float_>(value)) {
                auto d = value.cast<berialdraw::Dim>();
                self.viewport_size(d, d);
            } else {
                throw std::invalid_argument("Size property must be int or tuple/list of 2 values");
            }
        },
        "Viewport size: int (square) or (width, height)");

    cls.def("content_size", &berialdraw::ScrollView::content_size)
       .def("marged_size", &berialdraw::ScrollView::marged_size)
       .def("scroll_focus", &berialdraw::ScrollView::scroll_focus)
       .def("scroll_area", &berialdraw::ScrollView::scroll_area)
       // Event system integration
       BIND_EVENT_PROPERTY(berialdraw::ScrollView, berialdraw::ClickEvent, on_click)
       BIND_EVENT_PROPERTY(berialdraw::ScrollView, berialdraw::ScrollEvent, on_scroll)
       BIND_EVENT_PROPERTY(berialdraw::ScrollView, berialdraw::TouchEvent, on_touch);
}
