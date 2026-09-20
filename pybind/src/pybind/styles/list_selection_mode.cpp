#include "pybind/pyberialdraw.hpp"

void bind_list_selection_mode(py::module_& m) {
    py::enum_<berialdraw::ListSelectionMode>(m, "ListSelectionMode")
        .value("NO_SELECTION", berialdraw::ListSelectionMode::LIST_NO_SELECTION, "No selection allowed")
        .value("SINGLE_SELECTION", berialdraw::ListSelectionMode::LIST_SINGLE_SELECTION, "Only one item can be selected")
        .value("MULTI_SELECTION", berialdraw::ListSelectionMode::LIST_MULTI_SELECTION, "Multiple items can be selected");
}
