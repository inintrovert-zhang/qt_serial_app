#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QMainWindow>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include "sqlui.h"
#include "paraset.h"
namespace Ui {
class serialPort;
}

class serialPort : public QMainWindow
{
    Q_OBJECT

public:
    explicit serialPort(QWidget *parent = nullptr);
    ~serialPort();



public:
    bool isPortConnected();

    static  bool isPortObjectMaxNum(){
     return (numObjects >= maxObjects);
    }

private slots:
    void GetSerialport();
    bool OpenSerial(QString SerialName,int Baudrate);
    void readData();
    void timeUpdate();
    void on_OK_clicked();
    void GetSerialData(QByteArray);
    void on_pushButton_clicked();
    void sendSerialDat(QByteArray);
    void on_OK_2_clicked();

public:
    QSerialPort serial;



private:
    Ui::serialPort *ui;
    SQLUI serialSQL;
    paraSet mParaSet;

    QByteArray baRcvData;///< 写指针
    unsigned char Queue1[1024];                   ///< 循环消息队列
    int wReadPoint;                               ///< 读指针
    int wWritePoint;
    QTimer timer;
    //记录对象个数
    static size_t numObjects;
    static const size_t maxObjects;

signals:
    void send(QString);
    void sendUid(QString);
    void send(QByteArray);
};

#endif // SERIALPORT_H
