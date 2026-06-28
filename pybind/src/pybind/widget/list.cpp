#include "pybind/pyberialdraw.hpp"
#include "pybind/event_system.hpp"

void bind_list(pybind11::module_& m) {
    pybind11::class_<berialdraw::List, berialdraw::Widget>(m, "List")
        .def(pybind11::init<berialdraw::Widget*>(), pybind11::return_value_policy::reference_internal, pybind11::keep_alive<1, 2>())
        
        // Event system integration
        BIND_EVENT_PROPERTY(berialdraw::List, berialdraw::ClickEvent, on_click)
        BIND_EVENT_PROPERTY(berialdraw::List, berialdraw::KeyEvent, on_key_down)
        BIND_EVENT_PROPERTY(berialdraw::List, berialdraw::TouchEvent, on_touch)
        
        // ScrollbarStyle properties (inherited via ScrollableContent)
        .def_property("scrollbar_visible",
            [](berialdraw::List& self) -> bool { return self.scrollbar_visible(); },
            [](berialdraw::List& self, bool v) { self.scrollbar_visible(v); },
            "Scrollbar visibility")
        .def_property("scrollbar_width",
            [](berialdraw::List& self) -> berialdraw::Dim { return self.scrollbar_width(); },
            [](berialdraw::List& self, berialdraw::Dim w) { self.scrollbar_width(w); },
            "Scrollbar width")
        
        // BorderStyle properties
        .def_property("border_color",
            [](berialdraw::List& self) -> uint32_t { return self.border_color(); },
            [](berialdraw::List& self, uint32_t c) { self.border_color(c); },
            "Border color")
        .def_property("thickness",
            [](berialdraw::List& self) -> berialdraw::Dim { return self.thickness(); },
            [](berialdraw::List& self, berialdraw::Dim t) { self.thickness(t); },
            "Border thickness")
        .def_property("radius",
            [](berialdraw::List& self) -> berialdraw::Dim { return self.radius(); },
            [](berialdraw::List& self, berialdraw::Dim r) { self.radius(r); },
            "Border radius");
}
