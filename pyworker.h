#ifndef PYWORKER_H
#define PYWORKER_H

#include <QObject>
#include <QCoreApplication>
#pragma push_macro("slots")
#undef slots
#include <Python.h>
#pragma pop_macro("slots")

class PyWorker : public QObject
{
    Q_OBJECT
public:
    explicit PyWorker(QObject *parent = nullptr);
    ~PyWorker();
    bool initalized = false;
    PyObject *pModule;
    PyObject *catcher;

public slots:
    void runPyCode(QString pyCode);

signals:
    void finished();
};

#endif
