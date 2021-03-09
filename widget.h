#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QVector>
#include <QFileDialog>
#include <QThread>
#include <QtGlobal>
#include <QTextEdit>

#include "codemodel.h"
#include "pyworker.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    static void debugInfoToTextEdit(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    static QString debugOut;
    constexpr static auto compareFunc = [] (const QModelIndex & A, const QModelIndex & B) { return B.row() - A.row(); };
    CodeModel cm;
    QStandardItemModel* parasModel = new QStandardItemModel(this);
    QStandardItemModel* loopsModel = new QStandardItemModel(this);
    QStandardItemModel* colsModel = new QStandardItemModel(this);
    QThread th;
    PyWorker worker;

signals:
    void runPyCode(QString code);

public slots:
    void finishedCodeRun();

private slots:

    void on_addParaBtn_clicked();

    void on_deleteParaBtn_clicked();

    void on_applyBtn_clicked();

    void on_savePathBtn_clicked();

    void on_addLoopBtn_clicked();

    void on_deleteLoopBtn_clicked();

    void on_addColBtn_clicked();

    void on_deleteColBtn_clicked();

    void on_exeBtn_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
