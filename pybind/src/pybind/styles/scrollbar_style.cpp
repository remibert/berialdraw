#include "pybind/pyberialdraw.hpp"

void bind_scrollbar_style(py::module& m) {
    py::class_<berialdraw::ScrollbarStyle, berialdraw::Style>(m, "ScrollbarStyle")
        .def(py::init<>(), "Constructor")
        .def_property("scrollbar_visible",
            [](berialdraw::ScrollbarStyle& self) -> bool { 
                return self.scrollbar_visible(); 
            },
            [](berialdraw::ScrollbarStyle& self, bool v) { 
                self.scrollbar_visible(v); 
            }, "Whether scrollbar is visible")
        .def_property("scrollbar_thumb_color",
            [](berialdraw::ScrollbarStyle& self) -> uint32_t { 
                return self.scrollbar_thumb_color(); 
            },
            [](berialdraw::ScrollbarStyle& self, uint32_t c) { 
                self.scrollbar_thumb_color(c); 
            }, "Scrollbar thumb color")
        .def_property("scrollbar_track_color",
            [](berialdraw::ScrollbarStyle& self) -> uint32_t { 
                return self.scrollbar_track_color(); 
            },
            [](berialdraw::ScrollbarStyle& self, uint32_t c) { 
                self.scrollbar_track_color(c); 
            }, "Scrollbar track color")
        .def_property("scrollbar_width",
            [](berialdraw::ScrollbarStyle& self) -> berialdraw::Dim { 
                return self.scrollbar_width(); 
            },
            [](berialdraw::ScrollbarStyle& self, berialdraw::Dim w) { 
                self.scrollbar_width(w); 
            }, "Scrollbar width in pixels")
        .def_property("scrollbar_radius",
            [](berialdraw::ScrollbarStyle& self) -> berialdraw::Dim { 
                return self.scrollbar_radius(); 
            },
            [](berialdraw::ScrollbarStyle& self, berialdraw::Dim r) { 
                self.scrollbar_radius(r); 
            }, "Scrollbar corner radius in pixels")
        .def_property("scrollbar_margin",
            [](berialdraw::ScrollbarStyle& self) -> berialdraw::Dim { 
                return self.scrollbar_margin(); 
            },
            [](berialdraw::ScrollbarStyle& self, berialdraw::Dim m) { 
                self.scrollbar_margin(m); 
            }, "Scrollbar margin in pixels");
}
