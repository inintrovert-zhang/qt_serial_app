#include "serial.h"
#include <QTimer>

QSerialPort serial;
int Connum = 0;
QTimer *timer = new QTimer();
QByteArray baRcvData;
Serial::Serial()
{
    connect(&serial, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(&serial, SIGNAL(aboutToClose()), this, SLOT(closeTips()));
    connect(timer, SIGNAL(timeout()), this, SLOT(timeUpdate()));
}

bool Serial::OpenSerial(QString SerialName,int Baudrate)
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

void Serial::CloseSerial()
{
    serial.clear();
    serial.close();
}

//由于有两个实例化的对象，每次收到数据会执行两次，因为第一次读取了串口的数据，
//第二次进入的时候没有数据了，就不会执行"emit SerialData(string);"
void Serial::readData()
{
/*   if (serial.waitForReadyRead(10)){
        QByteArray data = serial.readAll();
        QString string(data);
        if(string != NULL)
        {
            qDebug() << string;
            emit SerialData(data);
        }
    }
    else
    {
         qDebug() << "no dat";
    }*/

    timer->start(20);//启动定时器，接收100毫秒数据（根据情况设定）
    baRcvData.append(serial.readAll());

}

void Serial::closeTips()
{
    qDebug() << "com close";
}

void Serial::timeUpdate()
{
    timer->stop();
    if(baRcvData.length()!=0)
    {
        qDebug()<<baRcvData;
        emit SerialData(baRcvData);
    }
    baRcvData.clear();
}
