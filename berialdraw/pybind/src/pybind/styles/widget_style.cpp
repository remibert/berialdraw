#include "pybind/pyberialdraw.hpp"
void bind_widget_style(pybind11::module_& m) {
    pybind11::class_<berialdraw::WidgetStyle, berialdraw::Style>(m, "WidgetStyle")
        .def(pybind11::init<>())
        .def_property("row", 
            static_cast<uint16_t (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::row),
            static_cast<void (berialdraw::WidgetStyle::*)(uint16_t)>(&berialdraw::WidgetStyle::row), "Row position")
        .def_property("column", 
            static_cast<uint16_t (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::column),
            static_cast<void (berialdraw::WidgetStyle::*)(uint16_t)>(&berialdraw::WidgetStyle::column), "Column position")
        .def("cell", &berialdraw::WidgetStyle::cell)
        .def_property("size_policy", 
            static_cast<berialdraw::SizePolicy (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::size_policy),
            static_cast<void (berialdraw::WidgetStyle::*)(berialdraw::SizePolicy)>(&berialdraw::WidgetStyle::size_policy), "Size policy")
        .def_property("min_size",
            [](berialdraw::WidgetStyle& self) -> pybind11::tuple {
                const auto& s = self.min_size();
                return pybind11::make_tuple(s.width(), s.height());
            },
            [](berialdraw::WidgetStyle& self, pybind11::object value) {
                if (pybind11::isinstance<pybind11::int_>(value) || pybind11::isinstance<pybind11::float_>(value)) {
                    // min_size = 100 -> (100, 0)
                    auto w = value.cast<berialdraw::Dim>();
                    self.min_size(berialdraw::Size(w, 0));
                } else if (pybind11::isinstance<pybind11::tuple>(value) || pybind11::isinstance<pybind11::list>(value)) {
                    auto seq = value.cast<pybind11::sequence>();
                    if (pybind11::len(seq) == 2) {
                        auto w = seq[0].cast<berialdraw::Dim>();
                        auto h = seq[1].cast<berialdraw::Dim>();
                        self.min_size(berialdraw::Size(w, h));
                    } else {
                        throw std::invalid_argument("min_size tuple/list must have 2 values (width, height)");
                    }
                } else if (pybind11::isinstance<pybind11::object>(value) && pybind11::hasattr(value, "width") && pybind11::hasattr(value, "height")) {
                    // Accepts pyberialdraw.Size
                    auto w = value.attr("width")().cast<berialdraw::Dim>();
                    auto h = value.attr("height")().cast<berialdraw::Dim>();
                    self.min_size(berialdraw::Size(w, h));
                } else {
                    throw std::invalid_argument("min_size must be int, tuple/list of 2 values, or Size object");
                }
            }, "Minimum size: int, (w,h), or Size")
        .def_property("max_size",
            [](berialdraw::WidgetStyle& self) -> pybind11::tuple {
                const auto& s = self.max_size();
                return pybind11::make_tuple(s.width(), s.height());
            },
            [](berialdraw::WidgetStyle& self, pybind11::object value) {
                if (pybind11::isinstance<pybind11::int_>(value) || pybind11::isinstance<pybind11::float_>(value)) {
                    auto w = value.cast<berialdraw::Dim>();
                    self.max_size(berialdraw::Size(w, 0));
                } else if (pybind11::isinstance<pybind11::tuple>(value) || pybind11::isinstance<pybind11::list>(value)) {
                    auto seq = value.cast<pybind11::sequence>();
                    if (pybind11::len(seq) == 2) {
                        auto w = seq[0].cast<berialdraw::Dim>();
                        auto h = seq[1].cast<berialdraw::Dim>();
                        self.max_size(berialdraw::Size(w, h));
                    } else {
                        throw std::invalid_argument("max_size tuple/list must have 2 values (width, height)");
                    }
                } else if (pybind11::isinstance<pybind11::object>(value) && pybind11::hasattr(value, "width") && pybind11::hasattr(value, "height")) {
                    auto w = value.attr("width")().cast<berialdraw::Dim>();
                    auto h = value.attr("height")().cast<berialdraw::Dim>();
                    self.max_size(berialdraw::Size(w, h));
                } else {
                    throw std::invalid_argument("max_size must be int, tuple/list of 2 values, or Size object");
                }
            }, "Maximum size: int, (w,h), or Size")
        .def_property("id", 
            static_cast<uint16_t (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::id),
            static_cast<void (berialdraw::WidgetStyle::*)(uint16_t)>(&berialdraw::WidgetStyle::id), "Widget ID")
        .def_property("extend", 
            static_cast<berialdraw::Extend (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::extend),
            static_cast<void (berialdraw::WidgetStyle::*)(berialdraw::Extend)>(&berialdraw::WidgetStyle::extend), "Extend mode")
        .def_property("pressed", 
            static_cast<bool (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::pressed),
            static_cast<void (berialdraw::WidgetStyle::*)(bool)>(&berialdraw::WidgetStyle::pressed), "Pressed state")
        .def_property("pressable", 
            static_cast<bool (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::pressable),
            static_cast<void (berialdraw::WidgetStyle::*)(bool)>(&berialdraw::WidgetStyle::pressable), "Pressable state")
        .def_property("checked", 
            static_cast<bool (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::checked),
            static_cast<void (berialdraw::WidgetStyle::*)(bool)>(&berialdraw::WidgetStyle::checked), "Checked state")
        .def_property("focusable", 
            static_cast<bool (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::focusable),
            static_cast<void (berialdraw::WidgetStyle::*)(bool)>(&berialdraw::WidgetStyle::focusable), "Focusable state")
        .def_property("selectable", 
            static_cast<bool (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::selectable),
            static_cast<void (berialdraw::WidgetStyle::*)(bool)>(&berialdraw::WidgetStyle::selectable), "Selectable state")
        .def_property("focused", 
            static_cast<bool (berialdraw::WidgetStyle::*)() const>(&berialdraw::WidgetStyle::focused),
            static_cast<void (berialdraw::WidgetStyle::*)(bool)>(&berialdraw::WidgetStyle::focused), "Focused state");
}
