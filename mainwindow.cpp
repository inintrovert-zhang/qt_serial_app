#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTime>
#include <QtXlsx>
#include <QMessageBox>
QXlsx::Document xlsx;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("测试控制终端"));
    QPalette mypalette;
    mypalette.setColor(QPalette::Text,Qt::blue);
    ui->lineEdit_hisTotal->setPalette(mypalette);
    ui->lineEdit_thisTotal->setPalette(mypalette);

    QDateTime current_date_time = QDateTime::currentDateTime();
    start_time = current_date_time.toString("yyyyMMddhhmmss");
    QString QRTableErr = "QERRCODE";
    ui->lineEdit_hisTotal->setText(QString::number(mainSQL.getToatalRecNum(QRTableErr)));
    ui->lineEdit_thisTotal->setText(QString::number(mainSQL.getThisRecNum(QRTableErr, start_time)));

    ui->textBrowser->document()->setMaximumBlockCount(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionAddPort_triggered()
{
    if(serialPort::isPortObjectMaxNum() == false)
    {
        serialPort *activePort1 = new serialPort(this);
        connect(activePort1, SIGNAL(send(QString)),this,SLOT(showPortPrint(QString)));
        connect(activePort1, SIGNAL(sendUid(QString)),this,SLOT(updateCheckNum(QString)));
        activePort1->show();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("打开端口数目已达上限....");
        msgBox.exec();
    }
}

void MainWindow::showPortPrint(QString dat)
{
    QTextCursor tc = ui->textBrowser->textCursor();
    tc.movePosition(QTextCursor::End);
    ui->textBrowser->append(dat);


//    ui->receive_data_textEdit->insertPlainText(com_str);
//    ui->receive_data_textEdit->moveCursor(QTextCursor::End);
}

void MainWindow::updateCheckNum(QString dat)
{
    showPortPrint(dat);

    QString QRTableErr = "QERRCODE";

    ui->lineEdit_hisTotal->setText(QString::number(mainSQL.getToatalRecNum(QRTableErr)));
    ui->lineEdit_thisTotal->setText(QString::number(mainSQL.getThisRecNum(QRTableErr, start_time)));
}

void MainWindow::on_pushButton_clicked()
{
    QString QRTableErr = "QERRCODE";
    QDateTime current_date_time = QDateTime::currentDateTime();
    start_time = current_date_time.toString("yyyyMMddhhmmss");
    ui->lineEdit_thisTotal->setText(QString::number(mainSQL.getThisRecNum(QRTableErr, start_time)));
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->textBrowser->clear();
}

void MainWindow::on_xlsxBtn_err_clicked()
{
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyyMMddhhmmss");
    QString FileName = "UID_ERRCODE"+current_date+".xlsx";

    xlsx.write(1,1,tr("DATE"));
    xlsx.write(1,2,tr("UID"));
    xlsx.write(1,3,tr("ERRCODE"));

    QString QRTableErr = "QERRCODE";
    QStringList value,value2,value3;
    mainSQL.GetValues2(QRTableErr,value,value2,value3);
    for(int i = 0; i< value.size();++i)
    {
        QString tmp = value.at(i);
        xlsx.write(2+i,1,tmp);
    }
    for(int i = 0; i< value2.size();++i)
    {
        QString tmp = value2.at(i);
        xlsx.write(2+i,2,tmp);
    }
    for(int i = 0; i< value3.size();++i)
    {
        QString tmp = value3.at(i);
        xlsx.write(2+i,3,tmp);
    }
   xlsx.saveAs(FileName);

   QFile file(FileName);

   if (file.exists())
   {
       QDesktopServices::openUrl(QUrl::fromLocalFile(FileName));
   }
}

void MainWindow::on_sendBtn_db_clicked()
{
    QString QRTableErr = "QERRCODE";
    QStringList value,value2,value3;
    mainSQL.GetValues2(QRTableErr,value,value2,value3);
    for(int i = 0; i< value.size();++i)
    {
        QString tmp = "date:" + value.at(i) +" uid:" +value2.at(i)+" errcode:" +value3.at(i);
        ui->textBrowser->append(tmp);
    }
}
