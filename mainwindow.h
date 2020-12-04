#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <serialsetup.h>
#include <QFont>
#include <QDebug>
#include <sqlui.h>

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
    void on_pushButton_clicked();

    void on_actionSerial_Port_Setting_triggered();
    void GetSerialData(QByteArray);

    void on_sendBtn_clicked();

    void on_sendBtn_db_clicked();

    void on_xlsxBtn_clicked();

    void on_textBrowser_textChanged();

private:
    Ui::MainWindow *ui;
    Serial serialPort;
    SerialSetup serialsetupDialog;

    SQLUI m_sql;

};
#endif // MAINWINDOW_H
