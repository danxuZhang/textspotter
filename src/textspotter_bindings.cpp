#include <pybind11/pybind11.h>

#include "textspotter/textspotter.hpp"

namespace py=pybind11;

PYBIND11_MODULE(_core, m) {
    m.doc() = "TextSpotter";
    py::class_<textspotter::TextSpotter>(m, "TextSpotter")
    .def(py::init<>())
    .def("Hello", &textspotter::TextSpotter::Hello);
}
