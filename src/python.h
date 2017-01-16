#ifndef PYMACS_PYTHON_H
#define PYMACS_PYTHON_H

#include <boost/python.hpp>
#include <Python.h>
#include <iostream>

namespace py = boost::python;

std::string parse_python_exception();

void start_python();

#endif //PYMACS_PYTHON_H
