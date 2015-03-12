#include "serialcomms.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <iostream>

using namespace serial;

int main(int argc, char **argv)
{
    QString buff = "";
    QCoreApplication app(argc, argv);


    SerialComms conn = SerialComms();

    /*
    if (!conn.serialConnect())
    {
        qDebug() << "Could not connect!";
        return 1;
    }

    conn.sendCommand(QByteArray("ATI"));
    buff = conn.readCommand();
    cout << buff.toStdString() << endl;
    */

    int x = conn.getRPM();

    cout << "RPM : "<< x <<endl;

    cout << "Trobel code : "<< conn.getErr().toStdString() <<endl;

    return app.exec();
}
