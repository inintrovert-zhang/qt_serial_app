#include "mainwindow.h"
#include <QApplication>
#include "singleapplication.h"
int main(int argc, char *argv[])
{
  //  QApplication a(argc, argv);
    SingleApplication a(argc, argv);
    if (a.isRunning()){
        return 0;
    }
    MainWindow w;
    w.show();

    a.setSingleMainWindow(&w);
    return a.exec();
}
