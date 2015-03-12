#ifndef CARSERIAL_H
#define CARSERIAL_H
#include <QtSerialPort/QtSerialPort>
#include <QString>

using namespace std;

namespace serial
{
    class SerialComms
    {

    public:
	SerialComms();
	~SerialComms();

    bool serialConnect(void);

	QString readCommand();

	bool sendCommand(const QByteArray &data);

    private:
	QSerialPort *port;

    };
}

#endif
