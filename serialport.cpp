#include "serialport.h"
#include "ui_serialport.h"
#include <QTimer>
#include <QTime>
size_t serialPort::numObjects;
const size_t serialPort::maxObjects = 4;


serialPort::serialPort(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::serialPort)
{
    if(numObjects >= maxObjects)
       throw NULL;
    ++numObjects;

    wReadPoint = 0;                               ///< 读指针
    wWritePoint = 0;                              ///< 写指针

    ui->setupUi(this);
    serialSQL.InitDB();
    this->setWindowTitle(tr("终端"));
    this->setAttribute(Qt::WA_DeleteOnClose,true);
    GetSerialport();
    QPalette mypalette;
    mypalette.setColor(QPalette::WindowText,Qt::darkRed);
    ui->labelComStat->setPalette(mypalette);
    ui->labelComStat->setText(tr("未连接.."));

    ui->textBrowser->document()->setMaximumBlockCount(100);

    connect(&serial, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(&timer, SIGNAL(timeout()), this, SLOT(timeUpdate()));
    connect(this,SIGNAL(send(QByteArray)),&mParaSet,SLOT(GetSerialData(QByteArray)));
    connect(this,SIGNAL(send(QString)),&mParaSet,SLOT(GetSerialInfor(QString)));
    connect(&mParaSet,SIGNAL(send(QByteArray)),this,SLOT(sendSerialDat(QByteArray)));
    timer.start(10);
}

serialPort::~serialPort()
{
    --numObjects;
    delete ui;
}


bool serialPort::isPortConnected()
{
    return serial.isOpen();
}

void serialPort::GetSerialport()
{
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        qDebug() << "Name : " << info.portName();
        QSerialPort serial;
        serial.setPort(info);
        if (serial.open(QIODevice::ReadWrite))
            {
                serial.close();
                ui->SerialPort->addItem(info.portName());
            }
    }
}

bool serialPort::OpenSerial(QString SerialName, int Baudrate)
{
    serial.setPortName(SerialName);
    serial.setBaudRate(Baudrate);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setParity(QSerialPort::NoParity);
    serial.setDataBits(QSerialPort::Data8);
    serial.setFlowControl(QSerialPort::NoFlowControl);
    if(serial.open(QIODevice::ReadWrite))
    {
        qDebug() <<SerialName<<" is opened!";
        return true;
    }
    else
    {
        return false;
    }
}

void serialPort::readData()
{
    qDebug()<<"serial dat in............";
    QByteArray recDat = serial.readAll();
    for(int iNum = 0; iNum < recDat.length(); iNum++)
    {
        int tempWReadPoint = wReadPoint;
        if((wWritePoint + 1) % (1024) == tempWReadPoint)
        {
            qDebug()<<"data queue is full!!!";
            continue;
        }
        Queue1[wWritePoint % (1024)] = recDat.at(iNum);
        wWritePoint = (wWritePoint +1) % (1024);
    }
}

void serialPort::timeUpdate()
{
    int itempWritePoint = wWritePoint;
    while((wReadPoint) % (1024) != itempWritePoint)
    {
        baRcvData.append(Queue1[wReadPoint]); //将数据存入分析
        int cmdLen  = 0;
        if(baRcvData.length() >= 4)
        {
            cmdLen = (baRcvData.at(2) << 8) + baRcvData.at(3);
        }
        if(baRcvData.length() >= 4 && baRcvData.at(0) == 0x4f && baRcvData.at(1) == 0x4b && cmdLen < 128) //定义命令长度不超过128
        {
            if(cmdLen + 4 == baRcvData.length())
            {
                GetSerialData(baRcvData);
                baRcvData.clear();
            }
            else
            if(cmdLen + 4 < baRcvData.length())
            {
                baRcvData.clear();
            }
            else
            if((0x4f == Queue1[(wReadPoint  + 1024 - 1) % (1024)]) && (0x4b == Queue1[(wReadPoint + 1024) % (1024)]))  //获取到新的数据
            {
                baRcvData.clear();
                baRcvData.append(0x4f);
                baRcvData.append(0x4b);
                qDebug()<< "new dat.................";
            }
        }
        else
        if(Queue1[wReadPoint] == '\n' || baRcvData.length() >= 256)//定义单个打印字符串长度不超过256
        {
            GetSerialData(baRcvData);
            baRcvData.clear();

            qDebug()<< "read wWritePoint:" << wWritePoint;
            qDebug()<< "read wReadPoint:" << wReadPoint;
        }

        wReadPoint = (wReadPoint + 1) % (1024);
    }
}

