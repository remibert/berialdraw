#include "pybind/pyberialdraw.hpp"
void bind_text_style(pybind11::module_& m) {
    pybind11::class_<berialdraw::TextStyle, berialdraw::Style>(m, "TextStyle")
        .def(pybind11::init<>())
        
        // font_familly property 
        .def_property("font_familly",
            [](berialdraw::TextStyle& self) -> std::string { return std::string(self.font_familly().c_str()); },
            [](berialdraw::TextStyle& self, const std::string& value) { self.font_familly(value.c_str()); }, "Font family name")
        
        // font_size property avec plusieurs formats
        .def_property("font_size",
            [](berialdraw::TextStyle& self) -> pybind11::tuple {
                const auto& size = self.font_size();
                return pybind11::make_tuple(size.width(), size.height());
            },
            [](berialdraw::TextStyle& self, pybind11::object value) {
                if (pybind11::isinstance<pybind11::int_>(value) || pybind11::isinstance<pybind11::float_>(value)) {
                    // font_size = 12 -> (12, 12)
                    auto dim = value.cast<berialdraw::Dim>();
                    self.font_size(dim, dim);
                } else if (pybind11::isinstance<pybind11::tuple>(value) || pybind11::isinstance<pybind11::list>(value)) {
                    auto seq = value.cast<pybind11::sequence>();
                    if (pybind11::len(seq) == 2) {
                        // font_size = (12, 16) -> (width=12, height=16)
                        self.font_size(seq[0].cast<berialdraw::Dim>(), seq[1].cast<berialdraw::Dim>());
                    } else {
                        throw std::invalid_argument("font_size tuple/list must have 2 values (width, height)");
                    }
                } else {
                    throw std::invalid_argument("font_size must be int/float or tuple/list of 2 values");
                }
            }, "Font size: int (same w/h) or (width, height)")
            
        // padding property avec plusieurs formats CSS
        .def_property("padding",
            [](berialdraw::TextStyle& self) -> pybind11::tuple {
                const auto& margin = self.padding();
                return pybind11::make_tuple(margin.top(), margin.right(), margin.bottom(), margin.left());
            },
            [](berialdraw::TextStyle& self, pybind11::object value) {
                if (pybind11::isinstance<pybind11::int_>(value) || pybind11::isinstance<pybind11::float_>(value)) {
                    // padding = 5 -> (5, 5, 5, 5)
                    auto dim = value.cast<berialdraw::Dim>();
                    self.padding(dim);
                } else if (pybind11::isinstance<pybind11::tuple>(value) || pybind11::isinstance<pybind11::list>(value)) {
                    auto seq = value.cast<pybind11::sequence>();
                    auto len = pybind11::len(seq);
                    if (len == 2) {
                        // padding = (5, 10) -> (5, 10, 5, 10) - vertical, horizontal
                        auto vertical = seq[0].cast<berialdraw::Dim>();
                        auto horizontal = seq[1].cast<berialdraw::Dim>();
                        self.padding(vertical, horizontal);
                    } else if (len == 4) {
                        // padding = (1, 2, 3, 4) -> (top, right, bottom, left)
                        self.padding(seq[0].cast<berialdraw::Dim>(), seq[1].cast<berialdraw::Dim>(), 
                                   seq[2].cast<berialdraw::Dim>(), seq[3].cast<berialdraw::Dim>());
                    } else {
                        throw std::invalid_argument("padding tuple/list must have 2 or 4 values");
                    }
                } else {
                    throw std::invalid_argument("padding must be int/float or tuple/list of 2 or 4 values");
                }
            }, "Padding: int (all), (v,h), or (top,right,bottom,left)")

        .def_property("text",
            [](berialdraw::TextStyle& self) -> std::string { return std::string(self.text().c_str()); },
            [](berialdraw::TextStyle& self, const std::string& value) { self.text(value.c_str()); }, "Text content")
        .def_property("text_color",
            [](berialdraw::TextStyle& self) -> uint32_t { return self.text_color(); },
            [](berialdraw::TextStyle& self, uint32_t value) { self.text_color(value); }, "Text color")
        .def_property("text_align",
            [](berialdraw::TextStyle& self) -> berialdraw::Align { return self.text_align(); },
            [](berialdraw::TextStyle& self, berialdraw::Align value) { self.text_align(value); }, "Text alignment");
}
