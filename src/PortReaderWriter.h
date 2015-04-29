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
    int timeoutMillis;
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

        // Decoding functions for line data retrieved from query functions
        // These functions do not perform I/O actions.
        static const int decodeDistanceTraveledMIL(const QString line_data);
        static const int decodeBarrometricPressure(const QString line_data);
        static const int decodeEthanolFuelP(const QString line_data);
        static const int decodeEngineCoolantTemp(const QString line_data);
        static const int decodeEngineLoad(const QString line_data);
        static const int decodeEngineOilTemp(const QString line_data);
        static const int decodeFuelPressure(const QString line_data);
        static const int decodeFuelAirRatio(const QString line_data);
        static const int decodeFuelLevelIntake(const QString line_data);
        static const int decodeIntakeManifoldAbsolutePressure(const QString line_data);
        static const int decodeIntakeAirTemp(const QString line_data);
        static const int decodeRPM(const QString line_data);
        static const int decodeRunTimeEngineStart(const QString line_data);
        static const int decodeTempEngine(const QString line_data);
        static const int decodeThrottlePosition(const QString line_data);
        static const int decodeVehicleSpeed(QString line_data);

        static const QString decodeOBDErrors(const QString line_data);
        
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
        QString readAll(int echo_chars = 0);

        // High-Lever interfaces for specific OBDII data
        // These functions do perform I/O to get data
        const QString queryDistanceTraveledMIL(void);
        const QString queryBarrometricPressure(void);
        const QString queryEthanolFuelP(void);
        const QString queryEngineCoolantTemp(void);
        const QString queryEngineLoad(void);
        const QString queryEngineOilTemp(void);
        const QString queryFuelPressure(void);
        const QString queryFuelAirRatio(void);
        const QString queryFuelLevelIntake(void);
        const QString queryIntakeManifoldAbsolutePressure(void);
        const QString queryIntakeAirTemp(void);
        const QString queryOBDErrorCodes(void);
        const QString queryRPM(void);
        const QString queryRunTimeEngineStart(void);
        const QString queryTempEngine(void);
        const QString queryThrottlePosition(void);
        const QString queryVehicleSpeed(void);

        void decodeMulty(const QByteArray line_data, int &codes);

    private slots:
        void handleReadReady(void);
        void handleTimeout(void);
        void handleError(QSerialPort::SerialPortError err);

    private:
        QList<QString> OBDErrorCodes;


    };
}

#endif
