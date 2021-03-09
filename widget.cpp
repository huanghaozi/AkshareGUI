#include "widget.h"
#include "ui_widget.h"

QString Widget::debugOut;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{

    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/img/logo.jpg"));
    ui->paraTable->setModel(parasModel);
    ui->loopTable->setModel(loopsModel);
    ui->colTable->setModel(colsModel);

    ui->paraTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->loopTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->colTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    this->parasModel->setColumnCount(2);
    this->parasModel->setHeaderData(0,Qt::Horizontal, "参数名");
    this->parasModel->setHeaderData(1,Qt::Horizontal, "值");

    this->loopsModel->setColumnCount(3);
    this->loopsModel->setHeaderData(0, Qt::Horizontal, "变量名");
    this->loopsModel->setHeaderData(1, Qt::Horizontal, "From");
    this->loopsModel->setHeaderData(2, Qt::Horizontal, "To");

    this->colsModel->setColumnCount(1);
    this->colsModel->setHeaderData(0,Qt::Horizontal, "列名");

    worker.moveToThread(&th);
    th.start();
    connect(this, &Widget::runPyCode, &worker, &PyWorker::runPyCode);
    connect(&worker, &PyWorker::finished, this, &Widget::finishedCodeRun);
    qInstallMessageHandler(debugInfoToTextEdit);
}

Widget::~Widget()
{

    th.quit();
    th.wait();
    delete ui;
}

void Widget::debugInfoToTextEdit(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context);
    Q_UNUSED(type);
    if(type == QtDebugMsg) {
        debugOut.append(QString("%1\n").arg(msg));
    }
}

void Widget::finishedCodeRun()
{
    ui->exeBtn->setEnabled(true);
    qDebug() << "执行完毕!";
    ui->outEdit->append(debugOut);
    debugOut.clear();
}


void Widget::on_addParaBtn_clicked()
{
    QList<QStandardItem*> tempItemList;
    tempItemList << new QStandardItem("双击编辑");
    tempItemList << new QStandardItem("'双击编辑'");
    this->parasModel->insertRow(this->parasModel->rowCount(), tempItemList);
}

void Widget::on_deleteParaBtn_clicked()
{
    auto indexs = ui->paraTable->selectionModel()->selectedIndexes();
    if(!indexs.isEmpty()) {
        std::sort(indexs.begin(), indexs.end(), compareFunc);
        foreach(auto row, indexs) {
            this->parasModel->removeRow(row.row());
        }
    }
}

void Widget::on_applyBtn_clicked()
{
    QStringList paras, cols, loops;
    auto rowCount = this->parasModel->rowCount();
    for(auto i=0; i<rowCount; i++) {
        auto temp0 = this->parasModel->item(i, 0);
        auto temp1 = this->parasModel->item(i, 1);
        paras.append(QString("%1=%2").arg(temp0->text(), temp1->text()));
    }
    rowCount = this->colsModel->rowCount();
    for(auto i=0; i<rowCount; i++) {
        auto temp0 = this->colsModel->item(i, 0);
        cols.append(temp0->text());
    }
    rowCount = this->loopsModel->rowCount();
    for(auto i=0; i<rowCount; i++) {
        auto temp0 = this->loopsModel->item(i, 0);
        auto temp1 = this->loopsModel->item(i, 1);
        auto temp2 = this->loopsModel->item(i, 2);
        loops.append(QString("%1,%2,%3").arg(temp0->text(), temp1->text(), temp2->text()));
    }
    this->cm.setParas(paras);
    this->cm.setColNames(cols);
    this->cm.setLoops(loops);
    this->cm.setSavePath(ui->savePathEdit->text());
    this->cm.setFuncName(ui->funcEdit->text());
    this->cm.genCode();
    ui->codeEdit->setText(this->cm.code);
}

void Widget::on_savePathBtn_clicked()
{
    auto fileName = QFileDialog::getSaveFileName(this, "保存文件", "", "Excel (*.xlsx)");
    if(!fileName.isNull()) {
        ui->savePathEdit->setText(fileName);
    }
}

void Widget::on_addLoopBtn_clicked()
{
    QList<QStandardItem*> tempItemList;
    tempItemList << new QStandardItem("双击编辑");
    tempItemList << new QStandardItem("双击编辑");
    tempItemList << new QStandardItem("双击编辑");
    this->loopsModel->insertRow(this->loopsModel->rowCount(), tempItemList);
}

void Widget::on_deleteLoopBtn_clicked()
{
    auto indexs = ui->loopTable->selectionModel()->selectedIndexes();
    if(!indexs.isEmpty()) {
        std::sort(indexs.begin(), indexs.end(), compareFunc);
        foreach(auto row, indexs) {
            this->loopsModel->removeRow(row.row());
        }
    }
}

void Widget::on_addColBtn_clicked()
{
    QList<QStandardItem*> tempItemList;
    tempItemList << new QStandardItem("'双击编辑'");
    this->colsModel->insertRow(this->colsModel->rowCount(), tempItemList);
}

void Widget::on_deleteColBtn_clicked()
{
    auto indexs = ui->colTable->selectionModel()->selectedIndexes();
    if(!indexs.isEmpty()) {
        std::sort(indexs.begin(), indexs.end(), compareFunc);
        foreach(auto row, indexs) {
            this->colsModel->removeRow(row.row());
        }
    }
}

void Widget::on_exeBtn_clicked()
{
    ui->exeBtn->setEnabled(false);
    ui->outEdit->setText("正在执行...");
    emit runPyCode(ui->codeEdit->toPlainText());
}
