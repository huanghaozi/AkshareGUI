#ifndef CODEMODEL_H
#define CODEMODEL_H

#include <QObject>
#include <QString>
#include <QStringList>

class CodeModel : public QObject
{
    Q_OBJECT
public:
    explicit CodeModel(QObject *parent = nullptr);
    void setLoops(QStringList loopIs);
    void setColNames(QStringList colNames);
    void setParas(QStringList paras);
    void setFuncName(QString funcName);
    void setSavePath(QString savePath);
    void genCode();
    QString code;

private:
    QString indentSolve(QString code, int tabs);
    QStringList loopIs;
    QStringList colNames;
    QStringList paras;
    QString funcName;
    QString savePath;
    QString headCode = QString("import pandas as pd\nimport akshare as ak\n\ndf_temp = list()\n");

signals:

};

#endif // CODEMODEL_H
