#include "obd2client.h"

OBD2Client::OBD2Client(QObject* parent): QThread(parent)
{

    massage = "Test connection";
    HostAddres = "127.0.0.1";
    HostPort = 16383;
}

OBD2Client::~OBD2Client()
{
  client.close();
}

void OBD2Client::start(QString address, quint16 port)
{
  QHostAddress addr(address);
  client.connectToHost(addr, port);
}

void OBD2Client::start()
{
  QHostAddress addr(HostAddres);
  client.connectToHost(addr, HostPort);
  sendData();
}

void OBD2Client::sendData()
{
    QByteArray bm= massage.toUtf8();

  client.write( bm );
}


void OBD2Client::setHostAddres(QString a)
{
    HostAddres = a;
}

void OBD2Client::setHostPort(quint16 p)
{
    HostPort = p;
}

void OBD2Client::connectto(QString str, quint16 p)
{
    setHostAddres(str);
    setHostPort(p);
    start();
}

void OBD2Client::setMassage(QString str)
{
    massage = str;
}
