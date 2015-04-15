#ifndef PORTREADERWRITER_H
#define PORTREADERWRITER_H
#include <QtSerialPort/QtSerialPort>
#include <QString>
#include <QVector>

using namespace std;

namespace serial {

    class PortReaderWriter : public QObject {
        Q_OBJECT

    public:
	static const int timeoutMillis = 5000;
        QSerialPort *port;
        QByteArray  readData;
        QByteArray  *writeData;
        QTimer m_timer;

        PortReaderWriter( QSerialPort *reqPort = 0
                        , QByteArray *data = 0
                        , QObject *parent = 0);
        ~PortReaderWriter();

        // Get the list of available ports
        static const QVector<QSerialPortInfo> getAvailPorts(void);

	// Set the connected port
	bool setPort(string portName);

        // Sets up the Serial Connection
        bool serialConnect(void);

        // Convieniece functions for determining state
        bool isConnected(void);

        // Test the serial interface of the connection
        bool testSerial(void);

        // Returns the OS specific name of the port
        QString const getConnectedPortName(void);

        // Low-Level interfaces to communicate via serial
        bool sendCommand(const QByteArray &data);
        QByteArray readAll();

        // High-Lever interfaces for specific OBDII data
        int decodeRPM(const QByteArray line_data);
        int decodeTempEngin(const QByteArray line_data);
        int decodeEnginLoad(const QByteArray line_data);
        int decodeVehicleSpeed(const QByteArray line_data);
        int decodeFuelLevelin(const QByteArray line_data);
        QString decodeErr(const QByteArray line_data);

    private slots:
        void handleReadReady(void);
        void handleTimeout(void);
        void handleError(QSerialPort::SerialPortError err);

    private:
        QList<QString> errorCodes;


    };
}

#endif
