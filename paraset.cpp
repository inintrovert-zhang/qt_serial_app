#include "paraset.h"
#include "ui_paraset.h"

#include <QTime>




paraSet::paraSet(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::paraSet)
{
    ui->setupUi(this);
    ui->bTnSendPara->setEnabled(false);
    QPalette mypalette;
    mypalette.setColor(QPalette::WindowText,Qt::blue);
    statusBar()->setPalette(mypalette);
    this->setWindowTitle(tr("参数设定"));
//    this->windowModality();//setWindowModality(windowModality);
    this->setWindowModality(Qt::ApplicationModal);
}

paraSet::~paraSet()
{
    delete ui;
}


void paraSet::on_bTnSendPara_clicked()
{
    //vol 4  cur 5  charg 4 motor 4 sleep 5  hall 1  acc 4  rssi 1 sw 2
//    if(serial.isOpen())
    {
        QByteArray send_dat = QByteArrayLiteral("\x1b\x11\x00\x22\x20");
         //4
        int vol_min = ui->lEditVolMin->text().toInt();
        send_dat.append((vol_min & 0xff00) >> 8);
        send_dat.append(vol_min & 0x00ff);
        int vol_max = ui->lEditVolMax->text().toInt();
        send_dat.append((vol_max & 0xff00) >> 8);
        send_dat.append(vol_max & 0x00ff);
         //5
        vol_min = ui->lEditWorkCurMin->text().toInt();
        send_dat.append((vol_min & 0xff00) >> 8);
        send_dat.append(vol_min & 0x00ff);
        vol_max = ui->lEditWorkCurMax->text().toInt();
        send_dat.append((vol_max & 0xff00) >> 8);
        send_dat.append(vol_max & 0x00ff);
        send_dat.append(ui->lEditWorkCurDly->text().toInt());

        //4
        vol_min = ui->lEditChagCurMin->text().toInt();
        send_dat.append((vol_min & 0xff00) >> 8);
        send_dat.append(vol_min & 0x00ff);
        vol_max = ui->lEditChagCurMax->text().toInt();
        send_dat.append((vol_max & 0xff00) >> 8);
        send_dat.append(vol_max & 0x00ff);

        //4
        vol_min = ui->lEditMotorCurMin->text().toInt();
        send_dat.append((vol_min & 0xff00) >> 8);
        send_dat.append(vol_min & 0x00ff);
        vol_max = ui->lEditMotorCurMax->text().toInt();
        send_dat.append((vol_max & 0xff00) >> 8);
        send_dat.append(vol_max & 0x00ff);

        //5
        vol_min = ui->lEditSlepCurMin->text().toInt();
        send_dat.append((vol_min & 0xff00) >> 8);
        send_dat.append(vol_min & 0x00ff);
        vol_max = ui->lEditSlepCurMax->text().toInt();
        send_dat.append((vol_max & 0xff00) >> 8);
        send_dat.append(vol_max & 0x00ff);
        send_dat.append(ui->lEditSlepCurDly->text().toInt());

        //1
        send_dat.append(ui->lEditHall->text().toInt());

        //4
        vol_min = ui->lEditAccMin->text().toInt();
        send_dat.append((vol_min & 0xff00) >> 8);
        send_dat.append(vol_min & 0x00ff);
        vol_max = ui->lEditAccMax->text().toInt();
        send_dat.append((vol_max & 0xff00) >> 8);
        send_dat.append(vol_max & 0x00ff);

        //2
        send_dat.append(ui->lEditRssi->text().toInt());
        send_dat.append(ui->lEditRssiTimeOut->text().toInt());

        //2
        vol_max = ui->lEditSW->text().toInt();
        send_dat.append((vol_max & 0xff00) >> 8);
        send_dat.append(vol_max & 0x00ff);

        send_dat.append(ui->lEditTemMin->text().toInt());
        send_dat.append(ui->lEditTemMax->text().toInt());

        emit send(send_dat);
//        statusBar()->showMessage("已发送...", 3000);
    }
//    else
//    {
//        statusBar()->showMessage("请先打开串口..", 3000);
//    }
}



