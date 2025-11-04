#include "pybind/pyberialdraw.hpp"
#include "pybind/event_system.hpp"

void bind_scroll_view(pybind11::module_& m) {
    pybind11::class_<berialdraw::ScrollView, berialdraw::Widget>(m, "ScrollView")
        .def(pybind11::init<berialdraw::Widget*>(), pybind11::return_value_policy::reference_internal, pybind11::keep_alive<1, 2>())
        
        // Propriétés avec support tuples
        .def_property("scroll_size",
            [](berialdraw::ScrollView& self) -> py::tuple {
                const auto& size = self.scroll_size();
                return py::make_tuple(size.width(), size.height());
            },
            [](berialdraw::ScrollView& self, py::object value) {
                if (py::isinstance<py::int_>(value) || py::isinstance<py::float_>(value)) {
                    // scroll_size = 100 -> (100, 100)
                    auto dim = value.cast<berialdraw::Dim>();
                    self.scroll_size(dim, dim);
                } else if (py::isinstance<py::tuple>(value) || py::isinstance<py::list>(value)) {
                    auto seq = value.cast<py::sequence>();
                    if (py::len(seq) == 2) {
                        // scroll_size = (100, 50) -> (width=100, height=50)
                        self.scroll_size(seq[0].cast<berialdraw::Dim>(), seq[1].cast<berialdraw::Dim>());
                    } else {
                        throw std::invalid_argument("scroll_size tuple/list must have 2 values (width, height)");
                    }
                } else {
                    throw std::invalid_argument("scroll_size must be int/float or tuple/list of 2 values");
                }
            }, "Scroll size: int (square) or (width, height)")
        .def_property("scroll_position",
            [](berialdraw::ScrollView& self) -> py::tuple {
                const auto& pos = self.scroll_position();
                return py::make_tuple(pos.x(), pos.y());
            },
            [](berialdraw::ScrollView& self, py::object value) {
                if (py::isinstance<py::tuple>(value) || py::isinstance<py::list>(value)) {
                    auto seq = value.cast<py::sequence>();
                    if (py::len(seq) == 2) {
                        self.scroll_position(seq[0].cast<berialdraw::Coord>(), seq[1].cast<berialdraw::Coord>());
                    } else {
                        throw std::invalid_argument("scroll_position must be tuple/list of 2 values (x, y)");
                    }
                } else {
                    throw std::invalid_argument("scroll_position must be tuple/list of 2 values");
                }
            }, "Scroll position as (x, y) tuple")
        .def_property("viewport_size",
            [](berialdraw::ScrollView& self) -> py::tuple {
                const auto& size = self.viewport_size();
                return py::make_tuple(size.width(), size.height());
            },
            [](berialdraw::ScrollView& self, py::object value) {
                if (py::isinstance<py::int_>(value) || py::isinstance<py::float_>(value)) {
                    // viewport_size = 100 -> (100, 100)
                    auto dim = value.cast<berialdraw::Dim>();
                    self.viewport_size(dim, dim);
                } else if (py::isinstance<py::tuple>(value) || py::isinstance<py::list>(value)) {
                    auto seq = value.cast<py::sequence>();
                    if (py::len(seq) == 2) {
                        // viewport_size = (100, 50) -> (width=100, height=50)
                        self.viewport_size(seq[0].cast<berialdraw::Dim>(), seq[1].cast<berialdraw::Dim>());
                    } else {
                        throw std::invalid_argument("viewport_size tuple/list must have 2 values (width, height)");
                    }
                } else {
                    throw std::invalid_argument("viewport_size must be int/float or tuple/list of 2 values");
                }
            }, "Viewport size: int (square) or (width, height)")
        .def("content_size", &berialdraw::ScrollView::content_size)
        .def("marged_size", &berialdraw::ScrollView::marged_size)
        .def("scroll_focus", &berialdraw::ScrollView::scroll_focus)
        .def("scroll_area", &berialdraw::ScrollView::scroll_area)
        
        // Event system integration
        BIND_EVENT_PROPERTY(berialdraw::ScrollView, berialdraw::ClickEvent, on_click)
        BIND_EVENT_PROPERTY(berialdraw::ScrollView, berialdraw::ScrollEvent, on_scroll)
        BIND_EVENT_PROPERTY(berialdraw::ScrollView, berialdraw::TouchEvent, on_touch);
}
