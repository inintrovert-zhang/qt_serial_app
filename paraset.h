#ifndef PARASET_H
#define PARASET_H

#include <QMainWindow>

namespace Ui {
class paraSet;
}

class paraSet : public QMainWindow
{
    Q_OBJECT

public:
    explicit paraSet(QWidget *parent = nullptr);
    ~paraSet();

private slots:
    void on_bTnSendPara_clicked();
    void GetSerialData(QByteArray);
    void GetSerialInfor(QString);
    void on_bTnGetPara_clicked();

private:
    Ui::paraSet *ui;

signals:
    void send(QByteArray);
};

#endif // PARASET_H
