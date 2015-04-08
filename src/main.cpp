#include "CarCoreApp.h"
#include "PortReaderWriter.h"
#include "ParseJson.h"
#include <cstdio>

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <iostream>


using namespace serial;

int main(int argc, char** argv) {
    // this is to choose if you want a command line or
    // GUI so far if you pass antthing then it will bring command line
    // and if you do not pass anything the you get GUI
    // !!!!!! please stop deliting it to :)
    if (argc < 2) {
        // GUI interfase by defolte if no argument passed
	return;
        QApplication a(argc, argv);
        MainWindow w;
        w.show();
        return a.exec();
    } else {
        CarCoreApp *app = new CarCoreApp(argc, &argv);
        qDebug() << "Application has launched.";
        app->run();

        #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
          QApplication::setGraphicsSystem("raster");
        #endif
        return app->exec();
    }
    qDebug() << "Application has launched.";
}
