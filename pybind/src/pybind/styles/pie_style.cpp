#include "pybind/pyberialdraw.hpp"
void bind_pie_style(py::module& m) {
    py::class_<berialdraw::PieStyle, berialdraw::Style> cls(m, "PieStyle");
    cls.def(py::init<>(), "Constructor");
    
    // Propriétés avec précision automatique int/float
    bind_precision_property<berialdraw::PieStyle>(cls, "start_angle",
        &berialdraw::PieStyle::start_angle,
        &berialdraw::PieStyle::start_angle,
        &berialdraw::PieStyle::start_angle_,
        "Start angle (int for normal, float for high precision)");
    
    bind_precision_property<berialdraw::PieStyle>(cls, "end_angle",
        &berialdraw::PieStyle::end_angle,
        &berialdraw::PieStyle::end_angle,
        &berialdraw::PieStyle::end_angle_,
        "End angle (int for normal, float for high precision)");
    
    bind_scalar_property<berialdraw::PieStyle, bool>(cls, "rope",
        &berialdraw::PieStyle::rope,
        &berialdraw::PieStyle::rope,
        "Rope mode");
}
