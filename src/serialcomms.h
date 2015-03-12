#ifndef CARSERIAL_H
#define CARSERIAL_H
#include <QtSerialPort/QtSerialPort>
#include <QString>

using namespace std;

namespace serial {

    class PortReaderWriter : public QObject {
        Q_OBJECT

    public:
        QSerialPort *port;
        QByteArray  readData;
        const QByteArray  *writeData;

        PortReaderWriter( QSerialPort *reqPort = nullptr
                        , const QByteArray *data = 0
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
        void handleWriteReady(const QByteArray data);
        void handleTimeout();
        void handleError(QSerialPort::SerialPortError err);
        void shutdown();

    private:
        QTimer m_timer;

    };
}

#endif
