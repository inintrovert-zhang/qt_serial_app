#include "serialsetup.h"
#include "ui_serialsetup.h"

extern QSerialPort serial;

SerialSetup::SerialSetup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SerialSetup)
{
    ui->setupUi(this);
    GetSerialport();
//    connect(&serialport,SIGNAL(SerialData(QByteArray)),this,SLOT(getgerialgata(QByteArray)));
}

SerialSetup::~SerialSetup()
{
    delete ui;
}

void SerialSetup::GetSerialport()
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




void SerialSetup::on_OK_clicked()
{
    if(serial.isOpen())
    {
        serialport.CloseSerial();
        ui->OK->setText(tr("CONNCT"));
        ui->SerialStatus->setText(serial.portName() + " closed..");
    }
    else
    {
        int baud = ui->BaudRate->currentText().toInt();
        bool SerialResault = serialport.OpenSerial(ui->SerialPort->currentText(),baud);
        if(SerialResault)
        {
            ui->OK->setText(tr("DISCONNCT"));
            ui->SerialStatus->setText(serial.portName() + " open successful!!");

        }else
        {
            ui->SerialStatus->setText(tr("串口打开失败，或已经打开！"));
        }
    }
}

void SerialSetup::getgerialgata(QByteArray data)
{
    qDebug() <<"dat coming serial setup";
    qDebug() <<data;
}

void SerialSetup::on_Cancle_clicked()
{
    this->close();
}
