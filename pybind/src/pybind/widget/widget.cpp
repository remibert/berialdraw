#include "pybind/pyberialdraw.hpp"
void bind_widget(pybind11::module_& m) {
    pybind11::class_<berialdraw::Widget, berialdraw::CommonStyle, berialdraw::WidgetStyle>(m, "Widget")
        // Properties (read-only)
        .def_property_readonly("parent", &berialdraw::Widget::parent, pybind11::return_value_policy::reference_internal)
        .def_property_readonly("root", &berialdraw::Widget::root, pybind11::return_value_policy::reference_internal)
        .def_property_readonly("classname", &berialdraw::Widget::classname)
        
        // Methods that need parameters stay as methods
        .def("scrollable_content", &berialdraw::Widget::scrollable_content, pybind11::return_value_policy::reference_internal, PYBIND11_RELEASE_GIL)
        .def("clear", &berialdraw::Widget::clear, PYBIND11_RELEASE_GIL)
        .def("clean_all", &berialdraw::Widget::clean_all, PYBIND11_RELEASE_GIL)
        .def("search", &berialdraw::Widget::search, pybind11::return_value_policy::reference_internal, PYBIND11_RELEASE_GIL)
        
        // Color methods that return uint32_t
        .def("pressed_color", &berialdraw::Widget::pressed_color, pybind11::arg("color"), pybind11::arg("pressed"), PYBIND11_RELEASE_GIL)
        .def("stated_color", static_cast<uint32_t (berialdraw::Widget::*)(uint32_t)>(&berialdraw::Widget::stated_color), pybind11::arg("color"), PYBIND11_RELEASE_GIL);
}
