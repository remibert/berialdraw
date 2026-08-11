#include "pybind/pyberialdraw.hpp"
void bind_desktop(pybind11::module_& m) {
    pybind11::class_<berialdraw::Desktop>(m, "Desktop")
        .def(pybind11::init<>())
        .def("dispatch", &berialdraw::Desktop::dispatch, pybind11::arg("output_svg_filename") = nullptr, PYBIND11_RELEASE_GIL)
        .def("quit", &berialdraw::Desktop::quit, PYBIND11_RELEASE_GIL)
        .def("is_loop", &berialdraw::Desktop::is_loop, PYBIND11_RELEASE_GIL)
        .def("mainloop", &berialdraw::Desktop::mainloop, PYBIND11_RELEASE_GIL)
        .def("add", &berialdraw::Desktop::add, PYBIND11_RELEASE_GIL)
        .def("remove", &berialdraw::Desktop::remove, PYBIND11_RELEASE_GIL)
        .def("back", &berialdraw::Desktop::back, PYBIND11_RELEASE_GIL)
        .def("front", static_cast<void (berialdraw::Desktop::*)(berialdraw::Window*)>(&berialdraw::Desktop::front), PYBIND11_RELEASE_GIL)
        .def("front", static_cast<berialdraw::Window* (berialdraw::Desktop::*)()>(&berialdraw::Desktop::front), pybind11::return_value_policy::reference_internal, PYBIND11_RELEASE_GIL)
        .def("remove_all", &berialdraw::Desktop::remove_all, PYBIND11_RELEASE_GIL);
}
