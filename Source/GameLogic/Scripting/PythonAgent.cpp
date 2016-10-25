#include "PythonAgent.h"
#include <Python.h>
//#include "../../../Support Libraries/Spython/include/Python.h"


PythonAgent::PythonAgent(std::string fileName)
{
	Py_Initialize();
	PyObject * pName = PyUnicode_FromString(fileName.c_str());
	/* Error checking of pName left out */
	PyObject * pModule = PyImport_Import(pName);
}


PythonAgent::~PythonAgent()
{
	Py_Finalize();
}
