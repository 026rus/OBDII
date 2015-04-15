/* 
 * Author: Vitaly, Zac Slade
 * 
 */

#include "PortReaderWriter.h"
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
    if ( 0 == this->port )
    {
	    this->m_timer.start(timeoutMillis);
	    return;
	}

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

	this->m_timer.start(timeoutMillis);
    }

    PortReaderWriter::~PortReaderWriter(void) {
        if (this->port != 0) { this->port->close(); }
    }

    const QVector<QSerialPortInfo> PortReaderWriter::getAvailPorts(){
        QVector<QSerialPortInfo> ports = QVector<QSerialPortInfo>();
        foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
            ports.append(info);
        }
        return ports;
    }

    bool PortReaderWriter::setPort(string portName) {
	// Just try it!
        this->port = new QSerialPort(QString(portName.c_str()));
        // Default for the device under test is 10400 baud
        this->port->open(QIODevice::ReadWrite);
        port->setBaudRate(QSerialPort::Baud38400);

        if (port->isOpen()){
	    connect(port
		    , SIGNAL( readyRead() )
		    , SLOT( handleReadReady() )
		    , Qt::QueuedConnection);

	    connect(&m_timer
		    , SIGNAL( timeout() )
		    , SLOT( handleTimeout() )
		    , Qt::QueuedConnection);

	    return true;
	}
        return false;
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
        // If did not finde ask user for input
        if(!gotit)
        {
            foreach(const QString elem, str)
            {
                qDebug() << elem << endl;
                cout << "Which serial device Name should we use?" << endl;
                cin >> input_device;
            }
        }

	return this->setPort(input_device);
    }

    bool PortReaderWriter::sendCommand(const QByteArray &data)
    {
        QByteArray lineEnding = data + "\r\n";

        if (0 == this->port) { return false; }

        this->port->open(QIODevice::ReadWrite);

        if (!this->port->isOpen()) { return false; }
        if (-1 < port->write(lineEnding)) { return true; }
        port->flush();
        if (this->port->waitForBytesWritten(timeoutMillis)) { return true; }

        return false;
    }

    QByteArray PortReaderWriter::readAll()
    {
        if (0 == this->port) { return "No port set!"; }
        this->port->open(QIODevice::ReadWrite);
            if (!this->port->isOpen()) { return "Could not open port for write"; }

        /* This should always happen outside the main GUI thread, no problem at cmdline */
        this->port->waitForReadyRead(timeoutMillis);


        QByteArray lineData = QByteArray();
        while (!this->port->atEnd()) {
            QByteArray scratch = QByteArray();
           if (0 < this->port->bytesAvailable()) {
               scratch +=  this->port->readAll();
           }
        }

        /*
         * All hex digits are crammed together with no spaces.
         * Any multiline response will be captured.  Unfortunately,
         * they are all run together.  This could be problematic.
         * Suggest moving to QVector<QByteArray> for return type.
         *
         * Return the now pristine line data.
         */
        QByteArray retval = lineData;
        retval = retval.remove(retval.size()-3, 3);
        retval.replace(" ", "");
        return retval;
    }

    /* Query the current RPM of the engine */
    const QByteArray PortReaderWriter::queryRPM() {
        if (!this->sendCommand("01 0C")) { return QByteArray(); }
        return this->readAll();
    }

    /* Query the fuel intake amount */
    const QByteArray PortReaderWriter::queryFuelLevelIntake() {
        if (!this->sendCommand("01 2F 1")) { return QByteArray(); }
        return this->readAll();
    }

    /* Query the list of error codes */
    const QByteArray PortReaderWriter::queryOBDErrorCodes() {
        if (!this->sendCommand("01 01")) { return QByteArray(); }
        return this->readAll();
    }

    /* Query the temperature of the engine */
    const QByteArray PortReaderWriter::queryTempEngine() {
        if (!this->sendCommand("01 05 1")) { return QByteArray(); }
        return this->readAll();
    }

    /* Query for the current vehicle speed */
    const QByteArray PortReaderWriter::queryVehicleSpeed() {
        if (!this->sendCommand("01 0D")) { return QByteArray(); }
        return this->readAll();
    }

    /* Query the current engine load */
    const QByteArray PortReaderWriter::queryEngineLoad() {
        if (!this->sendCommand("01 04 1")) { return QByteArray(); }
        return this->readAll();
    }


    /* Decode data from queryRPM() */
    const int PortReaderWriter::decodeRPM(const QByteArray line_data) {
        int x = 0;
        bool ok = false;
        x = line_data.toInt(&ok, 16) / 4;
        return ok ? x : -1;
    }

    /* Decode data from queryTempEngine() */
    const int PortReaderWriter::decodeTempEngine(const QByteArray line_data) {
        // Example data: 41057b
        int x = 0;
        bool ok = false;
        QString retval =  line_data.mid(4);
        x = (retval.toInt(&ok, 16)) - 40;
        return ok ? x : -1;
    }

    const QString PortReaderWriter::decodeOBDErrors(const QByteArray line_data)
    {
        /*
         *  41 01 respons to the request
         *  81 number of corrent troble codes
         * 	81 - 80 or (129)- 120
         * 	most segnificant bit indicate that the  Ceck Engine Light on or of.
         */


        QByteArray teststr = "48 6B 10 43 03 25 01 10 11 05 55";
        QString newtest;
        QString retval = line_data;
        QString str_num_of_cods;

        // cheking if data was not found
        if (retval.contains("unable", Qt::CaseInsensitive))
            return retval;

        QString *errors;
        bool ok;
        QString str;
        int num_of_cods=0;

        retval.replace(" ","");
        str = retval.replace(" ","");
        str_num_of_cods = str.mid(4,2);

        num_of_cods = (str_num_of_cods.toInt(&ok, 16)) - 128;

        // cheking for error in retriving numbers of codes
        if (num_of_cods < 1)
        {
            retval.append("\nnot correct number of error codes\nNumber of codes = ");
            retval.append(QString::number(num_of_cods));
            return retval;
        }

        errors = new QString[num_of_cods];

        teststr = teststr.replace(" ","");
        teststr = teststr.remove(0,8);


        for (int i=0; i<num_of_cods; i++)
        {

            str = teststr.left(4);
            if (teststr.size() >= 4 ) teststr = teststr.remove(0,4);

                newtest = str.mid(1);

                if (str.left(1) == "0")
                    newtest.prepend("P0"); else
                if (str.left(1) == "1")
                    newtest.prepend("P1"); else
                if (str.left(1) == "2")
                    newtest.prepend("P2");  else
                if (str.left(1) == "3")
                    newtest.prepend("P3"); else
                ///////////////////////////////////////
                if (str.left(1) == "4")
                    newtest.prepend("C0"); else
                if (str.left(1) == "5")
                    newtest.prepend("C1"); else
                if (str.left(1) == "6")
                    newtest.prepend("C2"); else
                if (str.left(1) == "7")
                    newtest.prepend("C3"); else
                    ///////////////////////////////////////
                if (str.left(1) == "8")
                    newtest.prepend("B0"); else
                if (str.left(1) == "9")
                    newtest.prepend("B1"); else
                if (str.left(1) == "A" || str.left(1) == "a")
                    newtest.prepend("B2"); else
                if (str.left(1) == "B" || str.left(1) == "b")
                    newtest.prepend("B3"); else
                    ////////////////////////////////////////
                if (str.left(1) == "C" || str.left(1) == "c")
                    newtest.prepend("U0");else
                if (str.left(1) == "D" || str.left(1) == "d")
                    newtest.prepend("U1");else
                if (str.left(1) == "E" || str.left(1) == "e")
                    newtest.prepend("U2");else
                if (str.left(1) == "F" || str.left(1) == "f")
                    newtest.prepend("U3");

            errors[i] = newtest;

                //qDebug() << newtest;
        }

        /*   Just testing */
        newtest ="";
        for (int i=0; i<num_of_cods; i++)
        {
            newtest += errors[i] + "\n";
        }
        /*  */

        return newtest;
    }

    /* Decode data from queryEngineLoad() */
    const int PortReaderWriter::decodeEngineLoad(const QByteArray line_data) {
        // Example: 0104f8
        QString retval = line_data.mid(4);
        qDebug() << retval;

        int x = 0;
        bool ok;
        x = ( (retval.toInt(&ok, 16)) * 100)/255 ;
        return ok ? x : -1;
    }

    /* Decode data from queryVehicleSpeed() */
    const int PortReaderWriter::decodeVehicleSpeed(const QByteArray line_data)
    {
        QString retval = line_data;
        retval = retval.replace(" ","");
        retval = retval.mid(4);
        qDebug() << retval;

        int x = 0;
        bool ok;
        x = (retval.toInt(&ok, 16));
        return ok ? x : -1;

    }

    const int PortReaderWriter::decodeFuelLevelIntake(const QByteArray line_data) {
        QString retval = retval.replace(" ","");
        retval = retval.mid(4);
        qDebug() << retval;

        int x = 0;
        bool ok;
        x = ((retval.toInt(&ok, 16)) * 100) / 255 ;
        return ok ? x : -1;
    }

    /* Ensure communications with the serial device are working */
    bool PortReaderWriter::testSerial() {
        if (!this->isConnected()) return false;

        // Send AT I command, nearly every serial device will respond
        // sanely to the AT I command.  If your device does not work
        // with an AT I command, then you might want a better serial device
        sendCommand(QByteArray("AT I"));
        QByteArray buff = this->readAll();
        qDebug() << buff << endl;
        return buff.size() > 0 ? true : false;
    }

    /* If we are connected, give the OS specific port name */
    const QString PortReaderWriter::getConnectedPortName() {
        if (0 == port) return "";
        if (!this->port->isOpen()) return "";
        return this->port->portName();
    }

    /* Check to see if we are connected */
    bool PortReaderWriter::isConnected(){
        if (0 == port) return false;
        if (this->port->isOpen()) return true;
        return false;
    }

    /* Connected to the QSerialPort error signal */
    void PortReaderWriter::handleError(QSerialPort::SerialPortError err) {
        // What to do with this error?
    }

    /* Connected to the QSerialPort timeout signal */
    void PortReaderWriter::handleTimeout() {
        // Hrm...  A Timeout
    }

    /* Connected to the QSerialPort readyRead signal */
    void PortReaderWriter::handleReadReady() {
        // Read data
    }
}
