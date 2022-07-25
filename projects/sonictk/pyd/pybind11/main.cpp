#include <Python.h>
#include <maya/MGlobal.h>
#include <stdio.h>
#include <pybind11/pybind11.h>

// https://zhuanlan.zhihu.com/p/80884925
void displayInfo(char *inputString)
{
	MGlobal::displayInfo(inputString);
	return;
}

PYBIND11_MODULE( pybind11cpp, m ){
    m.doc() = "pybind11 example";
    m.def("display_info", &displayInfo, "Maya Display Info" ,pybind11::arg("inputString") = "hello world!");
}

