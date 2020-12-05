#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serialport.h"
#include "sqlui.h"
#include "progresswait.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionAddPort_triggered();
    void showPortPrint(QString);
    void updateCheckNum(QString);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_xlsxBtn_err_clicked();

    void on_sendBtn_db_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    SQLUI mainSQL;
    QString start_time;
    progressWait progress;


signals:
    void importNum(int, int);

};
#endif // MAINWINDOW_H
