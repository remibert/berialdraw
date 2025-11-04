#include "pybind/pyberialdraw.hpp"
void bind_desktop(pybind11::module_& m) {
    pybind11::class_<berialdraw::Desktop>(m, "Desktop")
        .def(pybind11::init<>())
        .def("dispatch", &berialdraw::Desktop::dispatch, pybind11::arg("output_svg_filename") = nullptr)
        .def("quit", &berialdraw::Desktop::quit)
        .def("is_loop", &berialdraw::Desktop::is_loop)
        .def("mainloop", &berialdraw::Desktop::mainloop)
        .def("add", &berialdraw::Desktop::add)
        .def("remove", &berialdraw::Desktop::remove)
        .def("back", &berialdraw::Desktop::back)
        .def("front", static_cast<void (berialdraw::Desktop::*)(berialdraw::Window*)>(&berialdraw::Desktop::front))
        .def("front", static_cast<berialdraw::Window* (berialdraw::Desktop::*)()>(&berialdraw::Desktop::front), pybind11::return_value_policy::reference_internal)
        .def("remove_all", &berialdraw::Desktop::remove_all);
}
