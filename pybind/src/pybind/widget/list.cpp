#include "pybind/pyberialdraw.hpp"
#include "pybind/event_system.hpp"

void bind_list(pybind11::module_& m) {
    pybind11::class_<berialdraw::List, berialdraw::Widget>(m, "List")
        .def(pybind11::init<berialdraw::Widget*>(), pybind11::return_value_policy::reference_internal, pybind11::keep_alive<1, 2>())
        
        // Event system integration
        BIND_EVENT_PROPERTY(berialdraw::List, berialdraw::ClickEvent, on_click)
        BIND_EVENT_PROPERTY(berialdraw::List, berialdraw::KeyEvent, on_key_down)
        BIND_EVENT_PROPERTY(berialdraw::List, berialdraw::TouchEvent, on_touch)
        
        // List management methods
        .def("add", &berialdraw::List::add,
            pybind11::arg("text"),
            pybind11::arg("icon") = nullptr,
            pybind11::arg("enabled") = true,
            "Add an item to the list")
        .def("insert", &berialdraw::List::insert,
            pybind11::arg("index"),
            pybind11::arg("text"),
            pybind11::arg("icon") = nullptr,
            pybind11::arg("enabled") = true,
            "Insert an item at a specific index")
        .def("remove", &berialdraw::List::remove,
            pybind11::arg("index"),
            "Remove an item at a specific index")
        .def("clear", &berialdraw::List::clear,
            "Clear all items from the list")
        .def("count", &berialdraw::List::count,
            "Get the number of items")
        
        // Selection
        .def_property("selected",
            [](berialdraw::List& self) -> uint16_t { return self.selected(); },
            [](berialdraw::List& self, uint16_t index) { self.select(index); },
            "Selected item index (0xFFFF if none)")
        .def("select", &berialdraw::List::select,
            pybind11::arg("index"),
            "Set the selected item")
        
        // Item properties
        .def("set_enabled", &berialdraw::List::set_enabled,
            pybind11::arg("index"),
            pybind11::arg("enabled"),
            "Enable or disable an item")
        .def("is_enabled", &berialdraw::List::is_enabled,
            pybind11::arg("index"),
            "Check if an item is enabled")
        .def("text", &berialdraw::List::text,
            pybind11::arg("index"),
            "Get the text of an item")
        .def("set_text", &berialdraw::List::set_text,
            pybind11::arg("index"),
            pybind11::arg("text"),
            "Set the text of an item")
        .def("icon", &berialdraw::List::icon,
            pybind11::arg("index"),
            "Get the icon of an item")
        .def("set_icon", &berialdraw::List::set_icon,
            pybind11::arg("index"),
            pybind11::arg("icon"),
            "Set the icon of an item")
        
        // ListStyle properties
        .def_property("separator_color",
            [](berialdraw::List& self) -> uint32_t { return self.separator_color(); },
            [](berialdraw::List& self, uint32_t c) { self.separator_color(c); },
            "Separator color")
        .def_property("separator_thickness",
            [](berialdraw::List& self) -> berialdraw::Dim { return self.separator_thickness(); },
            [](berialdraw::List& self, berialdraw::Dim t) { self.separator_thickness(t); },
            "Separator thickness")
        .def_property("show_arrow",
            [](berialdraw::List& self) -> bool { return self.show_arrow(); },
            [](berialdraw::List& self, bool v) { self.show_arrow(v); },
            "Show arrow indicator")
        .def_property("arrow_right",
            [](berialdraw::List& self) -> bool { return self.arrow_right(); },
            [](berialdraw::List& self, bool v) { self.arrow_right(v); },
            "Arrow direction")
        
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
