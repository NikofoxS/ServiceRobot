//#include <Python.h>
//#include<iostream>
//int great_function_from_python(int a) {
//	int res;
//
//	//����PythonԪ�أ�module��function��tuple��string�ȵȣ�
//	//ʵ���϶���PyObject��C������������ǣ�һ��ʹ��PyObject *
//	PyObject *pModule, *pFunc;
//	PyObject *pArgs, *pValue;
//
//	/* import */
//	pModule = PyImport_Import(PyString_FromString("great_module"));
//
//	/* great_module.great_function */
//	pFunc = PyObject_GetAttrString(pModule, "great_function");
//
//	/* build args */
//	/*Ҳ���Դ���Python���͵ı�����ʹ��PyXXX_New���Դ�������ΪXXX�ı�����*/
//	pArgs = PyTuple_New(1);
//	/*��a��Tuple����a[i] = b��Ӧ�� PyTuple_SetItem(a, i, b)�����������Ż���һ������PyTuple_GetItem���ȡ��ĳһ���ֵ��*/
//	PyTuple_SetItem(pArgs, 0, PyInt_FromLong(a));
//
//	/* call */
//	pValue = PyObject_CallObject(pFunc, pArgs);
//
//	//Python����XXXת��ΪC��������YYYҪʹ��PyXXX_AsYYY����
//	//C����YYYת��ΪPython����XXXҪʹ��PyXXX_FromYYY����
//	res = PyInt_AsLong(pValue);//
//	return res;
//}
//int main()
//{
//	Py_Initialize();
//	std::cout << great_function_from_python(2) << "\n";
//	Py_Finalize();
//	std::cin.get();
//	return 0;
//}