#include "obd2client.h"


OBD2Client::OBD2Client(QObject* parent): QObject(parent)
{
  massage = "Test connection";
  connect(&client, SIGNAL(connected()),
    this, SLOT( sendData() ));
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
}

void OBD2Client::sendData()
{

  client.write( massage.toLatin1(), (qint64)(massage.length()) );
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
