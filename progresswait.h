#ifndef PROGRESSWAIT_H
#define PROGRESSWAIT_H

#include <QDialog>

namespace Ui {
class progressWait;
}

class progressWait : public QDialog
{
    Q_OBJECT

public:
    explicit progressWait(QWidget *parent = nullptr);
    ~progressWait();

private:
    Ui::progressWait *ui;

private slots:
    void changeShow(int, int);
};

#endif // PROGRESSWAIT_H
