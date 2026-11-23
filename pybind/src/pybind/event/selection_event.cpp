#include "pybind/pyberialdraw.hpp"

void bind_selection_event(pybind11::module_& m) {
    pybind11::class_<berialdraw::SelectionEvent, berialdraw::Event>(m, "SelectionEvent")
        .def("previous_indices", [](const berialdraw::SelectionEvent& self) {
            pybind11::list result;
            const auto& indices = self.previous_indices();
            for (uint32_t index = 0; index < indices.size(); index++)
            {
                result.append(indices[index]);
            }
            return result;
        })
        .def("indices", [](const berialdraw::SelectionEvent& self) {
            pybind11::list result;
            const auto& indices = self.indices();
            for (uint32_t index = 0; index < indices.size(); index++)
            {
                result.append(indices[index]);
            }
            return result;
        })
        .def_static("type_id", &berialdraw::SelectionEvent::type_id);
}
