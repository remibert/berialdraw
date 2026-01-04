#include "pybind/pyberialdraw.hpp"
void bind_line_style(py::module& m) {
    py::class_<berialdraw::LineStyle, berialdraw::Style> cls(m, "LineStyle");
    cls.def(py::init<>(), "Constructor");
        
    // Propriétés avec précision automatique et support tuple
    bind_point_property(cls, "point1",
        &berialdraw::LineStyle::point1,
        static_cast<void (berialdraw::LineStyle::*)(berialdraw::Coord, berialdraw::Coord)>(&berialdraw::LineStyle::point1),
        "Point1 as (x, y) tuple with automatic precision");

    bind_point_property(cls, "point2",
        &berialdraw::LineStyle::point2,
        static_cast<void (berialdraw::LineStyle::*)(berialdraw::Coord, berialdraw::Coord)>(&berialdraw::LineStyle::point2),
        "Point2 as (x, y) tuple with automatic precision");
}
