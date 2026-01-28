#include "pybind/pyberialdraw.hpp"
#include "pybind/event_system.hpp"

void bind_radio_group(pybind11::module_& m) {
    pybind11::class_<berialdraw::RadioGroup, std::shared_ptr<berialdraw::RadioGroup>>(m, "RadioGroup")
        .def(pybind11::init<>())
        .def("add_radio", &berialdraw::RadioGroup::add_radio, pybind11::arg("radio"))
        .def("remove_radio", &berialdraw::RadioGroup::remove_radio, pybind11::arg("radio"))
        .def("select", &berialdraw::RadioGroup::select, pybind11::arg("radio"))
        .def("selected", &berialdraw::RadioGroup::selected, pybind11::return_value_policy::reference);
}

void bind_radio(pybind11::module_& m) {
    pybind11::class_<berialdraw::Radio, berialdraw::Widget, berialdraw::BorderStyle, berialdraw::RadioStyle>(m, "Radio")
        .def(pybind11::init<berialdraw::Widget*>(), pybind11::return_value_policy::reference_internal, pybind11::keep_alive<1, 2>())
        
        // Group management
        .def("set_group", &berialdraw::Radio::set_group, pybind11::arg("group"))
        .def("get_group", &berialdraw::Radio::get_group, pybind11::return_value_policy::reference)
        .def("is_selected", &berialdraw::Radio::is_selected)
        
        // Event system integration
        BIND_EVENT_PROPERTY(berialdraw::Radio, berialdraw::ClickEvent, on_click)
        BIND_EVENT_PROPERTY(berialdraw::Radio, berialdraw::KeyEvent, on_key_down)
        BIND_EVENT_PROPERTY(berialdraw::Radio, berialdraw::FocusEvent, on_focus)
        BIND_EVENT_PROPERTY(berialdraw::Radio, berialdraw::TouchEvent, on_touch);
    
    // Bind RadioGroup after Radio
    bind_radio_group(m);
}
