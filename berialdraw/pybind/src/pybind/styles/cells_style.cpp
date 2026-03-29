#include "pybind/pyberialdraw.hpp"

void bind_cells_style(pybind11::module_& m) {
    pybind11::class_<berialdraw::CellsStyle> cls(m, "CellsStyle");
    cls.def(pybind11::init<>())
        // Count/size property
        .def("count",
            &berialdraw::CellsStyle::count,
            "Returns the number of cells in the collection")
        // Length support for Python
        .def("__len__",
            &berialdraw::CellsStyle::count)
        // Index access
        .def("__getitem__",
            [](berialdraw::CellsStyle& self, int32_t index) -> berialdraw::CellStyle& {
                return self[index];
            },
            pybind11::return_value_policy::reference)
        // Index assignment
        .def("__setitem__",
            [](berialdraw::CellsStyle& self, int32_t index, const berialdraw::CellStyle& cell) {
                self[index] = cell;
            })
        // Add cell to collection
        .def("append",
            &berialdraw::CellsStyle::push_back,
            "Add a new cell style to the collection")
        // Insert cell at index
        .def("insert",
            &berialdraw::CellsStyle::insert,
            "Insert a cell style at the given index")
        // Remove cell at index
        .def("remove",
            &berialdraw::CellsStyle::remove,
            "Remove a cell style at the given index")
        // Clear all cells
        .def("clear",
            &berialdraw::CellsStyle::clear,
            "Erase all cells from the collection")
        // Check if empty
        .def("empty",
            &berialdraw::CellsStyle::empty,
            "Tests if the collection is empty")
        // Check if index is within bounds
        .def("inbound",
            &berialdraw::CellsStyle::inbound,
            "Indicates if index is within bounds");
}
