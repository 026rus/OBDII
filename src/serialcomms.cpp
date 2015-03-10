/* 
 * Author: Vitaly, Zac Slade
 * 
 */

#include "serialcomms.h"
#include <iostream>
#include <string>

using namespace std;

namespace serial {
    SerialComms::SerialComms(void) {
        this->port = nullptr;
    }

    SerialComms::~SerialComms(void) {
        if (this->port != nullptr) { this->port->close(); }
    }

    bool SerialComms::serialConnect(void)
	{ 
        string input_device = "";
        foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
               cout << "Name        : " << info.portName().toStdString() << endl;
               cout << "Description : " << info.description().toStdString() << endl;
               cout << "Manufacturer: " << info.manufacturer().toStdString() << endl;
        }
        cout << "Which serial device Name should we use?" << endl;
        cin >> input_device;
        // Just try it!
        this->port = new QSerialPort(QString(input_device.c_str()));
        // Default for the device under test is 10400 baud
        port->setBaudRate(10400);
        this->port->open(QIODevice::ReadWrite);

        if (port->isOpen()) return true;
        return false;
	}

    bool SerialComms::sendCommand(const QByteArray &data) {
        if (!this->port->isOpen()) { return false; }
        if (-1 < port->write(data)) { return true; }
        return false;
	}

    QString SerialComms::readCommand() {
        if (!this->port->isOpen()) { return ""; }
	    return QString("That One!");
	}
};
