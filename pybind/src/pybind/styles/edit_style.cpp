#include "pybind/pyberialdraw.hpp"
void bind_edit_style(py::module& m) {
    py::class_<berialdraw::EditStyle, berialdraw::Style> cls(m, "EditStyle");
    cls.def(py::init<>(), "Constructor")
        .def_property(berialdraw::StyleNames::EDIT_PLACEHOLDER,
            [](berialdraw::EditStyle& self) -> std::string { return std::string(self.place_holder().c_str()); },
            [](berialdraw::EditStyle& self, const std::string& value) { self.place_holder(value.c_str()); }, "Placeholder text")
        .def_property(berialdraw::StyleNames::EDIT_MAX_LINES,
            [](berialdraw::EditStyle& self) -> uint16_t { return self.max_lines(); },
            [](berialdraw::EditStyle& self, uint16_t value) { self.max_lines(value); }, "Maximum visible lines")
        .def_property(berialdraw::StyleNames::EDIT_MAX_COLUMNS,
            [](berialdraw::EditStyle& self) -> uint16_t { return self.max_columns(); },
            [](berialdraw::EditStyle& self, uint16_t value) { self.max_columns(value); }, "Maximum visible columns");
    
    bind_color_property(cls, berialdraw::StyleNames::EDIT_SELECT_COLOR,
        &berialdraw::EditStyle::select_color,
        static_cast<void (berialdraw::EditStyle::*)(uint32_t)>(&berialdraw::EditStyle::select_color),
        "Selection color");
    bind_color_property(cls, berialdraw::StyleNames::EDIT_CURSOR_COLOR,
        &berialdraw::EditStyle::cursor_color,
        static_cast<void (berialdraw::EditStyle::*)(uint32_t)>(&berialdraw::EditStyle::cursor_color),
        "Cursor color");
    bind_color_property(cls, berialdraw::StyleNames::EDIT_PLACEHOLDER_COLOR,
        &berialdraw::EditStyle::place_holder_color,
        static_cast<void (berialdraw::EditStyle::*)(uint32_t)>(&berialdraw::EditStyle::place_holder_color),
        "Placeholder color");
    
    cls.def_property(berialdraw::StyleNames::EDIT_PASSWORD,
        [](berialdraw::EditStyle& self) -> bool { return self.password(); },
        [](berialdraw::EditStyle& self, bool value) { self.password(value); }, "Password mode");
}
