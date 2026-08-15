#include "pybind/pyberialdraw.hpp"

// ScrollableContent is a C++ base class with protected destructor.
// We need to declare it to pybind11 so that derived classes (List, ScrollView, TableView)
// can reference it, but we don't expose any constructors or methods.
// Python users will never instantiate or use ScrollableContent directly.
void bind_scrollable_content(pybind11::module_& m) {
    // Declare the class without holder type - just let pybind11 know it exists
    // The key is to use nodelete to prevent pybind11 from trying to delete instances
    pybind11::class_<berialdraw::ScrollableContent, 
                     berialdraw::Widget,
                     std::unique_ptr<berialdraw::ScrollableContent, pybind11::nodelete>>(m, "ScrollableContent");
    // No constructors, no methods - this class is purely for inheritance hierarchy
}
