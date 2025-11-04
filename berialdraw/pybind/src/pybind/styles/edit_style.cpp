#include "pybind/pyberialdraw.hpp"
void bind_edit_style(py::module& m) {
    py::class_<berialdraw::EditStyle, berialdraw::Style>(m, "EditStyle")
        .def(py::init<>(), "Constructor")
        .def_property("place_holder",
            [](berialdraw::EditStyle& self) -> std::string { return std::string(self.place_holder().c_str()); },
            [](berialdraw::EditStyle& self, const std::string& value) { self.place_holder(value.c_str()); }, "Placeholder text")
        .def_property("max_lines",
            [](berialdraw::EditStyle& self) -> uint16_t { return self.max_lines(); },
            [](berialdraw::EditStyle& self, uint16_t value) { self.max_lines(value); }, "Maximum visible lines")
        .def_property("max_columns",
            [](berialdraw::EditStyle& self) -> uint16_t { return self.max_columns(); },
            [](berialdraw::EditStyle& self, uint16_t value) { self.max_columns(value); }, "Maximum visible columns")
        .def_property("select_color",
            [](berialdraw::EditStyle& self) -> uint32_t { return self.select_color(); },
            [](berialdraw::EditStyle& self, uint32_t value) { self.select_color(value); }, "Selection color")
        .def_property("cursor_color",
            [](berialdraw::EditStyle& self) -> uint32_t { return self.cursor_color(); },
            [](berialdraw::EditStyle& self, uint32_t value) { self.cursor_color(value); }, "Cursor color")
        .def_property("place_holder_color",
            [](berialdraw::EditStyle& self) -> uint32_t { return self.place_holder_color(); },
            [](berialdraw::EditStyle& self, uint32_t value) { self.place_holder_color(value); }, "Placeholder color")
        .def_property("password",
            [](berialdraw::EditStyle& self) -> bool { return self.password(); },
            [](berialdraw::EditStyle& self, bool value) { self.password(value); }, "Password mode");
}
