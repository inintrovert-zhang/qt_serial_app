#ifndef SERIALSETUP_H
#define SERIALSETUP_H

#include <QDialog>
#include <serial.h>
#include <QDebug>

namespace Ui {
class SerialSetup;
}

class SerialSetup : public QDialog
{
    Q_OBJECT

public:
    explicit SerialSetup(QWidget *parent = nullptr);
    ~SerialSetup();

private slots:
    void GetSerialport();

    void on_OK_clicked();
    void getgerialgata(QByteArray);
    void on_Cancle_clicked();

private:
    Ui::SerialSetup *ui;
    Serial serialport;


};

#endif // SERIALSETUP_H
