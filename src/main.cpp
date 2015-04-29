#include "CarCoreApp.h"
#include "mainwindow.h"
#include "DataOut.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QApplication>

int main(int argc, char** argv) {
    // this is to choose if you want a command line or
    // GUI so far if you pass antthing then it will bring command line
    // and if you do not pass anything the you get GUI
    // !!!!!! please stop deliting it to :)
    if (argc < 2) {
        // GUI interfase by defolte if no argument passed
        QApplication app(argc, argv);
        MainWindow w;
        w.show();
        qDebug() << "Application has launched.";
        return app.exec();
    }

    CarCoreApp app(argc, &argv);
    app.run();
    qDebug() << "Application has launched.";
    return app.exec();
}
