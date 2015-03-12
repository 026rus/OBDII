#include "CarCoreApp.h"

CarCoreApp::CarCoreApp(int &argc, char** argv) : QCoreApplication(argc, argv) {
    connect(this, SIGNAL( aboutToQuit() ), SLOT( cleanupProgramAtExit() ) );
}

CarCoreApp::~CarCoreApp(){
    // Make sure we close our open port before exiting.
    conn.~SerialComms();
}

void CarCoreApp::run() {
    this->conn = serial::SerialComms();

    if (!conn.serialConnect()) {
        qDebug() << "Could not connect!";
        this->exit(1);
    }

    { // Test sending characters, The device should identify itself
        conn.sendCommand(QByteArray("AT I"));
        QByteArray buff = conn.readLine();
        qDebug() << buff << endl;
    }

    if (this->conn.isConnected()) {
        qDebug() << "Connected to serial port "
                 << this->conn.getConnectedPortName();
    } else {
        qDebug() << "Not connected to a serial port!";
        this->exit(2);
    }

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

    qDebug() << "RPM : " << rpmVal;
    qDebug() << "Trobel code : " << trobelCode;

    this->quit();
}

void CarCoreApp::cleanupProgramAtExit(){
    // Nothing for now
}