void serialPort::on_OK_clicked()
{
    if(serial.isOpen())
    {
        serial.clear();
        serial.close();
        ui->OK->setText(tr("CONNCT"));
        statusBar()->showMessage(serial.portName() + " closed..", 3000);
        ui->labelComStat->setText(tr("连接断开.."));
        emit send(serial.portName() + " closed..");
    }
    else
    {
        int baud = ui->BaudRate->currentText().toInt();
        bool SerialResault = OpenSerial(ui->SerialPort->currentText(),baud);
        if(SerialResault)
        {
            ui->OK->setText(tr("DISCONNCT"));
            statusBar()->showMessage(serial.portName() + " open successful!!", 3000);
            ui->labelComStat->setText(serial.portName() + ", " + QString::number(serial.baudRate()) + " 已连接");
            emit send(serial.portName() + ", " + QString::number(serial.baudRate()) + " 已连接");
        }else
        {
            statusBar()->showMessage(tr("串口打开失败，或已经打开！"), 3000);
        }
    }
}

void serialPort::GetSerialData(QByteArray data)
{
    QStringList QRKEY,QDVALUE,ERRKEY,ERRVALUE;
    QString QRTable = "QRCODE";
    QString QRKEY_C = "UID_16";
    QString QRTableErr = "QERRCODE";
    QString QRKEY_Err = "ErrCode";

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
            if(serialSQL.CheckRepeat(QRTable,QRKEY_C, dat_ch) == false)
            {
                serialSQL.insert(QRTable, QRKEY, QDVALUE);
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
        else
        if(len == 18 &&  data.length() == 22 && data.data()[4] == 0x1f)
        {
            //回复给设备
            QByteArray rsp = QByteArrayLiteral("\x1b\x11\x00\x02\x1f\x00");
            serial.write(rsp);

            QByteArray uid = data.mid(5,16);
            QString dat_ch = uid.toHex();
            QString err_code = QString::number(data.data()[21]);
           // QString str = dat_ch.to;
            ERRKEY<<"time"<<"UID_16"<<"ErrCode";
            QDateTime current_date_time = QDateTime::currentDateTime();
            QString current_date = current_date_time.toString("yyyyMMddhhmmss");
            ERRVALUE<<current_date<<dat_ch<<err_code;
            qDebug() <<"dat coming mainwindow!";

            QString expression = "UID_16 = '" + dat_ch +"'";
            qDebug() << expression;


            if(serialSQL.CheckRepeat(QRTableErr,QRKEY_C, dat_ch) == false)
            {
                serialSQL.insert(QRTableErr, ERRKEY, ERRVALUE);
            }
            else
            {
               serialSQL.Updata(QRTableErr,ERRKEY,ERRVALUE,expression);
                qDebug() << data <<" already exist in db!!";
            }

            QString strMsg = "UID:"+ dat_ch+" ERRCODE:"+ err_code+"\n";

            QTextCursor tc = ui->textBrowser->textCursor();
            tc.movePosition(QTextCursor::End);
            if(data.data()[21] == 0)
            {
                ui->textBrowser->append("<font color=\"#30923D\">" + strMsg + "</font> ");
                emit sendUid("<font color=\"#30923D\">"+ serial.portName() + "-"+strMsg +"</font> ");
            }
            else
            {
                ui->textBrowser->append("<font color=\"#FF0000\"  size='+1'>" + strMsg + "</font> ");
                emit sendUid("<font color=\"#FF0000\"  size='+1'>" + serial.portName() + "-"+strMsg +"</font> ");
            }


           // ui->lineEdit_hisTotal->setText(QString::number(serialSQL.getToatalRecNum(QRTableErr)));
           // QString portName = serial.portName();
           // ui->lineEdit_thisTotal->setText(QString::number(serialSQL.getThisComRecNum(QRTableErr, start_time,portName)));

        }
        else
        if(data.data()[4] == 0x20)
        {
            emit send(data);
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

void serialPort::on_pushButton_clicked()
{
    if(serial.isOpen() == false)
    {
        ui->SerialPort->clear();
        GetSerialport();
    }
}

void serialPort::sendSerialDat(QByteArray dat)
{
    if(serial.isOpen() == true)
    {
       serial.write(dat);
       QString str = "已发送!!";
       emit send(str);
    }
    else
    {
        QString str = "请先打开串口..";
        emit send(str);
    }
}

void serialPort::on_OK_2_clicked()
{
    mParaSet.show();
}

