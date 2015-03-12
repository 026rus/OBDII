#ifndef CARSERIAL_H
#define CARSERIAL_H
#include <QtSerialPort/QtSerialPort>
#include <QString>

using namespace std;

namespace serial {

    class SerialComms {

    public:
	SerialComms();
	~SerialComms();

    bool serialConnect(void);
    bool sendCommand(const QByteArray &data);

    int decodeRPM(const QByteArray line_data);
    int decodeTempEngin(const QByteArray line_data);
    QString decodeErr(const QByteArray line_data);

    QByteArray readLine();

    private:
	QSerialPort *port;

    };
}

#endif
