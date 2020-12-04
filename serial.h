#ifndef SERIAL_H
#define SERIAL_H

#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>

class Serial:public QObject
{
    Q_OBJECT

public:
    Serial();
    bool OpenSerial(QString SerialName,int Baudrate);
    void CloseSerial();
    bool isSerialOpen();
private slots:
    void readData();
    void closeTips();
    void timeUpdate();
signals:
    void SerialData(QByteArray);
};

#endif // SERIAL_H
