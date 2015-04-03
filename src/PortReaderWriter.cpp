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

        // Just try it!
        this->port = new QSerialPort(QString(input_device.c_str()));
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

    bool PortReaderWriter::sendCommand(const QByteArray &data)
    {
        QByteArray tend = data + "\r\n";

        if (0 == this->port) { return false; }

        this->port->open(QIODevice::ReadWrite);

        if (!this->port->isOpen()) { return false; }
        if (-1 < port->write(tend)) { return true; }
        port->flush();
        if (this->port->waitForBytesWritten(timeoutMillis)) { return true; }

        return false;
    }

    QByteArray PortReaderWriter::readLine()
    {


        if (0 == this->port) { return "No port set!"; }
        this->port->open(QIODevice::ReadWrite);
            if (!this->port->isOpen()) { return "Could not open port for write"; }
        this->port->waitForReadyRead(timeoutMillis);
        QByteArray scratch = this->port->readAll();

        while (this->port->waitForReadyRead(timeoutMillis) )
        {
           scratch +=  this->port->readAll();
        }

/* *************************************************
        for (int i=0; i<scratch.size(); i++)
        {
            qDebug()<<i<<"\t"<< scratch.toHex().at(i)<<"\t"<<strint[i] << "\t"<< strchar[i];
        }
**************************************************** */

        QByteArray qbr = scratch;
//        qbr.remove(0,5);
        qbr.remove(qbr.size()-3,3);

        return qbr;
    }

    /***********************************************/
    int PortReaderWriter::decodeRPM(const QByteArray line_data)
    {
        //QString comm = "01 0C"; // the code for rpm
        sendCommand(line_data);

        // QString retval = "1af8";
        QString retval = line_data;

        int x = 0;
        bool ok;
        x = (retval.toInt(&ok, 16))/4;

        if(ok) return x;

        else return -1;
    }

    int PortReaderWriter::decodeTempEngin(const QByteArray line_data) {

        //QString comm = "01 05 1"; // the code Enginr Tempereture

        //QString retval = "41 05 7B";
        QString retval = line_data;

        // get rid of all the spases in the onput
        retval = retval.replace(" ","");

        retval =  retval.mid(4);

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

        QByteArray teststr = "48 6B 10 43 03 25 01 10 11 05 55";
        QString newtest;
        QString retval = line_data;
        QString str_num_of_cods;

        QString *errors;
        bool ok;
        QString str;
        int num_of_cods=0;


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
         *
         *  0101
         *	41 01 83 07 E5 A5 "
         *	ATH1
         *	OK"H1
         *	03
         *  48 6B 10 43 03 25 01 10 11 05 55
         *  0325 0110 1105 55
         */

        retval.replace(" ","");
        str = retval.replace(" ","");
        str_num_of_cods = str.mid(4,2);

        num_of_cods = (str_num_of_cods.toInt(&ok, 16)) - 128;

        errors = new QString[num_of_cods];
/*  turn the headers on if we have more then one error code *
        if(num_of_cods>1)
        {
            sendCommand("ATH1");
            teststr = readLine();
            teststr.remove(0,5);
            if(!teststr.contains("OK"))
               qDebug() << "ERROR!!!";
        }
/* reading all errors *
        sendCommand("03");
        teststr = readLine();
        teststr = teststr.remove(0,3);
/* */
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


    int PortReaderWriter::decodeEnginLoad(const QByteArray line_data)
    {

        //QString comm = "01 05 1"; // the code Enginr Tempereture

        //QString retval = "01 04 f8";
        QString retval = line_data;
        retval = retval.replace(" ","");
        retval = retval.mid(4);

        //qDebug() << retval;

        int x = 0;
        bool ok;

        x = ( (retval.toInt(&ok, 16)) * 100)/255 ;

        if(ok) return x;
        else return -1;
    }


    int PortReaderWriter::decodeVehicleSpeed(const QByteArray line_data)
    {

        //QString comm = "01 05 1"; // the code Enginr Tempereture

        QString retval = "01 04 f8";
        //QString retval = line_data;
        retval = retval.replace(" ","");
        retval = retval.mid(4);

        //qDebug() << retval;

        int x = 0;
        bool ok;

        x = (retval.toInt(&ok, 16));

        if(ok) return x;
        else return -1;
    }

    /********************************************************/

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
