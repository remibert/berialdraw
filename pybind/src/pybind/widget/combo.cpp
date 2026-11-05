#include "pybind/pyberialdraw.hpp"

void bind_combo(pybind11::module_& m) {
    pybind11::class_<berialdraw::Combo, berialdraw::Edit, berialdraw::ComboStyle,
        std::unique_ptr<berialdraw::Combo, pybind11::nodelete>>(m, "Combo")
        .def(pybind11::init<berialdraw::Widget*>(), pybind11::return_value_policy::reference_internal,
            pybind11::keep_alive<1, 2>())
        .def_property("editable",
            static_cast<bool (berialdraw::Combo::*)() const>(&berialdraw::Combo::editable),
            static_cast<void (berialdraw::Combo::*)(bool)>(&berialdraw::Combo::editable))
        .def("open", &berialdraw::Combo::open)
        .def("close", &berialdraw::Combo::close)
        .def_property_readonly("is_open", &berialdraw::Combo::is_open)
        .def("append", [](berialdraw::Combo& self, const std::string& text) {
            return self.append(berialdraw::String(text.c_str()));
        }, pybind11::return_value_policy::reference_internal, pybind11::arg("text"))
        .def("prepend", [](berialdraw::Combo& self, const std::string& text) {
            return self.prepend(berialdraw::String(text.c_str()));
        }, pybind11::return_value_policy::reference_internal, pybind11::arg("text"))
        .def("insert", [](berialdraw::Combo& self, int index, const std::string& text) {
            return self.insert(index, berialdraw::String(text.c_str()));
        }, pybind11::return_value_policy::reference_internal, pybind11::arg("index"), pybind11::arg("text"))
        .def("at", &berialdraw::Combo::at, pybind11::return_value_policy::reference_internal,
            pybind11::arg("index"))
        .def("__getitem__", &berialdraw::Combo::operator[], pybind11::return_value_policy::reference_internal)
        .def("count", &berialdraw::Combo::count)
        .def("is_empty", &berialdraw::Combo::is_empty)
        .def("select", &berialdraw::Combo::select, pybind11::arg("index"))
        .def("select_next", &berialdraw::Combo::select_next, pybind11::return_value_policy::reference_internal)
        .def("select_previous", &berialdraw::Combo::select_previous, pybind11::return_value_policy::reference_internal)
        .def("select_first", &berialdraw::Combo::select_first, pybind11::return_value_policy::reference_internal)
        .def("select_last", &berialdraw::Combo::select_last, pybind11::return_value_policy::reference_internal)
        .def("unselect", &berialdraw::Combo::unselect, pybind11::arg("index"))
        .def("unselect_all", &berialdraw::Combo::unselect_all)
        .def("selected_item", &berialdraw::Combo::selected_item,
            pybind11::return_value_policy::reference_internal)
        .def("selected_items", [](berialdraw::Combo& self) {
            pybind11::list result;
            auto items = self.selected_items();
            for (int32_t index = 0; index < (int32_t)items.size(); index++)
            {
                result.append(items[index]);
            }
            return result;
        })
        .def("items", [](berialdraw::Combo& self) {
            pybind11::list result;
            auto items = self.items();
            for (int32_t index = 0; index < (int32_t)items.size(); index++)
            {
                result.append(items[index]);
            }
            return result;
        })
        .def_property_readonly("selected_index", &berialdraw::Combo::selected_index)
        .def("remove", &berialdraw::Combo::remove, pybind11::arg("index"))
        .def("clear", &berialdraw::Combo::clear)
        .def("serialize", &berialdraw::Combo::serialize, pybind11::arg("it"))
        .def("unserialize", &berialdraw::Combo::unserialize, pybind11::arg("it"));
}