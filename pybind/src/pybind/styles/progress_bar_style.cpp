#include "pybind/pyberialdraw.hpp"
void bind_progress_bar_style(py::module& m) {
    py::class_<berialdraw::ProgressBarStyle, berialdraw::Style> cls(m, "ProgressBarStyle");
    cls.def(py::init<>(), "Constructor");
    
    bind_color_property(cls, berialdraw::StyleNames::PROGRESSBAR_TRACK_COLOR,
        &berialdraw::ProgressBarStyle::track_color,
        static_cast<void (berialdraw::ProgressBarStyle::*)(uint32_t)>(&berialdraw::ProgressBarStyle::track_color),
        "Track color");
    bind_color_property(cls, berialdraw::StyleNames::PROGRESSBAR_FILL_COLOR,
        &berialdraw::ProgressBarStyle::fill_color,
        static_cast<void (berialdraw::ProgressBarStyle::*)(uint32_t)>(&berialdraw::ProgressBarStyle::fill_color),
        "Fill color");
    
    bind_precision_property<berialdraw::ProgressBarStyle>(cls, berialdraw::StyleNames::PROGRESSBAR_FILL_SIZE,
        &berialdraw::ProgressBarStyle::fill_size,
        &berialdraw::ProgressBarStyle::fill_size,
        &berialdraw::ProgressBarStyle::fill_size_q6,
        "Fill size (int for normal, float for high precision)");
    
    bind_precision_property<berialdraw::ProgressBarStyle>(cls, berialdraw::StyleNames::PROGRESSBAR_TRACK_SIZE,
        &berialdraw::ProgressBarStyle::track_size,
        &berialdraw::ProgressBarStyle::track_size,
        &berialdraw::ProgressBarStyle::track_size_q6,
        "Track size (int for normal, float for high precision)");
    
    bind_scalar_property<berialdraw::ProgressBarStyle, int32_t>(cls, berialdraw::StyleNames::RANGE_VALUE,
        &berialdraw::ProgressBarStyle::value,
        &berialdraw::ProgressBarStyle::value,
        "Current value");
    
    bind_scalar_property<berialdraw::ProgressBarStyle, int32_t>(cls, berialdraw::StyleNames::RANGE_MIN_VALUE,
        &berialdraw::ProgressBarStyle::min_value,
        &berialdraw::ProgressBarStyle::min_value,
        "Minimum value");
    
    bind_scalar_property<berialdraw::ProgressBarStyle, int32_t>(cls, berialdraw::StyleNames::RANGE_MAX_VALUE,
        &berialdraw::ProgressBarStyle::max_value,
        &berialdraw::ProgressBarStyle::max_value,
        "Maximum value");
    
    bind_scalar_property<berialdraw::ProgressBarStyle, uint32_t>(cls, berialdraw::StyleNames::RANGE_STEP_VALUE,
        &berialdraw::ProgressBarStyle::step_value,
        &berialdraw::ProgressBarStyle::step_value,
        "Step value");
}

