#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFont>
#include <QTime>
#include <QtXlsx>
extern QSerialPort serial;
QXlsx::Document xlsx;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_sql.InitDB();
    connect(&serialPort,SIGNAL(SerialData(QByteArray)),this,SLOT(GetSerialData(QByteArray)));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QString mystring = ui->textEdit->toPlainText();

    statusBar()->showMessage("Status is here...", 3000);

}

void MainWindow::on_actionSerial_Port_Setting_triggered()
{
    serialsetupDialog.show();
}

void MainWindow::GetSerialData(QByteArray data)
{
    QStringList QRKEY,QDVALUE;
    QString QRTable = "QRCODE";
    QString QRKEY_C = "UID_16";

    if(data.data()[0] == 0x4f && data.data()[1] == 0x4b)
    {
        int len = (data.data()[2] << 8) + data.data()[3];
        if(len == 17 &&  data.length() == 21 && data.data()[4] == 0x1e)
        {
            QByteArray uid = data.mid(5);
            QString dat_ch = uid.toHex();
           // QString str = dat_ch.to;
            QRKEY<<"UID_16";
            QDVALUE<<dat_ch;
            qDebug() <<"dat coming mainwindow!";
            if(m_sql.CheckRepeat(QRTable,QRKEY_C, dat_ch) == false)
            {
                m_sql.insert(QRTable, QRKEY, QDVALUE);
            }
            else
            {
                qDebug() << data <<" already exist in db!!";
            }

            QTextCursor tc = ui->textBrowser->textCursor();
            tc.movePosition(QTextCursor::End);
            tc.insertText("UID:"+ dat_ch+"\n");

            QByteArray rsp = QByteArrayLiteral("\x1b\x11\x00\x02\x1e\x00");
            serial.write(rsp);
        }
    }
    else
    {
         QString dis = QString::fromLocal8Bit(data);
         QTextCursor tc = ui->textBrowser->textCursor();
         tc.movePosition(QTextCursor::End);
         tc.insertText(dis);
        // ui->textBrowser->append(dis);
    }


}

void MainWindow::on_sendBtn_clicked()
{
   if(serial.isOpen())
   {
        QString mystring = ui->textEdit->toPlainText();
        serial.write(mystring.toLocal8Bit(), mystring.toLocal8Bit().length());
   }
   else
   {
       statusBar()->showMessage("please connnect serial port...", 3000);
   }
}

void MainWindow::on_sendBtn_db_clicked()
{
    QString QRTable = "QRCODE";
    QStringList value;
    m_sql.GetValues(QRTable,value);
    for(int i = 0; i< value.size();++i)
    {
        QString tmp = value.at(i);
        ui->textBrowser->append(tmp);
    }
}

void MainWindow::on_xlsxBtn_clicked()
{
     QDateTime current_date_time = QDateTime::currentDateTime();
     QString current_date = current_date_time.toString("yyyyMMddhhmmss");
     QString FileName = "UID_"+current_date+".xlsx";


     xlsx.write(1,1,tr("UID"));
     QString QRTable = "QRCODE";
     QStringList value;
     m_sql.GetValues(QRTable,value);
     for(int i = 0; i< value.size();++i)
     {
         QString tmp = value.at(i);
         xlsx.write(2+i,1,tmp);
     }
    xlsx.saveAs(FileName);

    QFile file(FileName);

    if (file.exists())
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(FileName));
    }
}

void MainWindow::on_textBrowser_textChanged()
{
    ui->textBrowser->moveCursor(QTextCursor::End);
}
