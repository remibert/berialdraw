#include "pybind/pyberialdraw.hpp"
void bind_pie_style(py::module& m) {
    py::class_<berialdraw::PieStyle, berialdraw::Style> cls(m, "PieStyle");
    cls.def(py::init<>(), "Constructor");
    
    // Propriétés avec précision automatique int/float
    bind_precision_property<berialdraw::PieStyle>(cls, berialdraw::StyleNames::PIE_START_ANGLE,
        &berialdraw::PieStyle::start_angle,
        &berialdraw::PieStyle::start_angle,
        &berialdraw::PieStyle::start_angle_q6,
        "Start angle (int for normal, float for high precision)");
    
    bind_precision_property<berialdraw::PieStyle>(cls, berialdraw::StyleNames::PIE_SWEEP_ANGLE,
        &berialdraw::PieStyle::end_angle,
        &berialdraw::PieStyle::end_angle,
        &berialdraw::PieStyle::end_angle_q6,
        "End angle (int for normal, float for high precision)");
    
    bind_scalar_property<berialdraw::PieStyle, bool>(cls, berialdraw::StyleNames::PIE_ROPE,
        &berialdraw::PieStyle::rope,
        &berialdraw::PieStyle::rope,
        "Rope mode");
}
