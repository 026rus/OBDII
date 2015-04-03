#include "CarCoreApp.h"
#include "PortReaderWriter.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <iostream>

#include <QtWidgets/QApplication>
#include "mainwindow.h"

using namespace serial;

int main(int argc, char **argv)
{

  QApplication a(argc, argv);
  MainWindow w;
//    CarCoreApp *app = new CarCoreApp(argc, &argv);
//    qDebug() << "Application has launched.";
//    app->run();
serial::PortReaderWriter *conn;
    conn = new serial::PortReaderWriter();

//#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
//  QApplication::setGraphicsSystem("raster");
//#endif
  w.show();
  return a.exec();
//    return app->exec();
}
