#include "pybind/pyberialdraw.hpp"
#include "pybind/event_system.hpp"

void bind_scroll_view(pybind11::module_& m) {
    pybind11::class_<berialdraw::ScrollView, berialdraw::Widget> cls(m, "ScrollView");

    cls.def(pybind11::init<berialdraw::Widget*>(), pybind11::return_value_policy::reference_internal, pybind11::keep_alive<1, 2>());

    // Propriétés avec support tuples using generic binders
    bind_size_property(cls, "scroll_size",
        &berialdraw::ScrollView::scroll_size,
        static_cast<void (berialdraw::ScrollView::*)(berialdraw::Dim, berialdraw::Dim)>(&berialdraw::ScrollView::scroll_size),
        "Scroll size: int (square) or (width, height)");

    bind_point_property(cls, "scroll_position",
        &berialdraw::ScrollView::scroll_position,
        static_cast<void (berialdraw::ScrollView::*)(berialdraw::Coord, berialdraw::Coord)>(&berialdraw::ScrollView::scroll_position),
        "Scroll position as (x, y) tuple");

    bind_size_property(cls, "viewport_size",
        &berialdraw::ScrollView::viewport_size,
        static_cast<void (berialdraw::ScrollView::*)(berialdraw::Dim, berialdraw::Dim)>(&berialdraw::ScrollView::viewport_size),
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
