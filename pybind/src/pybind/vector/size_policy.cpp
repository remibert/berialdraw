#include "pybind/pyberialdraw.hpp"
void bind_size_policy(py::module& m) {
    py::enum_<berialdraw::SizePolicy>(m, "SizePolicy")
        .value("NORMAL_SIZE", berialdraw::SizePolicy::NORMAL_SIZE, "Occupies a normal space in the layout")
        .value("ENLARGE_WIDTH", berialdraw::SizePolicy::ENLARGE_WIDTH, "Occupies the maximum width space in the layout")
        .value("ENLARGE_HEIGHT", berialdraw::SizePolicy::ENLARGE_HEIGHT, "Occupies the maximum height space in the layout")
        .value("ENLARGE_ALL", berialdraw::SizePolicy::ENLARGE_ALL, "Occupies the maximum space in all directions of the layout")
        .value("SHRINK_WIDTH", berialdraw::SizePolicy::SHRINK_WIDTH, "Occupies the minimum width space in the layout")
        .value("SHRINK_HEIGHT", berialdraw::SizePolicy::SHRINK_HEIGHT, "Occupies the minimum height space in the layout")
        .value("SHRINK_ALL", berialdraw::SizePolicy::SHRINK_ALL, "Occupies the minimum space in all directions of the layout");
}
