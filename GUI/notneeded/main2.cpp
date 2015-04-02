#include "CarCoreApp.h"
#include "PortReaderWriter.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <iostream>

using namespace serial;

int main(int argc, char **argv) {
    CarCoreApp *app = new CarCoreApp(argc, &argv);
    qDebug() << "Application has launched.";
    app->run();
    return app->exec();
}
