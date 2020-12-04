#ifndef SINGLEAPPLICATION_H
#define SINGLEAPPLICATION_H

#include <QApplication>
#include <QWidget>

class QLocalServer;
class QWidget;

class SingleApplication : public QApplication
{
    Q_OBJECT

public:
    SingleApplication(int &argc, char **argv);
    ~SingleApplication();

public:
    bool isRunning();

    //保存主窗口指针
    void setSingleMainWindow(QWidget* w);

private slots:
    void newLocalConnection();

private:
    bool m_bRunning;
    QLocalServer *m_pServer;
    QWidget *m_widget;
};

#endif // SINGLEAPPLICATION_H
