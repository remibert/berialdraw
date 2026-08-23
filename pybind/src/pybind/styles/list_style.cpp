#include "pybind/pyberialdraw.hpp"

void bind_list_style(pybind11::module_& m) {
    pybind11::class_<berialdraw::ListStyle, berialdraw::Style> cls(m, "ListStyle");
    cls.def(pybind11::init<>());
    
    cls.def_property(berialdraw::StyleNames::LISTSTYLE_SELECTION_MODE, 
        static_cast<berialdraw::ListSelectionMode (berialdraw::ListStyle::*)() const>(&berialdraw::ListStyle::selection_mode),
        static_cast<void (berialdraw::ListStyle::*)(berialdraw::ListSelectionMode)>(&berialdraw::ListStyle::selection_mode), 
        "Selection mode for the list");
}
