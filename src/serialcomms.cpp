/* 
 * Author: Vitaly, Zac Slade
 * 
 */

#include "serialcomms.h"
#include <iostream>
#include <string>
#include <QString>


using namespace std;

namespace serial
{

    PortReaderWriter::PortReaderWriter(QSerialPort *reqPort
                                       , QByteArray *dataForWrite
                                       , QObject *parent)
        : QObject(parent)
        , port(reqPort)
        , writeData(dataForWrite)
    {
        connect(port
                , SIGNAL( readReady() )
                , SLOT( handleReadReady() )
                , Qt::QueuedConnection);

        connect(port
                , SIGNAL( error(QSerialPort::SerialPortError) )
                , SLOT(handleError(QSerialPort::SerialPortError) )
                , Qt::QueuedConnection);

        connect(&m_timer
                , SIGNAL( timeout() )
                , SLOT( handleTimeout() )
                , Qt::QueuedConnection);

        this->m_timer.start(5000); // 5 seconds, value in millis
    }

    PortReaderWriter::~PortReaderWriter(void) {
        if (this->port != 0) { this->port->close(); }
    }

    bool PortReaderWriter::serialConnect(void)
	{ 
        string input_device = "";
        bool gotit= false;
        QStringList str;
        foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
            str.append( "Name        : " + info.portName());
            if (info.manufacturer().contains("FTDI"))
            {
                input_device = info.portName().toStdString();
                gotit = true;
            }
            str.append("Description : " + info.description());
            str.append("Manufacturer: " + info.manufacturer() );
        }

        if(!gotit)
        {
            foreach(const QString elem, str)
            {
                qDebug() << elem << endl;
                cout << "Which serial device Name should we use?" << endl;
                cin >> input_device;
            }
        }

        // Just try it!
        this->port = new QSerialPort(QString(input_device.c_str()));
        // Default for the device under test is 10400 baud
        port->setBaudRate(QSerialPort::Baud38400);
        this->port->open(QIODevice::ReadWrite);

        if (port->isOpen()) return true;
        return false;
	}

    bool PortReaderWriter::sendCommand(const QByteArray &data) {
        if (!this->port->isOpen()) { return false; }
        if (-1 < port->write(data)) { return true; }
        return false;
    }

    QByteArray PortReaderWriter::readLine() {
        if (!this->port->isOpen()) { return ""; }
        return this->port->readLine();
    }

    /***********************************************/
    int PortReaderWriter::decodeRPM(const QByteArray line_data) {
        //QString comm = "01 0C"; // the code for rpm

        QString retval = "1af8";
        int x = 0;
        bool ok;

        x = (retval.toInt(&ok, 16))/4;

        if(ok) return x;
        else return -1;
    }

    int PortReaderWriter::decodeTempEngin(const QByteArray line_data) {

        //QString comm = "01 05 1"; // the code Enginr Tempereture

        QString retval = "1af8";
        int x = 0;
        bool ok;

        x = (retval.toInt(&ok, 16)) - 40;

        if(ok) return x;
        else return -1;
    }

    QString PortReaderWriter::decodeErr(const QByteArray line_data)
    {

        //QString comm = "01 01"; // the code for Error code
        /*
         *  41 01 respons to the request
         *  81 number of corrent troble codes
         * 	81 - 80 or (129)- 120
         * 	most segnificant bit indicate that the  Ceck Engine Light on or of.
         */

        QString retval = line_data;
        QString r_str;
        QString *errors;
        bool ok;
        QString str;
        int x=0;


        /*
         * > 03 is will give you all the actual troble codes
         *  like 01 33 00 00 00 00
         *  most of it is junk we need only  0133
         * 	and 0 = P0 and rest is as is
         * 	 so the code will be P0133
         */

        /*
         * here I just plaing around trying to see how it will work and what
         * to do
         */

        retval.replace(" ","");
        str = retval.replace(" ","");
        r_str = str.mid(4,2);

        x = (r_str.toInt(&ok, 16)) - 128;

        errors = new QString[x];

        for (int i=0; i<x; i++)
        {
            // send 03 to serial;
            // reas
            errorCodes[i];
        }

        /*   Just testing
        for (int i=0; i<x; i++)
        {
            qDebug() << "Error Code "<< i << " : "  << errors[i];
        }
        */

        return r_str;
    }

    QString PortReaderWriter::getConnectedPortName() {
        if (0 == port) return "";
        if (!this->port->isOpen()) return "";
        return this->port->portName();
    }

    bool PortReaderWriter::isConnected(){
        if (0 == port) return false;
        if (this->port->isOpen()) return true;
        return false;
    }

    void PortReaderWriter::handleError(QSerialPort::SerialPortError err) {
	// What to do with this error?

    }

    void PortReaderWriter::handleTimeout() {
	// Hrm...  A Timeout

    }

    void PortReaderWriter::handleReadReady() {
	// Read data
    }
}
