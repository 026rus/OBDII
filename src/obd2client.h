#ifndef OBD2CLIENT_H
#define OBD2CLIENT_H

#include <QtNetwork>
#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QHostAddress>



class OBD2Client: public QThread
{
Q_OBJECT
public:

    OBD2Client(QObject* parent = 0);
    ~OBD2Client();

    void start(QString address, quint16 port);
    void start();
//    void run() Q_DECL_OVERRIDE;

    void setHostAddres(QString a);
    void setHostPort(quint16 p);
    void setMassage(QString);

public slots:

    void sendData();

    void connectto(QString, quint16);

private:

    QTcpSocket client;

    QString massage;

    QString HostAddres;
    quint16 HostPort;
    QTcpSocket socet;
    QMutex mutex;
    QWaitCondition cond;
    bool quit;
};

#endif // OBD2CLIENT_H
