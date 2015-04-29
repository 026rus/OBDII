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
        this->timeoutMillis = 1000;
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
        if (0 != this->port) {
            this->port->flush();
            delete this->port;
        }

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
        }

        // Test the serial device to ensure connection
        return this->testSerial();
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

	// Write the bytes and wait for completion
        if (-1 < port->write(lineEnding)) {
	    if (this->port->waitForBytesWritten(timeoutMillis)) {
		// Be sure to flush the port after completing the write
		port->flush();
		return true;
	    }
	}

	// Port failed to write or write completely
	// Flush the buffer and signal failure
	port->flush();
        return false;
    }

    QString PortReaderWriter::readAll(int echo_chars)
    {
        if (0 == this->port) { return "No port set!"; }

        this->port->open(QIODevice::ReadWrite);
            if (!this->port->isOpen()) { return "Could not open port for write"; }

        /* This should always happen outside the main GUI thread, no problem at cmdline */
        QByteArray lineData = QByteArray();
        while (this->port->waitForReadyRead(timeoutMillis)) {
            if (0 < this->port->bytesAvailable()) {
                lineData +=  this->port->readAll();
            }
        }

        /* Clean the binary data out of the read data */
        //lineData = lineData.simplified();
        lineData = lineData.replace(" ","");
        int size = lineData.size();
        QByteArray transData = QByteArray();
        for (int i = 0; i < size;i++) {
            if (isalnum(lineData[i])) {
                transData += lineData[i];
            }
            if ('\r' == lineData[i] || '\n' == lineData[i]) {
                transData += lineData[i];
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
        QString retval = transData;
        if (echo_chars > 0)
           retval = retval.mid(echo_chars+1);
        retval = retval.remove(retval.size()-2,2);

        qDebug() << "Retval in ReadALL: " <<retval;
        return retval;
    }

    /* Query the current RPM of the engine */
    const QString PortReaderWriter::queryRPM() {
        if (!this->sendCommand("01 0C")) { return QByteArray(); }
        return this->readAll(4);
    }

    /* Query the fuel intake amount */
    const QString PortReaderWriter::queryFuelLevelIntake() {
        if (!this->sendCommand("01 2F 1")) { return QByteArray(); }
        return this->readAll(5);
    }

    /* Query the list of error codes */
    const QString PortReaderWriter::queryOBDErrorCodes() {
        if (!this->sendCommand("01 01")) { return QString(); }
        return this->readAll(4);
    }

    /* Query the temperature of the engine */
    const QString PortReaderWriter::queryTempEngine() {
        if (!this->sendCommand("01 05 1")) { return QByteArray(); }
        return this->readAll(5);
    }

    /* Query for the current vehicle speed */
    const QString PortReaderWriter::queryVehicleSpeed() {
        if (!this->sendCommand("01 0D")) { return QByteArray(); }
        return this->readAll(4);
    }

    /* Query the current engine load */
    const QString PortReaderWriter::queryEngineLoad() {
        if (!this->sendCommand("01 04 1")) { return QString(); }
        return this->readAll(5);
    }

    /* Decode data from queryRPM() */
    const int PortReaderWriter::decodeRPM(const QString line_data) {
	qDebug() << "Raw RPM: " <<line_data;
        bool ok = false;
        QString retval =  line_data.mid(4);
        int x = retval.toInt(&ok, 16) / 4;
        return ok ? x : -1;
    }

    /* Decode data from queryTempEngine() */
    const int PortReaderWriter::decodeTempEngine(const QString line_data) {
        // Example data: 41057b
        bool ok = false;
        QString retval =  line_data.mid(4);
        int x = retval.toInt(&ok, 16) / 4;
        return ok ? x : -1;
    }

    const QString PortReaderWriter::decodeOBDErrors(const QString line_data)
    {
        /*
         *  41 01 respons to the request
         *  81 number of corrent troble code
         * 	81 - 80 or (129)- 120
         * 	most segnificant bit indicate that the  Ceck Engine Light on or of.
         */

        if(line_data.size() < 4)
        {
            qDebug() << line_data;
            return "";
        }

        QString teststr = "48 6B 10 43 03 25 01 10 11 05 55";
        QString newtest;
        QString retval = line_data;
        QString str_num_of_cods;

        qDebug() << retval;
        // cheking if data was not found
        if (retval.contains("unable", Qt::CaseInsensitive))
            return retval;

        qDebug() << "Trobel code: " <<line_data;
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

        /* remove the scratch codes from newtest */
        newtest ="";

        /* build up the return string from the errors array */
        for (int i=0; i<num_of_cods; i++) {
            newtest += errors[i] + "\n";
        }

        return newtest;
    }

    // Calculated engine load value in %
    const int PortReaderWriter::decodeEngineLoad(const QString line_data) {
        // Example: 0104f8
        bool ok = false;
        QString retval = line_data.mid(4);
        qDebug() << retval;
        int x = ( (retval.toInt(&ok, 16)) * 100)/255 ;
        return ok ? x : -1;
    }

    // Temperature of engine coolant in C
    const int PortReaderWriter::decodeEngineCoolantTemp(const QString line_data)
    {
        bool ok= false;
        QString retval = line_data.mid(4);
        qDebug() << retval;
        int x = (retval.toInt(&ok, 16)) - 40;
	return ok ? x : -1;
    }

    // Temperature of engine oil in C
    const int PortReaderWriter::decodeEngineOilTemp(const QString line_data)
    {
        //QString comm = "01 05 1"; // the code Enginr Tempereture

        //QString retval = "41 05 7B";
        bool ok = false;
        QString retval = line_data.mid(4);
        qDebug() << retval;
        int x = (retval.toInt(&ok, 16)) - 40;
	return ok ? x : -1;
    }

    // Fuel pressure kPa (gauge)
    const int PortReaderWriter::decodeFuelPressure(const QString line_data)
    {
        bool ok = false;
        QString retval =  line_data.mid(4);
        qDebug() << retval;
        int x = (retval.toInt(&ok, 16)) * 3;
	return ok ? x : -1;
    }

    // Intake manifold absolute pressure kPa (gauge)
    const int PortReaderWriter::decodeIntakeManifoldAbsolutePressure(const QString line_data)
    {
        bool ok = false;
        QString retval =  line_data.mid(4);
        int x = (retval.toInt(&ok, 16));
	return ok ? x : -1;
    }

    // Temperature of the intake air in C
    const int PortReaderWriter::decodeIntakeAirTemp(const QString line_data)
    {
        bool ok = false;
        QString retval = line_data.mid(4);
        qDebug() << retval;
        int x = (retval.toInt(&ok, 16)) - 40;
	return ok ? x : -1;
    }

    // Throttle position in %
    const int PortReaderWriter::decodeThrottlePosition(const QString line_data)
    {
        bool ok = false;
        QString retval = line_data.mid(4);
        qDebug() << retval;
        int x = ((retval.toInt(&ok, 16))*100)/255;
        return ok ? x : -1;
    }

    // Distance traveled with malfunction indicator lamp (MIL) on in km
    const int PortReaderWriter::decodeDistanceTraveledMIL(const QString line_data)
    {
        //QString retval = "41 0C 0F A0";
        bool oka = false;
        bool okb = false;
        QString retval =  line_data.mid(4);
        QString A = retval.left(2);
        QString B = retval.mid(2);
        qDebug() << retval;
        int x = ((retval.toInt(&oka, 16))*256) + B.toInt(&okb, 16);
        return (oka && okb) ? x : -1;
    }

    // Run time since engine starts in seconds.
    const int PortReaderWriter::decodeRunTimeEngineStart(const QString line_data)
    {
        QString retval = line_data.mid(4); 
        QString A = retval.left(2);
        QString B = retval.mid(2);
        qDebug() << retval;
        bool oka = false;
        bool okb = false;
        int x = ((retval.toInt(&oka, 16))*256) + B.toInt(&okb, 16);
        return (oka && okb) ? x : -1;
    }

    const int PortReaderWriter::decodeVehicleSpeed(QString line_data)
    {
        bool ok = false;
        QString retval = line_data;
        qDebug() << "RS: "<<line_data;
        retval = retval.mid(4);
        int x = retval.toInt(&ok, 16);
        qDebug() << "Speed: "<<x<<" hex: " << retval;
        if (!ok) qDebug() << "ERROR: "<< retval;
        return ok ? x : -1;
    }

    const int PortReaderWriter::decodeFuelLevelIntake(const QString line_data) {
        bool ok = false;
        QString retval = line_data.mid(4);
        qDebug() << retval;
        int x = ((retval.toInt(&ok, 16)) * 100) / 255 ;
        return ok ? x : -1;
    }

    // Barometric pressure kPa (Absolute)
    const int PortReaderWriter::decodeBarrometricPressure(const QString line_data)
    {
        bool ok = false;
        QString retval = line_data.mid(4);
        qDebug() << retval;
        int x = (retval.toInt(&ok, 16));
        return ok ? x : -1;
    }

    // Fuel/Air commanded equivalence ratio
    const int PortReaderWriter::decodeFuelAirRatio(const QString line_data)
    {
        bool oka = false;
        bool okb = false;
        QString retval = line_data.mid(4);
        QString B = retval.mid(2);
        qDebug() << retval;
        int x = ( ( (retval.toInt(&oka, 16)) * 256) + B.toInt(&okb, 16))/32768 ;
        return (oka && okb) ? x : -1;
    }

    // Ethanol fule % ratio
    const int PortReaderWriter::decodeEthanolFuelP(const QString line_data)
    {
        bool ok = false;
        QString retval = line_data.mid(4);
        qDebug() << retval;
        int x =  ( (retval.toInt(&ok, 16)) * 100)/255 ;
        return ok ? x : -1;
    }

    void PortReaderWriter::decodeMulty(const QByteArray line_data, int &codes)
    {
        // Only in connectionc is CAN (ISO 15765-4)

        // send value will be like: 01 04 05 0B 0C
        // 01 - mode
        // -------------------------
        // 04 - engine load
        // 05 - coolant temperatur
        // 0B - manifold pressure
        // 0C - engine RPM

        // QString retval = line_data;
        QString retval = "00A 0: 41 04 3F 05 44 0B 1: 21 0C 17 B8 00 00 00";

        QString temp;
        const int maxcod = 6;
        QString incode[maxcod] = {"04",
                             "05",
                             "0B",
                             "0C",
                             "",
                             ""};

        QString STRcodes[maxcod];
        int x = 0,
            y = 0;

        int CC[6];

        bool ok;

        retval = retval.replace(" ","");
        retval = retval.remove(0,3);

        while (( x = retval.indexOf(":",0,Qt::CaseInsensitive) ) != -1)
        {
               retval = retval.remove(x-1,2);
        }

        for (int i=1; i<maxcod; i++)
        {
            x = retval.indexOf(incode[i-1], 0, Qt::CaseInsensitive);
            y = retval.indexOf(incode[i], 0, Qt::CaseInsensitive);
            if ( (y-x-2) > 4 || (y-x-2) < 0)
                STRcodes[i-1] = retval.mid(x+2, 4);
            else
                STRcodes[i-1] = retval.mid(x+2, (y-x-2));
            CC[i-1] = STRcodes[i-1].toInt(&ok, 16);
        }


        qDebug() << retval;
        for (int i=0; i<maxcod; i++)
        {
            qDebug() << "In code: " <<incode[i];
            qDebug() << "the value: " <<STRcodes[i];
            qDebug() << "The desimal: " << CC[i];
            qDebug() << "----------------------------";
        }

    }

    bool PortReaderWriter::testSerial() {
        // Send AT I command, nearly every serial device will respond
        // sanely to the AT I command.  If your device does not work
        // with an AT I command, then you might want a better serial device
        sendCommand(QByteArray("AT I"));
        QString buff = this->readAll();
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
