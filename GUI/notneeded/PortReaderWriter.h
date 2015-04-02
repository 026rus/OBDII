#ifndef PORTREADERWRITER_H
#define PORTREADERWRITER_H
#include <QtSerialPort/QtSerialPort>
#include <QString>

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

        // Sets up the Serial Connection
        bool serialConnect(void);

        // Convieniece functions for determining state
        bool isConnected();
        QString getConnectedPortName();

        // Low-Level interfaces to communicate via serial
        bool sendCommand(const QByteArray &data);
        QByteArray readLine();


        // High-Lever interfaces for specific OBDII data
        int decodeRPM(const QByteArray line_data);
        int decodeTempEngin(const QByteArray line_data);
        QString decodeErr(const QByteArray line_data);

    private slots:
        void handleReadReady();
        void handleTimeout();
        void handleError(QSerialPort::SerialPortError err);

    private:
        QList<QString> errorCodes;


    };
}

#endif
