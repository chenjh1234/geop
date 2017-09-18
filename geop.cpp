#include <iostream>
#include <QString>
#include <QApplication>
#include <QDebug>
 
#include "mainwindow.h"
 

using namespace std;


int main(int argc, char *argv[])
{
  //  Q_INIT_RESOURCE(application);

    QApplication app(argc, argv);
    app.setOrganizationName(ORG_NAME);
    app.setApplicationName(APP_NAME);
    MainWindow mainWin;
    mainWin.show();
    return app.exec();
}
