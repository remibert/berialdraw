#include "pybind/pyberialdraw.hpp"
void bind_widget(pybind11::module_& m) {
    pybind11::class_<berialdraw::Widget, berialdraw::CommonStyle, berialdraw::WidgetStyle>(m, "Widget")
        // Properties (read-only)
        .def_property_readonly("parent", &berialdraw::Widget::parent, pybind11::return_value_policy::reference_internal)
        .def_property_readonly("root", &berialdraw::Widget::root, pybind11::return_value_policy::reference_internal)
        .def_property_readonly("classname", &berialdraw::Widget::classname)
        
        // Methods that need parameters stay as methods
        .def("scroll_view", &berialdraw::Widget::scroll_view, pybind11::return_value_policy::reference_internal)
        .def("clear", &berialdraw::Widget::clear)
        .def("clean_all", &berialdraw::Widget::clean_all)
        .def("search", &berialdraw::Widget::search, pybind11::return_value_policy::reference_internal)
        
        // Color methods need parameters, so they stay as methods
        .def("pressed_color", &berialdraw::Widget::pressed_color, pybind11::arg("color"), pybind11::arg("pressed"))
        .def("stated_color", &berialdraw::Widget::stated_color, pybind11::arg("color"));
}
