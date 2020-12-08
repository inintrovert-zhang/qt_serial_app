#include "progresswait.h"
#include "ui_progresswait.h"

progressWait::progressWait(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::progressWait)
{
    ui->setupUi(this);
    this->setWindowModality(Qt::ApplicationModal);
    this->setWindowTitle(tr("导入"));
    ui->label->setText(tr("导入完成！！！"));
}

progressWait::~progressWait()
{
    delete ui;
}

void progressWait::changeShow(int total, int count)
{
    ui->progressBar->setValue(count * 100 / total);
    ui->label_num->setText("总数："+ QString::number(total) + ",完成：" + QString::number(count));
    if(count == total)
    {
        ui->label->setText(tr("导入完成！！！"));
    }
    else
    if(count == 0)
    {
       ui->label->setText(tr("正在导入，请耐心等待"));
    }
}
