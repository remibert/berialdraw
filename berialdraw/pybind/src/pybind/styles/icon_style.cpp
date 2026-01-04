#include "pybind/pyberialdraw.hpp"
void bind_icon_style(pybind11::module_& m) {
    pybind11::class_<berialdraw::IconStyle, berialdraw::Style> cls(m, "IconStyle");
    cls.def(pybind11::init<>(), "Constructor");
        
    // zoom property avec précision automatique
    cls.def_property("zoom",
        [](berialdraw::IconStyle& self) -> berialdraw::Dim { return self.zoom(); },
        [](berialdraw::IconStyle& self, pybind11::object value) {
            if (pybind11::isinstance<pybind11::int_>(value)) {
                self.zoom(value.cast<berialdraw::Dim>());
            } else if (pybind11::isinstance<pybind11::float_>(value)) {
                self.zoom_(static_cast<berialdraw::Dim>(value.cast<double>() * 64));
            } else {
                throw std::invalid_argument("zoom must be int or float");
            }
        }, "Zoom ratio (int for normal, float for high precision)");

    // icon_padding - custom inline since signature doesn't match bind_margin_property pattern
    cls.def_property("icon_padding",
        [](berialdraw::IconStyle& self) -> pybind11::tuple {
            const auto& m = self.icon_padding();
            return pybind11::make_tuple(m.top(), m.right(), m.bottom(), m.left());
        },
        [](berialdraw::IconStyle& self, pybind11::object value) {
            if (pybind11::isinstance<pybind11::int_>(value) || pybind11::isinstance<pybind11::float_>(value)) {
                auto dim = value.cast<double>();
                if (pybind11::isinstance<pybind11::int_>(value)) {
                    self.icon_padding(static_cast<berialdraw::Dim>(dim), static_cast<berialdraw::Dim>(dim), 
                                    static_cast<berialdraw::Dim>(dim), static_cast<berialdraw::Dim>(dim));
                } else {
                    self.icon_padding_(static_cast<berialdraw::Dim>(dim * 64), static_cast<berialdraw::Dim>(dim * 64), 
                                     static_cast<berialdraw::Dim>(dim * 64), static_cast<berialdraw::Dim>(dim * 64));
                }
            } else if (pybind11::isinstance<pybind11::tuple>(value) || pybind11::isinstance<pybind11::list>(value)) {
                auto seq = value.cast<pybind11::sequence>();
                if (pybind11::len(seq) == 4) {
                    if (pybind11::isinstance<pybind11::int_>(seq[0]) && pybind11::isinstance<pybind11::int_>(seq[1]) && 
                        pybind11::isinstance<pybind11::int_>(seq[2]) && pybind11::isinstance<pybind11::int_>(seq[3])) {
                        self.icon_padding(seq[0].cast<berialdraw::Dim>(), seq[1].cast<berialdraw::Dim>(), 
                                        seq[2].cast<berialdraw::Dim>(), seq[3].cast<berialdraw::Dim>());
                    } else {
                        auto top = seq[0].cast<double>();
                        auto right = seq[1].cast<double>();
                        auto bottom = seq[2].cast<double>();
                        auto left = seq[3].cast<double>();
                        self.icon_padding_(static_cast<berialdraw::Dim>(top * 64), static_cast<berialdraw::Dim>(right * 64), 
                                         static_cast<berialdraw::Dim>(bottom * 64), static_cast<berialdraw::Dim>(left * 64));
                    }
                } else {
                    throw std::invalid_argument("icon_padding tuple/list must have 4 values");
                }
            } else {
                throw std::invalid_argument("icon_padding must be int/float or tuple/list of 4 values");
            }
        }, "Icon padding: int/float (all) or (top,right,bottom,left) with automatic precision");

    cls.def_property("filename",
        [](berialdraw::IconStyle& self) -> std::string { return std::string(self.filename().c_str()); },
        [](berialdraw::IconStyle& self, const std::string& value) { self.filename(value.c_str()); }, "Icon filename");
    cls.def_property("icon_color",
        [](berialdraw::IconStyle& self) -> uint32_t { return self.icon_color(); },
        [](berialdraw::IconStyle& self, uint32_t value) { self.icon_color(value); }, "Icon color");
}
