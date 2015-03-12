#include "CarCoreApp.h"
#include "serialcomms.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <iostream>

using namespace serial;

int main(int argc, char **argv)
{
    CarCoreApp app = CarCoreApp(argc, argv);

    SerialComms conn = SerialComms();

    if (!conn.serialConnect()) {
        qDebug() << "Could not connect!";
        app.exit(1);
    }

    { // Test sending characters, The device should identify itself
        conn.sendCommand(QByteArray("AT I"));
        QByteArray buff = conn.readLine();
        cout << buff.toStdString() << endl;
    }

    qDebug() << "Connected to serial port" << endl;

    int rpmVal = 0;
    { // Try to get the RPM
        conn.sendCommand("01 0C");
        QByteArray buff = conn.readLine();
        rpmVal = conn.decodeRPM(buff);
    }

    QString trobelCode = "";
    { // Try to get the Trouble Code
        conn.sendCommand("STUFF");
        // TODO: Needs to be multiline aware
        QByteArray buff = conn.readLine();
        trobelCode = conn.decodeErr(buff);
    }

    cout << "RPM : " << rpmVal << endl;
    cout << "Trobel code : " << trobelCode.toStdString() << endl;

    app.quit();
    return app.exec();
}
