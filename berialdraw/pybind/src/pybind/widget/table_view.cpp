#include "pybind/pyberialdraw.hpp"
#include "pybind/event_system.hpp"

void bind_table_view(pybind11::module_& m) {
    pybind11::class_<berialdraw::TableView, berialdraw::Widget>(m, "TableView")
        .def(pybind11::init<berialdraw::Widget*>(), pybind11::return_value_policy::reference_internal, pybind11::keep_alive<1, 2>())
        // Event system integration
        BIND_EVENT_PROPERTY(berialdraw::TableView, berialdraw::ClickEvent, on_click)
        BIND_EVENT_PROPERTY(berialdraw::TableView, berialdraw::KeyEvent, on_key_down)
        BIND_EVENT_PROPERTY(berialdraw::TableView, berialdraw::TouchEvent, on_touch)
        // Methods
        .def("set_widget", &berialdraw::TableView::set_widget)
        .def("get_widget", &berialdraw::TableView::get_widget, pybind11::return_value_policy::reference_internal)
        .def("remove_widget", &berialdraw::TableView::remove_widget)
        .def("clear", &berialdraw::TableView::clear)
        .def("row_count", &berialdraw::TableView::row_count)
        .def("column_count", &berialdraw::TableView::column_count)
        .def("load", [](berialdraw::TableView& self, pybind11::list py_data) {
            // Convert Python list of lists to table cells
            for (uint16_t row = 0; row < (uint16_t)py_data.size(); row++) {
                pybind11::list row_data = pybind11::cast<pybind11::list>(py_data[row]);
                for (uint16_t col = 0; col < (uint16_t)row_data.size(); col++) {
                    std::string cell_value = pybind11::cast<std::string>(row_data[col]);
                    berialdraw::Label* label = new berialdraw::Label(&self);
                    label->text(cell_value.c_str());
                    label->cell(row, col);
                }
            }
        })
        .def("save", [](berialdraw::TableView& self) -> pybind11::list {
            pybind11::list result;
            for (berialdraw::Dim row = 0; row < self.row_count(); row++) {
                pybind11::list row_data;
                for (berialdraw::Dim col = 0; col < self.column_count(); col++) {
                    berialdraw::Widget* widget = self.get_widget(row, col);
                    std::string cell_value;
                    if (widget) {
                        berialdraw::Label* label = dynamic_cast<berialdraw::Label*>(widget);
                        if (label) {
                            cell_value = label->text().c_str();
                        }
                    }
                    row_data.append(cell_value);
                }
                result.append(row_data);
            }
            return result;
        });
}
