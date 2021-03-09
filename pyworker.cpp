#include "pyworker.h"

PyWorker::PyWorker(QObject *parent) : QObject(parent)
{

}

PyWorker::~PyWorker()
{
    Py_Finalize();
}

void PyWorker::runPyCode(QString pyCode)
{

    if(initalized == false) {
        qunsetenv("PYTHONHOME");
        qunsetenv("PYTHONPATH");
        qputenv("PYTHONHOME", QString("%1/python/Scripts").arg(QCoreApplication::applicationDirPath()).toUtf8());
        qputenv("PYTHONPATH", QString("%1/python/Lib;%1/python/Lib/site-packages;%1/python/DLLs").arg(QCoreApplication::applicationDirPath()).toUtf8());
        std::string stdOutErr = "import sys\nclass CatchOutErr:\n    def __init__(self):\n        self.value = str()\n    def write(self, txt):\n        self.value += txt\ncatchOutErr = CatchOutErr()\nsys.stdout = catchOutErr\nsys.stderr = catchOutErr\n";
        Py_Initialize();
        pModule = PyImport_AddModule("__main__");
        PyRun_SimpleString(stdOutErr.c_str());
        catcher = PyObject_GetAttrString(pModule,"catchOutErr");
        initalized = true;
    }
    PyRun_SimpleString("catchOutErr.value = str()");
    PyRun_SimpleString(pyCode.toUtf8().data());
    PyErr_Print();
    PyObject *output = PyObject_GetAttrString(catcher,"value");
    auto s = PyBytes_AsString(PyUnicode_AsEncodedString(output,"utf-8","strict"));
    emit finished();
    qDebug() << s;
}