void paraSet::GetSerialData(QByteArray data)
{
    if(data.data()[0] == 0x4f && data.data()[1] == 0x4b)
    {
        int len = (data.data()[2] << 8) + data.data()[3];
        if(len == 2 &&  data.length() == 6 && data.data()[4] == 0x20 && data.data()[5] == 0x00)
        {
            statusBar()->showMessage("参数发送成功..", 3000);
        }
        else
        if(len == 34 &&  data.length() == 38 && data.data()[4] == 0x20)
        {
            //4
           int vol_min = (data.data()[5] << 8) + (data.data()[6]&0x00ff);
            ui->lEditVolMin->setText(QString::number(vol_min));

           vol_min = (data.data()[7] << 8)+ (data.data()[8]&0x00ff);
           ui->lEditVolMax->setText(QString::number(vol_min));

           //5
           vol_min = (data.data()[9] << 8) + (data.data()[10]&0x00ff);
           ui->lEditWorkCurMin->setText(QString::number(vol_min));

           vol_min = (data.data()[11] << 8) + (data.data()[12]&0x00ff);
           ui->lEditWorkCurMax->setText(QString::number(vol_min));

           vol_min = data.data()[13];
           ui->lEditWorkCurDly->setText(QString::number(vol_min));

           //4
           vol_min = (data.data()[14] << 8) + (data.data()[15]&0x00ff);
           ui->lEditChagCurMin->setText(QString::number(vol_min));

           vol_min = (data.data()[16] << 8) + (data.data()[17]&0x00ff);
           ui->lEditChagCurMax->setText(QString::number(vol_min));


           //4
           vol_min = (data.data()[18] << 8) + (data.data()[19]&0x00ff);
           ui->lEditMotorCurMin->setText(QString::number(vol_min));

           vol_min = (data.data()[20] << 8) + (data.data()[21]&0x00ff);
           ui->lEditMotorCurMax->setText(QString::number(vol_min));


           //5

           vol_min = (data.data()[22] << 8) + (data.data()[23]&0x00ff);
           ui->lEditSlepCurMin->setText(QString::number(vol_min));

           vol_min = (data.data()[24] << 8) + (data.data()[25]&0x00ff);
           ui->lEditSlepCurMax->setText(QString::number(vol_min));

           vol_min = data.data()[26];
           ui->lEditSlepCurDly->setText(QString::number(vol_min));


           //1
           vol_min = data.data()[27];
           ui->lEditHall->setText(QString::number(vol_min));

           //4
           vol_min = (data.data()[28] << 8) + (data.data()[29]&0x00ff);
           ui->lEditAccMin->setText(QString::number(vol_min));

           vol_min = (data.data()[30] << 8) + (data.data()[31]&0x00ff);
           ui->lEditAccMax->setText(QString::number(vol_min));

           //2
           vol_min = data.data()[32];
           ui->lEditRssi->setText(QString::number(vol_min));
           vol_min = data.data()[33];
           ui->lEditRssiTimeOut->setText(QString::number(vol_min));


           //2
           vol_min = (data.data()[34] << 8) + (data.data()[35]&0x00ff);
           ui->lEditSW->setText(QString::number(vol_min));

           //2
           vol_min = data.data()[36];
           ui->lEditTemMin->setText(QString::number(vol_min));
           vol_min = data.data()[37];
           ui->lEditTemMax->setText(QString::number(vol_min));

           ui->bTnSendPara->setEnabled(true);

           statusBar()->showMessage("参数接收成功..", 3000);
        }
    }
}

void paraSet::GetSerialInfor(QString info)
{
   statusBar()->showMessage(info, 3000);
}

void paraSet::on_bTnGetPara_clicked()
{
   // if(serial.isOpen())
   // {
        QByteArray rsp = QByteArrayLiteral("\x1b\x11\x00\x01\x20");
        emit send(rsp);
   //     serial.write(rsp);
   //     statusBar()->showMessage("已发送..", 3000);
   // }
   // else
   // {
   //     statusBar()->showMessage("请先打开串口..", 3000);
   // }
}

