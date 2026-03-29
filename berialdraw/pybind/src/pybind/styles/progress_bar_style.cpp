#include "pybind/pyberialdraw.hpp"
void bind_progress_bar_style(py::module& m) {
    py::class_<berialdraw::ProgressBarStyle, berialdraw::Style> cls(m, "ProgressBarStyle");
    cls.def(py::init<>(), "Constructor");
    
    bind_color_property(cls, "track_color",
        &berialdraw::ProgressBarStyle::track_color,
        static_cast<void (berialdraw::ProgressBarStyle::*)(uint32_t)>(&berialdraw::ProgressBarStyle::track_color),
        "Track color");
    bind_color_property(cls, "fill_color",
        &berialdraw::ProgressBarStyle::fill_color,
        static_cast<void (berialdraw::ProgressBarStyle::*)(uint32_t)>(&berialdraw::ProgressBarStyle::fill_color),
        "Fill color");
    
    bind_precision_property<berialdraw::ProgressBarStyle>(cls, "fill_size",
        &berialdraw::ProgressBarStyle::fill_size,
        &berialdraw::ProgressBarStyle::fill_size,
        &berialdraw::ProgressBarStyle::fill_size_,
        "Fill size (int for normal, float for high precision)");
    
    bind_precision_property<berialdraw::ProgressBarStyle>(cls, "track_size",
        &berialdraw::ProgressBarStyle::track_size,
        &berialdraw::ProgressBarStyle::track_size,
        &berialdraw::ProgressBarStyle::track_size_,
        "Track size (int for normal, float for high precision)");
    
    bind_scalar_property<berialdraw::ProgressBarStyle, int32_t>(cls, "value",
        &berialdraw::ProgressBarStyle::value,
        &berialdraw::ProgressBarStyle::value,
        "Current value");
    
    bind_scalar_property<berialdraw::ProgressBarStyle, int32_t>(cls, "min_value",
        &berialdraw::ProgressBarStyle::min_value,
        &berialdraw::ProgressBarStyle::min_value,
        "Minimum value");
    
    bind_scalar_property<berialdraw::ProgressBarStyle, int32_t>(cls, "max_value",
        &berialdraw::ProgressBarStyle::max_value,
        &berialdraw::ProgressBarStyle::max_value,
        "Maximum value");
    
    bind_scalar_property<berialdraw::ProgressBarStyle, uint32_t>(cls, "step_value",
        &berialdraw::ProgressBarStyle::step_value,
        &berialdraw::ProgressBarStyle::step_value,
        "Step value");
}

