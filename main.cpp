#include "pybind11/pybind11.h"
#include <chrono>
#include <vector>
#include "rectangle.h"
#include "polygon.h"

struct Vars{
    Vars():_pi(pi), _arc(arc), _iarc(iarc){}
    const float _pi;
    const float _arc;
    const float _iarc;
};

namespace py = pybind11;

PYBIND11_MODULE(collision, m){
    py::class_<Vertex2d>(m, "V")
        .def(py::init<float, float>())
        .def("__str__", &Vertex2d::__str__);
    py::class_<Polygon>(m, "Polygon")
        .def(py::init<py::list>())
        .def("__str__", &Polygon::__str__)
        .def("get", &Polygon::get);
    py::class_<Rectangle>(m, "Rect")
        .def(py::init<float, float, float, float, float>())
        .def("__str__", &Rectangle::__str__)
        .def("get", &Rectangle::get);
    py::class_<Vars>(m, "Vars")
        .def(py::init<>())
        .def_readonly("pi", &Vars::_pi)
        .def_readonly("arc", &Vars::_arc)
        .def_readonly("iarc", &Vars::_iarc);
}
