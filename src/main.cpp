#include "serialcomms.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <iostream>

using namespace serial;

int main(int argc, char **argv) {
    QString buff = "";
    QCoreApplication app(argc, argv);
    std::cout << "It works!" << std::endl;

    SerialComms conn = SerialComms();
    if (!conn.serialConnect()) {
        qDebug() << "Could not connect!";
        return 1;
    }

    conn.sendCommand(QByteArray("ATI"));
    buff = conn.readCommand();
    std::cout << buff.toStdString() << std::endl;

    return app.exec();
}
