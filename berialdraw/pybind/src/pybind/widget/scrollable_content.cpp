#include "pybind/pyberialdraw.hpp"
#include "pybind/event_system.hpp"

// ScrollableContent is a base class with protected destructor that cannot be directly bound.
// Instead, we provide its interface through derived classes (ScrollView and TableView).
// This function is kept as a no-op for backward compatibility with the module initialization.
void bind_scrollable_content(pybind11::module_& m) {
    // No binding needed - ScrollableContent interface is provided through ScrollView and TableView
}
