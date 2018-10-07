#include <Python.h>
#include<iostream>
int great_function_from_python(const char a[5])
{
	int res;

	PyObject *pModule, *pFunc;
	PyObject *pArgs, *pValue;

	pModule = PyImport_Import(PyString_FromString("facerecog"));

	pFunc = PyObject_GetAttrString(pModule, "facefounct");

	pValue = PyObject_CallFunction(pFunc,"s",a);

	//Python����XXXת��ΪC��������YYYҪʹ��PyXXX_AsYYY����
	//C����YYYת��ΪPython����XXXҪʹ��PyXXX_FromYYY����
	res = PyInt_AsLong(pValue);//
	return res;
}
int main()
{
	Py_Initialize();
	std::cout << great_function_from_python("picc.jpg") << "\n";
	Py_Finalize();
	std::cin.get();
	return 0;
}