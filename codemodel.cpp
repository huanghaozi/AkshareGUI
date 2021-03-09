#include "codemodel.h"

CodeModel::CodeModel(QObject *parent) : QObject(parent)
{

}

void CodeModel::setLoops(QStringList loopIs)
{
    this->loopIs = loopIs;
}

void CodeModel::setColNames(QStringList colNames)
{
    this->colNames = colNames;
}

void CodeModel::setParas(QStringList paras)
{
    this->paras = paras;
}

void CodeModel::setFuncName(QString funcName)
{
    this->funcName = funcName;
}

void CodeModel::setSavePath(QString savePath)
{
    this->savePath = savePath;
}

void CodeModel::genCode()
{
    QString funcContentCode = QString("df_temp.append(ak.%1(%2))\n").arg(this->funcName, this->paras.join(","));
    QString mergeCode = QString("df = pd.concat(df_temp, axis=0).reset_index(drop=True)\n");
    QString saveCode = savePath.isEmpty()?QString():QString("df.to_excel('%1', index=None)\n").arg(savePath);
    QString colSetCode;
    QString wrappedLoopCode;
    if(!this->colNames.empty()) {
        colSetCode = QString("df.cols = [%1]\n").arg(this->colNames.join(","));
    }
    int tabs = 0;
    QString temp;
    if(!this->loopIs.empty()) {
        foreach(auto iName, loopIs) {
            auto iNameProps = iName.split(",");
            temp.append(indentSolve(QString("for %1 in range(%2, %3):\n").arg(iNameProps[0], iNameProps[1], iNameProps[2]), tabs));
            tabs++;
        }
        wrappedLoopCode.append(temp);
    }
    wrappedLoopCode.append(indentSolve(funcContentCode, tabs));
    wrappedLoopCode.append(mergeCode);
    wrappedLoopCode.append(colSetCode);
    wrappedLoopCode.append(saveCode);
    this->code = QString("");
    this->code.append(this->headCode);
    this->code.append(wrappedLoopCode);
}

QString CodeModel::indentSolve(QString code, int tabs)
{
    QString rtn;
    for(int i = 0; i < tabs; i++) {
        rtn.append("    ");
    }
    rtn.append(code);
    return rtn;
}
