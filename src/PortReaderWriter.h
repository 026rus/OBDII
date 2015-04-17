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
    static const int timeoutMillis = 1000;
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
        static const int decodeDistanceTraveledMIL(const QByteArray line_data);
        static const int decodeBarrometricPressure(const QByteArray line_data);
        static const int decodeEthanolFuelP(const QByteArray line_data);
        static const int decodeEngineCoolantTemp(const QByteArray line_data);
        static const int decodeEngineLoad(const QByteArray line_data);
        static const int decodeEngineOilTemp(const QByteArray line_data);
        static const int decodeFuelPressure(const QByteArray line_data);
        static const int decodeFuelAirRatio(const QByteArray line_data);
        static const int decodeFuelLevelIntake(const QByteArray line_data);
        static const int decodeIntakeManifoldAbsolutePressure(const QByteArray line_data);
        static const int decodeIntakeAirTemp(const QByteArray line_data);
        static const int decodeRPM(const QByteArray line_data);
        static const int decodeRunTimeEngineStart(const QByteArray line_data);
        static const int decodeTempEngine(const QByteArray line_data);
        static const int decodeThrottlePosition(const QByteArray line_data);
        static const int decodeVehicleSpeed(QByteArray line_data);

        static const QString decodeOBDErrors(const QByteArray line_data);
        
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
        QByteArray readAll(int echo_chars = 0);

        // High-Lever interfaces for specific OBDII data
        // These functions do perform I/O to get data
        const QByteArray queryDistanceTraveledMIL(void);
        const QByteArray queryBarrometricPressure(void);
        const QByteArray queryEthanolFuelP(void);
        const QByteArray queryEngineCoolantTemp(void);
        const QByteArray queryEngineLoad(void);
        const QByteArray queryEngineOilTemp(void);
        const QByteArray queryFuelPressure(void);
        const QByteArray queryFuelAirRatio(void);
        const QByteArray queryFuelLevelIntake(void);
        const QByteArray queryIntakeManifoldAbsolutePressure(void);
        const QByteArray queryIntakeAirTemp(void);
        const QByteArray queryOBDErrorCodes(void);
        const QByteArray queryRPM(void);
        const QByteArray queryRunTimeEngineStart(void);
        const QByteArray queryTempEngine(void);
        const QByteArray queryThrottlePosition(void);
        const QByteArray queryVehicleSpeed(void);

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
